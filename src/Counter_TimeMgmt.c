#include "Counter_TimeMgmt.h"
#include <stdlib.h>     /* atoi */
 
RTC_InitTypeDef rtc;
RTC_TimeTypeDef time;
RTC_DateTypeDef date;
RTC_TimeTypeDef alarmTime;
RTC_AlarmTypeDef alarm;
EXTI_InitTypeDef exti;
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef exticonf;
//EXTIMode_TypeDef extimode;
//EXTITrigger_TypeDef extitrig;

void CNT_First_Set_RTC (void)
{

    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_RTCAccessCmd(ENABLE);
    //RCC_BackupResetCmd(ENABLE);
    //RCC_BackupResetCmd(DISABLE);
 
    RCC_LSICmd(DISABLE);
    RCC_LSEConfig(RCC_LSE_ON);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);
    while (!RCC_GetFlagStatus(RCC_FLAG_LSERDY)) {}
      
    rtc.RTC_HourFormat = RTC_HourFormat_24;
    rtc.RTC_SynchPrediv = 0xFF;
    rtc.RTC_AsynchPrediv = 0x7F;
    RTC_Init(&rtc);
        
    CNT_Set_RTC_Time(CZEROHOUR, CZEROMIN, CZEROSEC);
    CNT_Set_RTC_Date(CZEROWEEKDAY, CZERODAY, CZEROMONTH, CZEROYEAR);
    CNT_RTC_SetWakeUpPeriod(CZEROWKUP); 
    
    
    
}

ErrorStatus CNT_Set_RTC_Time ( uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    time.RTC_H12 = RTC_HourFormat_24;
    time.RTC_Hours = hours;
    time.RTC_Minutes = minutes;
    time.RTC_Seconds = seconds;
    return RTC_SetTime(RTC_Format_BIN, &time);  // init RTC time
}


ErrorStatus CNT_Set_RTC_Date(uint8_t weekday, uint8_t day, uint8_t month, uint8_t year)
{
  date.RTC_WeekDay      = weekday;
  date.RTC_Date         = day;
  date.RTC_Month        = month;
  date.RTC_Year         = year;
  return RTC_SetDate(RTC_Format_BIN, &date);
}


void CNT_Get_RTC_Date(void)
{
  
  RTC_GetDate(RTC_Format_BIN, &date);
 
  weekday = date.RTC_WeekDay; //      = weekday;
  day = date.RTC_Date; //         = day;
  month = date.RTC_Month;//        = month;
  year = date.RTC_Year;//         = year;
  return;
}

void CNT_Get_RTC_Time ()
{
    RTC_GetTime(RTC_Format_BIN, &time);  // init RTC time
  
    hours= time.RTC_Hours ;
    minutes= time.RTC_Minutes  ;
    seconds = time.RTC_Seconds  ;
    return ;  // init RTC time
}


ErrorStatus CNT_RTC_SetWakeUpPeriod(uint32_t WakeUpPeriodSeconds)
{
  RTC_ITConfig(RTC_IT_WUT, DISABLE);
  RTC_WakeUpCmd(DISABLE);
  RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
  RTC_SetWakeUpCounter(WakeUpPeriodSeconds);
  RTC_ClearFlag(RTC_FLAG_WUTF);
  RTC_ClearITPendingBit(RTC_IT_WUT);
  
  exticonf.EXTI_Line = EXTI_Line20;
  exticonf.EXTI_Mode = EXTI_Mode_Interrupt;
  exticonf.EXTI_Trigger = EXTI_Trigger_Rising;
  exticonf.EXTI_LineCmd = ENABLE;
  EXTI_Init(&exticonf); 
   
  RTC_ITConfig(RTC_IT_WUT, ENABLE);
  return RTC_WakeUpCmd(ENABLE);
}

void CNT_RTC_ClearWakeupFlags(void)
{
  RTC_ClearFlag(RTC_FLAG_WUTF);
  RTC_ClearITPendingBit(RTC_IT_WUT);
}




// не сделано пока... 
void CNT_Time_LPSLEEPms(uint32_t ms) // спим сколько то миллисекунд
{  // запускаем таймер и ждем прерывания. В прерывании запрещаем прерывание и останавливаем таймер
    
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
  
    TIM_TimeBaseInitTypeDef TIM_InitStructure;  // Заводим структуру
    TIM_TimeBaseStructInit(&TIM_InitStructure); // Инициализация структуры
    TIM_InitStructure.TIM_Prescaler = 32;    // Предделитель ms 32768 / 1000
    TIM_InitStructure.TIM_Period = ms;        // Период таймера - из входного параметра
    TIM_TimeBaseInit(TIM9, &TIM_InitStructure); // Функция настройки таймера
    
    TIM_RemapConfig( TIM9, TIM9_LSE); // connect to LSE 
    
    
    //NVIC_EnableIRQ(TIM6_DAC_IRQn);  
    TIM_ITConfig(TIM6, TIM_DIER_UIE, ENABLE);
   
    
    TIM_Cmd(TIM9, ENABLE);                      // Включение таймера
  
  
  
}


void CNT_TIME_SetTimeFromSMSDeliveryReport(void) // Set time from SMS Delivery Report
{
    
    unsigned char *point;
    unsigned char tempsymb[5]; // temporary symbol for data &time
    //unsigned char *tpoint;
    uint8_t p;
    
    point = &at_in[0];
    //tpoint = &tempsymb[0];
    p=0;
  
    // разбираем пока так... 
    
    // ищем пятую кавычку - дата доставки
    while (*point++ != '"'){;} // первая
    while (*point++ != '"'){;} // вторая
    while (*point++ != '"'){;} // третья
    while (*point++ != '"'){;} // кавычка
    while (*point++ != '"'){;} // еще одна
    //далее год - мес - дата
    while (*point != '/'){tempsymb[p++] = (uint8_t)*point++;} // год
    tempsymb[p] = 0x00;
    date.RTC_Year = atoi( (char const*)&tempsymb[0]); // = год;
    p=0; *point++;
    while (*point != '/'){tempsymb[p++] = (uint8_t)*point++;} // месяц
    tempsymb[p] = 0x00;
    date.RTC_Month = atoi( (char const*)&tempsymb[0]); //   = month;
    p=0; *point++;
    while (*point != ','){tempsymb[p++] = (uint8_t)*point++;} // день
    tempsymb[p] = 0x00;
    date.RTC_Date = atoi( (char const*)&tempsymb[0]); //
    p=0; *point++;
    
    while (*point != ':'){tempsymb[p++] = (uint8_t)*point++;} // часы 
    tempsymb[p] = 0x00; 
    time.RTC_Hours = atoi( (char const*)&tempsymb[0]); // 
    p=0; *point++;
    
    while (*point != ':'){tempsymb[p++] = (uint8_t)*point++;} // минуты 
    tempsymb[p] = 0x00; 
    time.RTC_Minutes = atoi( (char const*)&tempsymb[0]); // 
    p=0; *point++;
    
    while (*point != '+'){tempsymb[p++] = (uint8_t)*point++;} // секунды 
    tempsymb[p] = 0x00; 
    time.RTC_Seconds = atoi( (char const*)&tempsymb[0]); // 
    p=0; *point++;
    
    RTC_SetTime(RTC_Format_BIN, &time);  // init RTC time
    RTC_SetDate(RTC_Format_BIN, &date);
}





