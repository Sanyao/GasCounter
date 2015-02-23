/* initialization procedure file header */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_INIT_H
#define __GCOUNTER_INIT_H

#include "Counter_GPIO_define.h"
#include "Counter_GPIO_utils.h"
#include "Counter_TimeMgmt.h"
#include "Counter_Misc.h"
#include "stm32l1xx_rcc.h"
#include "Counter_UART.h"


void CNT_Init(void);


#endif /* __GCOUNTER_INIT_H*/