#include <stdint.h>
#include "stm32f10x.h"

static volatile uint64_t g_sysRunTime = 0;

static void (*g_pTaskScheduleFunc)(void);          // ����ָ�����������������ȵĺ�����ַ

/**
***********************************************************
* @brief ע��������Ȼص�����
* @param pFunc, ����ص�������ַ
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
    // SystemCoreClock ��ϵͳ�ĺ���ʱ��Ƶ�ʣ�һ����72MHz
    // SysTick_Config ����Ϊ SystemCoreClock / 1000������ 1ms �Ķ�ʱ�ж�
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}
   // SysTick_Config(SystemCoreClock / 1000);  
}

/**
***********************************************************
* @brief ��ȡϵͳ����ʱ��
* @param
* @return ��1msΪ��λ
***********************************************************
*/
uint64_t GetSysRunTime(void)
{
	return g_sysRunTime;
}
