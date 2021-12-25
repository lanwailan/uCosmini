#include "os.h"

void OSTimeTick(void)
{
	unsigned int i;

	

	/* 扫描就绪列表中所有任务的TaskDelayTicks，如果不为0，则减1 */
	for(i=0; i<OS_CFG_PRIO_MAX; i++)
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks --;
		}
	}
	
	/* 任务调度 */
	OSSched();
}

void OSTimeDly(OS_TICK dly)
{
    OSTCBCurPtr->TaskDelayTicks = dly;
    OSSched();
}