#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#define  LED0 0
#define  LED1 1 

//#define LED0 PAout(8)	// PA8
//#define LED1 PDout(2)	// PD2	

void LED_Init(void);//初始化
/**
***********************************************************
* @brief 点亮LED
* @param ledNo，LED标号，0~2
* @return 
***********************************************************
*/
void TurnOnLed(uint8_t ledNo);

/**
***********************************************************
* @brief 熄灭LED
* @param ledNo，LED标号，0~2
* @return 
***********************************************************
*/
void TurnOffLed(uint8_t ledNo);
		 				    
#endif
