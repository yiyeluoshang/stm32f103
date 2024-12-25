#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include <stdint.h> 

void SysTick_Init(void); 
uint64_t GetSysRunTime(void);

/**
***********************************************************
* @brief 注册任务调度回调函数
* @param pFunc, 传入回调函数地址
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void));
#endif
