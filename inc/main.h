/* main header file
  includes and common use parameters in this
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_MAIN_H
#define __GCOUNTER_MAIN_H


#include "stm32l1xx.h"
#include "stm32l1xx_conf.h" //modified - commented unused includes
#include "Counter_GPIO_define.h"
#include "Counter_Init.h"
#include "Counter_GPIO_utils.h"
#include "Counter_PwrMgmt.h"
#include "Counter_Measurement.h"
#include "stm32l1xx_syscfg.h"
#include "Counter_Misc.h"
#include "Counter_GSM.h"
#include "Counter_UART.h"
#include "stm32l1xx_dbgmcu.h"
#include "Counter_Memory.h"

#define DUMMYPULSECOUNTDELAY 50000
#define DUMMYALARMCOUNTDELAY 65000


/* define border params */
#define VOLTIONSTRMIN 2200 // minimal voltage on ionistor to success GSM session  
#define VOLTIONSTRGIST 100 // Гистерезис для зарядки 


extern unsigned char at_out[MAX_AT_SIZE];
extern unsigned char at_in[MAX_AT_SIZE];




/* define main Procedures */

void EXTI20_IRQHandler(void); // wakeup intr hanhler
void EXTI0_IRQHandler(void); // Pulse Count handler - Ch1 - PC0
void EXTI1_IRQHandler(void); // Pulse Count handler CH2 - PC1
void EXTI2_IRQHandler(void); // Pulse Count handler CH3 - PC2

void EXTI4_IRQHandler(void); // Alarm handler CH1 - PC4
void EXTI5_IRQHandler(void); // Alarm handler CH2 - PC5



#endif /* __GCOUNTER_MAIN_H */