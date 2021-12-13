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
    OS_IdleTaskInit(p_err);
    
    if(*p_err != OS_ERR_NONE)
    {
        return;
    }
}

void OSStart(OS_ERR *p_err)
{
    if(OSRunning == OS_STATE_OS_STOPPED)
    {
        OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;

        /* 用于启动任务切换，即配置PendSV的优先级为最低，然后触发PendSV异常，在pendsv异常服务函数中进行任务切换*/
        OSStartHighRdy();

        *p_err = OS_ERR_FATAL_RETURN;
    }
    else
    {
        *p_err = OS_STATE_OS_RUNNING;
    }
}

void OSSched (void)
{
#if(0)
    if( OSTCBCurPtr == OSRdyList[0].HeadPtr )
    {
        OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
    }
    else
    {
        OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
    }
    
    OS_TASK_SW();
#endif

    if(OSTCBCurPtr == &OSIdleTaskTCB)
    {
        if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
        {
            OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
        }
        else if(OSRdyList[1].HeadPtr->TaskDelayTicks ==0)
        {
            OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
        }
        else
        {
            return;
        }
    }
    else
    {
        if(OSTCBCurPtr == OSRdyList[0].HeadPtr)
        {
            if(OSRdyList[1].HeadPtr->TaskDelayTicks == 0)
            {
                OSTCBHighRdyPtr = OSRdyList[1].HeadPtr;
            }
            else if(OSTCBCurPtr->TaskDelayTicks != 0)
            {
                OSTCBHighRdyPtr = &OSIdleTaskTCB;
            }
            else
            {
                return;
            }
            
        }
        else if(OSTCBCurPtr == OSRdyList[1].HeadPtr)
        {
            if(OSRdyList[0].HeadPtr->TaskDelayTicks == 0)
            {
                OSTCBHighRdyPtr = OSRdyList[0].HeadPtr;
            }
            else if(OSTCBCurPtr->TaskDelayTicks != 0)
            {
                OSTCBHighRdyPtr = &OSIdleTaskTCB;
            }
            else
            {
                return;
            }
        }
        
    }
    OS_TASK_SW();
}

void OS_IdleTask(void *p_arg)
{
    p_arg = p_arg;

    for(;;)
    {
        OSIdleTaskCtr ++;
    }
}

void OS_IdleTaskInit(OS_ERR *p_err)
{
    OSIdleTaskCtr = (OS_IDLE_CTR)0;

    OSTaskCreate(
        (OS_TCB *)&OSIdleTaskTCB,
        (OS_TASK_PTR)OS_IdleTask,
        (void *)0,
        (CPU_STK *)OSCfg_IdleTaskStkBasePtr,
        (CPU_STK_SIZE)OSCfg_IdleTaskStkSize,
        (OS_ERR *)p_err
        );
}