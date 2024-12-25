#include <stdint.h>
#include "stm32f10x.h"

static volatile uint64_t g_sysRunTime = 0;

static void (*g_pTaskScheduleFunc)(void);          // 函数指针变量，保存任务调度的函数地址

/**
***********************************************************
* @brief 注册任务调度回调函数
* @param pFunc, 传入回调函数地址
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void))
{
	g_pTaskScheduleFunc = pFunc;
}
void SysTick_Handler(void)
{
	g_sysRunTime++;
	g_pTaskScheduleFunc();
}

void SysTick_Init(void) 
{
    // SystemCoreClock 是系统的核心时钟频率，一般是72MHz
    // SysTick_Config 参数为 SystemCoreClock / 1000，产生 1ms 的定时中断
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}
   // SysTick_Config(SystemCoreClock / 1000);  
}

/**
***********************************************************
* @brief 获取系统运行时间
* @param
* @return 以1ms为单位
***********************************************************
*/
uint64_t GetSysRunTime(void)
{
	return g_sysRunTime;
}
