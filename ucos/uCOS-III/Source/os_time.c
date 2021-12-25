#include "os.h"

void OSTimeTick(void)
{
	unsigned int i;

	

	/* ɨ������б������������TaskDelayTicks�������Ϊ0�����1 */
	for(i=0; i<OS_CFG_PRIO_MAX; i++)
	{
		if(OSRdyList[i].HeadPtr->TaskDelayTicks > 0)
		{
			OSRdyList[i].HeadPtr->TaskDelayTicks --;
		}
	}
	
	/* ������� */
	OSSched();
}

void OSTimeDly(OS_TICK dly)
{
    OSTCBCurPtr->TaskDelayTicks = dly;
    OSSched();
}