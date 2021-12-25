
    IMPORT OSTCBCurPtr    ;
    IMPORT OSTCBHighRdyPtr

    EXPORT OSStartHighRdy ;
    EXPORT PendSV_Handler



NVIC_INT_CTRL    EQU      0xE000ED04
NVIC_SYSPRI14    EQU      0xE000ED22
NVIC_PENDSV_PRI  EQU      0xFF
NVIC_PENDSVSET   EQU      0x10000000

    PRESERVE8
    THUMB

    AREA CODE,CODE,READONLY

;*******************************************************************
;                        开始第一次上下文切换
; 1、配置PendSV异常的优先级为最低
; 2、在开始第一次上下文切换之前，设置psp=0
; 3、触发PendSV异常，开始上下文切换
;*******************************************************************

OSStartHighRdy
    LDR R0,=NVIC_SYSPRI14               ; 设置pendsv异常优先级为最低
    LDR R1,=NVIC_PENDSV_PRI
    STRB R1,[R0]

    MOVS R0,#0                          ;设置psp的值为0，开始第一次上下文切换
    MSR  PSP,R0

    LDR   R0,=NVIC_INT_CTRL              ;触发pendsv异常
    LDR   R1,=NVIC_PENDSVSET             ; write 1 to bit28 means trig pendsv 
    STR   R1,[R0]

    CPSIE   I    ; 启用总中断，NMI和HardFault除外

OSStartHang
    B    OSStartHang                ;程序永远不会到这里


;*******************************************************************
;                        pendsvHandler 异常
;*******************************************************************
PendSV_Handler
    CPSID   I

    MRS     R0,PSP    ;将process stack pointer 加载到R0

    CBZ     R0, OS_CPU_PendSVHandler_nosave

    ;保存上下文
    ;任务切换，把下一个要运行的任务的栈内容加载到cpu寄存器中
    ;进入pendv异常的时候，当前cpu xPSR，PC
    ;R14,R12,R3,R2,R1，R0会自动存储到当前任务栈
    ;同时递减PSP，顺便通过代码MRS R0,PSP
    ;手动存储CPU寄存器R4-R11的值到当前任务的栈

    STMDB R0!,{R4-R11}

    ;加载OSTCBCurPtr 的指针地址到R1
    LDR R1,=OSTCBCurPtr
    ;加载OSTCBCurPtr的指针到R1
    LDR R1,[R1]
    ;存储R0的值到OSTCBCurPtr->OSTCBStkPtr，这个时候R0存的是任务空闲栈的栈顶
    STR R0,[R1]

    ;切换下文
    ;实现OSTCBCurPtr = OSTCBHighRdyPtr
    ;把下一个要运行的任务栈内容加载到CPU寄存器中
    ;

OS_CPU_PendSVHandler_nosave
    
    ;加载OSTCBCurPtr 的指针地址到R0，这里LDR属于伪指令
    LDR R0,=OSTCBCurPtr       ;(5)

    LDR R1,=OSTCBHighRdyPtr   ;(6) 加载OSTCBHighRdyPtr 指针的地址到R1

    LDR R2,[R1]     ; (7)加载OSTCBHighRdyPtr指针到R2

    STR R2,[R0]      ; (8)存储OSTCBHighRdyPtr 到 OSTCBCurPtr

    LDR R0,[R2]   ; (9)加载OSTCBHighRdyPtr 到R0.TCB第一个成员是栈指针StkPtr.所以R0=StkPtr。后续操作都是通过R0

    LDMIA R0!,{R4-R11} ;(10)

    MSR PSP,R0

    ORR LR,LR,#0x04

    CPSIE I

    BX LR
    NOP
    END

