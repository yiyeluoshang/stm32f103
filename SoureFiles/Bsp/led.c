#include "led.h"

typedef struct
{
	uint32_t rcu;
	GPIO_TypeDef  *gpio;
	uint16_t pin;
}LED_GPIO_t;

static LED_GPIO_t g_led_gpio_list[] =
{
{RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8},
{RCC_APB2Periph_GPIOD, GPIOD, GPIO_Pin_2}
};

#define LED_NUM_MAX  (sizeof(g_led_gpio_list)/sizeof(g_led_gpio_list[0]))
/**
***********************************************************
* @brief LED硬件初始化
* @param
* @return 
***********************************************************
*/
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz	
	for(uint8_t i = 0; i < LED_NUM_MAX; i++)
	{
		RCC_APB2PeriphClockCmd(g_led_gpio_list[i].rcu, ENABLE);
		GPIO_InitStructure.GPIO_Pin = g_led_gpio_list[i].pin;
		GPIO_Init(g_led_gpio_list[i].gpio, &GPIO_InitStructure);	
		GPIO_SetBits(g_led_gpio_list[i].gpio,g_led_gpio_list[i].pin);	
	}

 	
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟
//	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 端口配置
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
// GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
// GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 输出高

// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 端口配置, 推挽输出
// GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
// GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 输出高 
}
 
/**
***********************************************************
* @brief 点亮LED
* @param ledNo，LED标号，0~1
* @return 
***********************************************************
*/
void TurnOnLed(uint8_t ledNo)
{
	if (ledNo >= LED_NUM_MAX)
	{
		return;
	}
	GPIO_ResetBits(g_led_gpio_list[ledNo].gpio,g_led_gpio_list[ledNo].pin);
}

/**
***********************************************************
* @brief 点亮LED
* @param ledNo，LED标号，0~1
* @return 
***********************************************************
*/
void TurnOffLed(uint8_t ledNo)
{
	if (ledNo >= LED_NUM_MAX)
	{
		return;
	}
	GPIO_SetBits(g_led_gpio_list[ledNo].gpio,g_led_gpio_list[ledNo].pin);
}
