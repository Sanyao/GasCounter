 /* initialization procedure file header */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_TIMEMGMT_H
#define __GCOUNTER_TIMEMGMT_H

#include "stm32l1xx_rtc.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_exti.h"
#include "Counter_Misc.h"
#include "main.h"
#include "Counter_UART.h"

// time constant definitions
#define         CZEROHOUR       14
#define         CZEROMIN        15
#define         CZEROSEC        16
#define         CZERODAY        06
#define         CZEROMONTH      RTC_Month_February
#define         CZEROYEAR       15
#define         CZEROWEEKDAY    RTC_Weekday_Friday
#define         CZEROWKUP       3600  // seconds



extern uint8_t hours, minutes, seconds;  // from main
extern uint8_t weekday, day, month, year;
extern unsigned char at_out[MAX_AT_SIZE], at_in[MAX_AT_SIZE];

void CNT_First_Set_RTC (void); 
ErrorStatus CNT_Set_RTC_Time(uint8_t hours, uint8_t minutes, uint8_t seconds);
ErrorStatus CNT_Set_RTC_Date(uint8_t weekday, uint8_t day, uint8_t month, uint8_t year);
ErrorStatus CNT_RTC_SetWakeUpPeriod(uint32_t WakeUpPeriodSeconds);
void CNT_RTC_ClearWakeupFlags(void); // clear flags after intr
void CNT_Get_RTC_Time(void); // to global vars
void CNT_Get_RTC_Date(void);


// делать
void CNT_TIME_SetTimeFromSMSDeliveryReport(void); // Set time from SMS Delivery Report



// не сделано пока
void CNT_Time_LPSLEEPms(uint32_t ms); // low power sleep сколько то миллисекунд - на таймере





#endif /* __GCOUNTER_TIMEMGMT_H*/