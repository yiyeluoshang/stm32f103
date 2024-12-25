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
* @brief LEDӲ����ʼ��
* @param
* @return 
***********************************************************
*/
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz	
	for(uint8_t i = 0; i < LED_NUM_MAX; i++)
	{
		RCC_APB2PeriphClockCmd(g_led_gpio_list[i].rcu, ENABLE);
		GPIO_InitStructure.GPIO_Pin = g_led_gpio_list[i].pin;
		GPIO_Init(g_led_gpio_list[i].gpio, &GPIO_InitStructure);	
		GPIO_SetBits(g_led_gpio_list[i].gpio,g_led_gpio_list[i].pin);	
	}

 	
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
//	
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA.8 �˿�����
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
// GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
// GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PA.8 �����

// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD.2 �˿�����, �������
// GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
// GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 //PD.2 ����� 
}
 
/**
***********************************************************
* @brief ����LED
* @param ledNo��LED��ţ�0~1
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
* @brief ����LED
* @param ledNo��LED��ţ�0~1
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
