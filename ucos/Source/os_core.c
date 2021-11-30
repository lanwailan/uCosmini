#include "os.h"

void OS_RdyListInit(void)
{
    OS_PRIO i;
    OS_RDY_LIST *p_rdy_list;

    for(i=0u;i<OS_CFG_PRIO_MAX;i++)
    {
        p_rdy_list = &OSRdyList[i];
        p_rdy_list->HeadPtr = (OS_TCB *)0;
        p_rdy_list->TailPtr = (OS_TCB *)0;
    }
}

void OSInit(OS_ERR *p_err)
{
    /* OS global running status */
    OSRunning = OS_STATE_OS_STOPPED;


    OSTCBCurPtr = (OS_TCB *)0;

    OSTCBHighRdyPtr = (OS_TCB *)0;

    OS_RdyListInit();

    *p_err = OS_ERR_NONE;
}

void OSStart(OS_ERR *p_err)
{
    if(OSRunning == OS_STATE_OS_STOPPED)
    {
        OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;

        /* 用于启动任务切换，即配置PendSV的优先级为最低，然后触发PendSV异常，在pendsv异常服务函数中进行任务切换*/
        OSStartHighRdy();

        *p_err = OS_ERR_FATAL_RUNNING;
    }
    else
    {
        *p_err = OS_STATE_OS_RUNNING;
    }
}