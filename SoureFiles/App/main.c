#include "led.h"
#include "key.h"
#include "stm32f10x.h"
#include "systick.h"
#include "hmi_app.h"
#include "usart.h"
typedef struct
{
    uint8_t   run;
    uint16_t  timCount;
    uint16_t  timRolad;
    void      (*pTaskFuncCb)(void);
} TaskComps_t;

static TaskComps_t g_Task_List[] =
{
    {0, 5, 5, HmiTask},
};

#define TASK_NUMS	(sizeof(g_Task_List)/sizeof(g_Task_List[0]))

static void TaskHandle(void)
{
    for(uint8_t i = 0; i < TASK_NUMS; i++)
    {
        if(g_Task_List[i].run)
        {
            g_Task_List[i].run = 0;
            g_Task_List[i].pTaskFuncCb();
        }
    }

}

/**
***********************************************************
* @brief 在定时器中断服务函数中被间接调用，设置时间片标记，
         需要定时器1ms产生1次中断
* @param
* @return
***********************************************************
*/
static void TaskScheduleCb(void)
{
    for(uint8_t i = 0; i < TASK_NUMS; i++)
    {
        if(g_Task_List[i].timCount)
        {
            g_Task_List[i].timCount--;
            if(g_Task_List[i].timCount == 0)
            {
                g_Task_List[i].run = 1;
                g_Task_List[i].timCount = g_Task_List[i].timRolad;
            }
        }
    }
}

static void DrvInit(void)
{
    SysTick_Init();
//	delay_init();	    	 //延时函数初始化
    LED_Init();		  	//初始化与LED连接的硬件接口
    key_config();
	usart_drv();
}
static void AppInit(void)
{
    TaskScheduleCbReg(TaskScheduleCb);
}

int main(void)
{
    DrvInit();
    AppInit();
    while(1)
    {
	
        TaskHandle();
    }
}

