#include "os_cfg_app.h"
#include "os.h"


CPU_STK		OSCfg_IdleTaskStk[OS_CFG_IDLE_TASK_STK_SIZE];


CPU_STK *const OSCfg_IdleTaskStkBasePtr = (CPU_STK *)&OSCfg_IdleTaskStk[0];
CPU_STK_SIZE  const OSCfg_IdleTaskStkSize = (CPU_STK_SIZE)OS_CFG_IDLE_TASK_STK_SIZE;