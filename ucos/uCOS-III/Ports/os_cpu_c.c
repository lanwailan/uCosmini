#include "os.h"
#include "ARMCM3.h"
CPU_STK *OSTaskStkInit(OS_TASK_PTR       p_task,
                        void             *p_arg,
                        CPU_STK          *p_stk_base,
                        CPU_STK_SIZE     stk_size)
{
    CPU_STK *p_stk;
    /* ARMCM3 处理器的栈是由高地址向低地址生长，获取到栈顶地址，地址逐一递减即可 */
    p_stk = &p_stk_base[stk_size];

    *--p_stk = (CPU_STK)0x01000000u;     /*xPSR 的bit24必须置为1 */
    *--p_stk = (CPU_STK)p_task;         /* R15(PC)任务入口的地址*/
    *--p_stk = (CPU_STK)0x14141414u;    /* R14(LR)              */
    *--p_stk = (CPU_STK)0x12121212u;    /* R12                 */
    *--p_stk = (CPU_STK)0x03030303u;    /* R3                  */
    *--p_stk = (CPU_STK)0x02020202u;    /* R2                  */
    *--p_stk = (CPU_STK)0x01010101u;    /* R1                  */
    *--p_stk = (CPU_STK)p_arg;          /* R0 : 任务形参*/
    /* 异常发生时需手动保存的寄存器 */
    *--p_stk = (CPU_STK)0x11111111u;    /* R11                 */
    *--p_stk = (CPU_STK)0x10101010u;    /* R10                 */
    *--p_stk = (CPU_STK)0x09090909u;    /* R9                  */
    *--p_stk = (CPU_STK)0x08080808u;    /* R8                  */
    *--p_stk = (CPU_STK)0x07070707u;    /* R7                  */
    *--p_stk = (CPU_STK)0x06060606u;    /* R6                  */
    *--p_stk = (CPU_STK)0x05050505u;    /* R5                  */
    *--p_stk = (CPU_STK)0x04040404u;    /* R4                  */

    return (p_stk);
}

#if(0)
void OS_CPU_SysTickInit(CPU_INT32U cnts)
{
    CPU_INT32U prio;

    /* 填写Systick 的重载计数值 */
    CPU_REG_NVIC_ST_RELOAD = cnts -1;

    /* 设置SysTick中断的优先级 */
    prio = CPU_REG_NVIC_SHPRI3;
    prio &= DEF_BIT_FIELD(24,0);
    prio |= DEF_BIT_MASK(OS_CPU_CFG_SYSTICK_PRIO,24);

    CPU_REG_NVIC_SHPRI3 = prio;

    /* 启用SysTick的时钟源和启动计数器 */

    CPU_REG_NVIC_ST_CTRL |= CPU_REG_NVIC_ST_CTRL_CLKSOURCE | CPU_REG_NVIC_ST_CTRL_ENABLE;
    CPU_REG_NVIC_ST_CTRL |= CPU_REG_NVIC_ST_CTRL_TICKINT;
}
#endif

void OS_CPU_SysTickInit(CPU_INT32U ms)
{
    SysTick->LOAD = ms*SystemCoreClock/1000 -1;  //

    NVIC_SetPriority(SysTick_IRQn,(1<< __NVIC_PRIO_BITS)-1);

    SysTick->VAL = 0;

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |   // 选择时钟源
                        SysTick_CTRL_TICKINT_Msk |  // 启用中断
                        SysTick_CTRL_ENABLE_Msk;   // 启用计数器开始
}

void SysTick_Handler(void)
{
    OSTimeTick();
}