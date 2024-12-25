#include "stm32f10x.h"
#include "usart.h"
/**
***********************************************************
* @brief 配置串口时钟，使能引脚 PA10 -> Rx PA9 -> Tx
* @param
* @return 
***********************************************************
*/
static void usart_gpio_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitTypeDef GPIO_Structure;
	GPIO_Structure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	GPIO_Structure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_Structure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Structure);
	
	GPIO_Structure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_Structure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_Structure);

}
 
/**
***********************************************************
* @brief 配置串口
* @param
* @return 
***********************************************************
*/
static void usart_config(uint32_t baudrate)
{
	USART_DeInit(USART1);
	
	USART_InitTypeDef USART_Structure;
	
	USART_Structure.USART_BaudRate 				= baudrate;
	USART_Structure.USART_WordLength 			= USART_WordLength_8b;
	USART_Structure.USART_StopBits				= USART_StopBits_1;
	USART_Structure.USART_Parity 				= USART_Parity_No;
	USART_Structure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_Structure.USART_Mode 					= USART_Mode_Tx;
	
	USART_Init(USART1, &USART_Structure);
	
	NVIC_InitTypeDef NVIC_Structure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_Structure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Structure.NVIC_IRQChannelSubPriority        = 2;
	NVIC_Structure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_Structure);
	
	USART_Cmd(USART1, ENABLE);                    //使能串口1
}

/**
***********************************************************
* @brief 串口初始化
* @param
* @return 
***********************************************************
*/
void usart_drv(void)
{
	usart_gpio_init();
	usart_config(9600);
}

/**
***********************************************************
* @brief 重定义串口
* @param
* @return 
***********************************************************
*/
int fputc(int ch,FILE *f)
{
    USART_SendData(USART1, (uint8_t)ch);

    // 等待发送完成
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return ch;
}
