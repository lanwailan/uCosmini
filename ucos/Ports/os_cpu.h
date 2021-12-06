#ifndef  OS_CPU_H
#define  OS_CPU_H

/*
*********************************************************************************************************
*                                               ºê¶¨Òå
*********************************************************************************************************
*/

#ifndef  NVIC_INT_CTRL
#define  NVIC_INT_CTRL                      *((CPU_REG32 *)0xE000ED04)   /* ÖÐ¶Ï¿ØÖÆ¼°×´Ì¬¼Ä´æÆ÷ SCB_ICSR */
#endif

#ifndef  NVIC_PENDSVSET
#define  NVIC_PENDSVSET                                    0x10000000    /* ´¥·¢PendSVÒì³£µÄÖµ Bit28£ºPENDSVSET */
#endif

#define  OS_TASK_SW()               NVIC_INT_CTRL = NVIC_PENDSVSET
#define  OSIntCtxSw()               NVIC_INT_CTRL = NVIC_PENDSVSET
/*
*********************************************************************************************************
*                                              º¯ÊýÉùÃ÷
*********************************************************************************************************
*/
void OSStartHighRdy(void);
void PendSV_Handler(void);


void OS_CPU_SysTickInit(CPU_INT32U ms);
void SysTick_Handler(void);

#endif   /* OS_CPU_H */

