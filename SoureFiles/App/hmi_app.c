#include <stdint.h>
#include <stdio.h>
#include "key.h"
#include "led.h"
/**
***********************************************************
* @brief 人机交互任务处理函数
* @param 
* @return 
***********************************************************
*/
void HmiTask(void)
{
//	RtcTime_t rtcTime;
//	GetRtcTime(&rtcTime);
//	printf("%d-%02d-%02d %02d:%02d:%02d\n", rtcTime.year, rtcTime.month, rtcTime.day,
//											rtcTime.hour,rtcTime.minute, rtcTime.second);
	uint8_t keyVal;
	
	keyVal = GetKeyVal();
		switch (keyVal)
		{
			case KEY1_SHORT_PRESS:

				TurnOnLed(LED0);
				break;
			case KEY1_LONG_PRESS:

				TurnOffLed(LED0);
				break;
			case KEY2_SHORT_PRESS:

				TurnOnLed(LED1);
				break;
			case KEY2_LONG_PRESS:
				
				TurnOffLed(LED1);
				break;
			default:
				break;
		}
}
