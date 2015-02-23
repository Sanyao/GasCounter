#include "main.h"

uint16_t needTX = OFF; // on or off need tx flag - regular TX
uint16_t needTX_Alarm = OFF; // alarm tx needed flag

unsigned char *s;
volatile unsigned char* sms = "default SMS. maxlength = 64 symbols. rezerve area for this mess.";
/* define global variables */
uint32_t Pulse1Overall;
uint32_t Pulse2Overall;
uint32_t Pulse3Overall;
uint32_t Pulse1Current;
uint32_t Pulse2Current;
uint32_t Pulse3Current;

uint32_t Alarms1Count =0;
uint32_t Alarms2Count =0;

// global voltages variables
uint16_t Volt_LIBAT;
uint16_t Volt_SOBAT;
uint16_t Volt_INSTR;
uint16_t Volt_GSMMO;
uint16_t Volt_REFIN;
uint16_t Volt_TSENS;
int16_t  TemperatureInternal;
uint16_t Volt_VCC;

uint8_t hours, minutes, seconds;  // from main
uint8_t weekday, day, month, year;

float koeffRAIN = 1.64; // учет входного сопротивдения АЦП - костыль для измерения напряжения 
                      // примерно столько. будем потом корректировать по показаниям напряжения питания sim900 и Volt_GSMMO


int main()
{
  int i, j;
  uint16_t Volts;
      /* first delay*/
    __disable_irq();
  SYSCFG_DeInit();                      // reset congiguration
  CNT_Init();                           // first setup for counter systems
  NVIC_EnableIRQ(RTC_WKUP_IRQn);        // enable rtc wkup interrupt
  NVIC_EnableIRQ(USART1_IRQn);
  __enable_irq();                       // global enable interrupts
     needTX=ON;    
    CNT_DummyDelay(5000000);  //for sleep unchange ^)
     
  CNT_GetVoltages(); // debug
     
  while (1)
  {
   
    if (needTX==ON)
    { CNT_GPIO_RetFromStop();
      CNT_GetVoltages();
      if (Volt_INSTR > VOLTIONSTRMIN)
      {      if (CNT_GSM_Module_ON()==ON) 
              { //CNT_GetVoltages();
                CNT_GSM_SendDefaultSMS();
                CNT_GSM_Module_OFF();
                needTX=OFF;
              }
      }      
    }  
    
    CNT_GPIO_PrepToStop();
    PWR_UltraLowPowerCmd(ENABLE);                       // go to stop mode
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_SLEEPEntry_WFI);
 
 }
  
  //return 0;
}


void RTC_WKUP_IRQHandler(void) // wakeup intr hanhler
{
   CNT_RTC_ClearWakeupFlags();
   //GPIO_HIGH(LED_PORT,LED2_PIN);
   CNT_GetVoltages();
   needTX = ON ;
         
   //GPIO_LOW(LED_PORT,LED2_PIN);
   EXTI_ClearFlag(EXTI_Line20);

} // end interrupt


void EXTI0_IRQHandler(void) // Pulse Count handler - Ch1 - PC0
{
  GPIO_HIGH(LED_PORT,LED1_PIN);
  NVIC_DisableIRQ(EXTI0_IRQn);
  Pulse1Overall +=1;
  CNT_DummyDelay(DUMMYPULSECOUNTDELAY);
  EXTI_ClearITPendingBit(EXTI_Line0);
  EXTI_ClearFlag(EXTI_Line0);  
  NVIC_EnableIRQ(EXTI0_IRQn);
  GPIO_LOW(LED_PORT,LED1_PIN);
}

void EXTI1_IRQHandler(void) // Pulse Count handler CH2 - PC1
{
  GPIO_HIGH(LED_PORT,LED1_PIN);
  NVIC_DisableIRQ(EXTI1_IRQn);
  Pulse2Overall +=1;
  CNT_DummyDelay(DUMMYPULSECOUNTDELAY);
  EXTI_ClearITPendingBit(EXTI_Line1);
  EXTI_ClearFlag(EXTI_Line1);
  NVIC_EnableIRQ(EXTI1_IRQn);
  GPIO_LOW(LED_PORT,LED1_PIN);
}

void EXTI2_IRQHandler(void) // Pulse Count handler CH3 - PC2
{
  GPIO_HIGH(LED_PORT,LED1_PIN);
  NVIC_DisableIRQ(EXTI2_IRQn);
  Pulse3Overall +=1;
  CNT_DummyDelay(DUMMYPULSECOUNTDELAY);
  EXTI_ClearITPendingBit(EXTI_Line2);
  EXTI_ClearFlag(EXTI_Line2);
  NVIC_EnableIRQ(EXTI2_IRQn);
  GPIO_LOW(LED_PORT,LED1_PIN);
}


void EXTI4_IRQHandler(void) // Alarm handler CH1 - PC4
{
  uint32_t i;
  NVIC_DisableIRQ(EXTI4_IRQn);
  for (i=0; i<10; i++) { // :)
                        GPIO_TOGGLE(LED_PORT,LED1_PIN);
                        CNT_DummyDelay(DUMMYALARMCOUNTDELAY);
                        CNT_DummyDelay(DUMMYALARMCOUNTDELAY);
                        }
  Alarms1Count +=1;
  needTX_Alarm = ON;
  EXTI_ClearITPendingBit(EXTI_Line4);
  EXTI_ClearFlag(EXTI_Line4);
  NVIC_EnableIRQ(EXTI4_IRQn);
  //GPIO_LOW(LED_PORT,LED1_PIN);
}


void EXTI9_5_IRQHandler(void) // Alarm handler CH2 - PC5
{
  uint32_t i;
  NVIC_DisableIRQ(EXTI9_5_IRQn);
  for (i=0; i<10; i++) { // :)
                        GPIO_TOGGLE(LED_PORT,LED1_PIN);
                        CNT_DummyDelay(DUMMYALARMCOUNTDELAY);
                        CNT_DummyDelay(DUMMYALARMCOUNTDELAY);
                        }
  Alarms2Count +=1;
  needTX_Alarm = ON;
  EXTI_ClearITPendingBit(EXTI_Line5);
  EXTI_ClearFlag(EXTI_Line5);
  NVIC_EnableIRQ(EXTI9_5_IRQn);
  //GPIO_LOW(LED_PORT,LED1_PIN);
}



