#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include <stdint.h> 

void SysTick_Init(void); 
uint64_t GetSysRunTime(void);

/**
***********************************************************
* @brief ע��������Ȼص�����
* @param pFunc, ����ص�������ַ
* @return 
***********************************************************
*/
void TaskScheduleCbReg(void (*pFunc)(void));
#endif
