
#include "Counter_init.h"


void CNT_Init(void)
{// first initialisation after power up
    RCC_DeInit();
    RCC_MSIRangeConfig(RCC_MSIRange_5); // 2 MHz clock
    RCC_MSICmd(ENABLE);
    while (!RCC_GetFlagStatus(RCC_FLAG_MSIRDY) ) {;} // wait MSI clock
    RCC_SYSCLKConfig(RCC_SYSCLKSource_MSI);
      RCC_HSICmd(DISABLE);
  
  CNT_First_Set_RTC (); // from 
  First_Init_GPIOs (); // from Counter GPIO utils - first port states
  CNT_SetPulseWorkState (); 
  CNT_SetAlarmWorkState ();
  CNT_UART_init();  
}

