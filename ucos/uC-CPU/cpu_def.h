#ifndef  CPU_DEF_MODULE_PRESENT
#define  CPU_DEF_MODULE_PRESENT

/* ---------------------- CPU WORD SIZE ----------------------- */
#define  CPU_WORD_SIZE_08                          1u   /*  8-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_16                          2u   /* 16-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_32                          4u   /* 32-bit word size (in octets).                                */
#define  CPU_WORD_SIZE_64                          8u   /* 64-bit word size (in octets).                                */


#define     CPU_CRITICAL_METHOD_NONE          0
#define     CPU_CRITICAL_METHOD_INT_DIS_EN    1
#define     CPU_CRITICAL_METHOD_STATUS_STK    2
#define     CPU_CRITICAL_METHOD_STATUS_LOCAL  3

#endif /* CPU_DEF_MODULE_PRESENT */


