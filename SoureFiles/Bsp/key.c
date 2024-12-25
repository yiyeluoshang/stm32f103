#include <stdint.h>
#include "key.h"
#include "systick.h"

typedef struct
{
	uint32_t rcu;	
	uint16_t pin; 
	GPIO_TypeDef* gpio;
}Key_GPIO_t;

static Key_GPIO_t gpio_list[] =
{
	{RCC_APB2Periph_GPIOA, GPIO_Pin_15, GPIOA},
	{RCC_APB2Periph_GPIOC, GPIO_Pin_5, GPIOC}
//	{RCC_APB2Periph_GPIOA, GPIO_Pin_15, GPIOA}
//	{RCC_APB2Periph_GPIOA, GPIO_Pin_0, GPIOA}
};

#define KEY_NUM_MAX (sizeof(gpio_list) / sizeof(gpio_list[0]))

void key_config(void)
{
	GPIO_InitTypeDef GPIO_Structure;
	
	GPIO_Structure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;

	for(uint8_t i = 0; i < KEY_NUM_MAX; i++)
	{
		RCC_APB2PeriphClockCmd(gpio_list[i].rcu,ENABLE);
		GPIO_Structure.GPIO_Pin = gpio_list[i].pin;
		GPIO_Init(gpio_list[i].gpio, &GPIO_Structure);
	}

}

typedef  enum 
{
	KEY_RELEASE = 0,
	KEY_CONFIRM,
	KEY_SHORTPRESS,
	KEY_LONGPRESS

}KEY_STATE;

#define CONFIRM_TIME                 10       // 按键消抖时间窗10ms
#define DOUBLE_CLICK_INTERVAL_TIME   300      // 两次抬起时间窗300ms，用来判断是否双击
#define LONGPRESS_TIME               1000     // 长按时间窗1000ms

typedef struct
{
	KEY_STATE keyState;
	uint8_t singleClickNum;
	uint64_t firstIoChangeSysTime;
	uint64_t firstReleaseSysTime;
} Key_Info_t;

static Key_Info_t g_keyInfo[KEY_NUM_MAX];

static uint8_t KeyScan(uint8_t keyIndex)
{
	volatile uint64_t curSysTime;
	uint8_t keyPress;
	
	//keyPress = gpio_input_bit_get(g_gpioList[keyIndex].gpio, g_gpioList[keyIndex].pin);
	keyPress =GPIO_ReadInputDataBit(gpio_list[keyIndex].gpio,gpio_list[keyIndex].pin);
	switch (g_keyInfo[keyIndex].keyState)
	{
		case KEY_RELEASE:                                          //  释放状态：判断有无按键按下
			if (!keyPress)                                         //  有按键按下
			{ 
				g_keyInfo[keyIndex].keyState = KEY_CONFIRM;        //  获取系统运行时间
				g_keyInfo[keyIndex].firstIoChangeSysTime = GetSysRunTime();
				break;
			}

			if (g_keyInfo[keyIndex].singleClickNum != 0)
			{
				curSysTime = GetSysRunTime();
				if (curSysTime - g_keyInfo[keyIndex].firstReleaseSysTime > DOUBLE_CLICK_INTERVAL_TIME)
				{
					g_keyInfo[keyIndex].singleClickNum = 0;
					return (keyIndex + 1); //  返回单击按键码值，三个按键短按对应0x01, 0x02, 0x03
				}
			}

			break;
			
		case KEY_CONFIRM:
			if (!keyPress)
			{
				curSysTime = GetSysRunTime();
				if (curSysTime - g_keyInfo[keyIndex].firstIoChangeSysTime > CONFIRM_TIME) //  超过时间窗，切换到短按
				{
					g_keyInfo[keyIndex].keyState = KEY_SHORTPRESS;
				}
			}
			else
			{
				g_keyInfo[keyIndex].keyState = KEY_RELEASE;
			}
			break;
			
		case KEY_SHORTPRESS:           //  短按状态：继续判定是短按，还是双击，还是长按
			if (keyPress)
			{
				g_keyInfo[keyIndex].keyState = KEY_RELEASE;
				
				g_keyInfo[keyIndex].singleClickNum++;
				if (g_keyInfo[keyIndex].singleClickNum == 1)
				{
					g_keyInfo[keyIndex].firstReleaseSysTime = GetSysRunTime();
					break;
				}
				else
				{
					curSysTime = GetSysRunTime();
					if (curSysTime - g_keyInfo[keyIndex].firstReleaseSysTime <= DOUBLE_CLICK_INTERVAL_TIME) //  超过双击时间间隔窗，返回双击码值
					{
						g_keyInfo[keyIndex].singleClickNum = 0;
						return (keyIndex + 0x51); //  返回按键码值，三个按键双击对应0x51, 0x52, 0x53
					}
				}
			}
			else
			{
				curSysTime = GetSysRunTime();
				if (curSysTime - g_keyInfo[keyIndex].firstIoChangeSysTime > LONGPRESS_TIME)
				{	
					g_keyInfo[keyIndex].keyState = KEY_LONGPRESS;
				}
			}
			break;
		case KEY_LONGPRESS:
			if (keyPress)
			{
				g_keyInfo[keyIndex].keyState = KEY_RELEASE;
				return (keyIndex + 0x81); //返回按键码值，三个按键长按对应0x81, 0x82, 0x83
			}
			break;
		default:
			g_keyInfo[keyIndex].keyState = KEY_RELEASE;
			break;
	}
	return 0;
}

/**
***********************************************************
* @brief 获取按键码值
* @param
* @return 三个按键码值，短按0x01 0x02 0x03，
			长按0x81 0x82 0x83，没有按下为0
***********************************************************
*/
uint8_t GetKeyVal(void)
{
	uint8_t res = 0;

	for (uint8_t i = 0; i < KEY_NUM_MAX; i++)
	{
		res = KeyScan(i);
		if (res != 0)
		{
			break;
		}
	}
	return res;
}
