/* initialization procedure file header */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_GPIOUTILS_H
#define __GCOUNTER_GPIOUTILS_H

#include "Counter_GPIO_define.h"
#include "Counter_Misc.h"
#include "Counter_Measurement.h"


extern uint16_t channel_shorted; // закороченные каналы
extern uint16_t channel_opened; // обрыв каналов


void  First_Init_GPIOs (void); 

void ADC_Measurement_Configure_GPIO(void);
void  CNT_SetPulseWorkState (void);
void  CNT_SetAlarmWorkState (void);
void  CNT_GPIO_GSM_Lines(uint8_t state);  // ON-OFF all gsm module lines

//Setting line
void CNT_GPIO_SetPinMode(GPIO_TypeDef* CNTPORT, uint32_t CNTPIN, GPIOMode_TypeDef INOUTDIR, GPIOOType_TypeDef PUSHPULLMODE, GPIOPuPd_TypeDef PULLED);

void  CNT_GPIO_PrepToStop(void); // prepare GPIO for enter to stop mode
void  CNT_GPIO_RetFromStop(void); // return GPIO after stop mode


void CNT_GPIO_CheckChannel(void); // check channel circuit



#endif /* __GCOUNTER_GPIOUTILS_H*/