
#include "Counter_GPIO_utils.h"



void  First_Init_GPIOs (void)
{
  /* GPIO, EXTI and NVIC Init structure declaration */
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* GPIO Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);


/* Configure the LED_pin as output push-pull for LD3 & LD4 usage*/
  GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(LED_PORT, &GPIO_InitStructure);
  
  /* Force a low level on LEDs*/ 
  // for debug
  GPIO_LOW(LED_PORT,LED1_PIN);	
  GPIO_LOW(LED_PORT,LED2_PIN);

      
  /* Configure initial Battery state */
      // output part
      GPIO_InitStructure.GPIO_Pin = SBEN_PIN | LBEN_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // !!! inverted pins
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_Init(BATTM_PORT, &GPIO_InitStructure);
      
      // Charge source select  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      GPIO_LOW(BATTM_PORT,SBEN_PIN);	// Enable Solar
      GPIO_LOW(BATTM_PORT,LBEN_PIN);   // & Enable Lithium for first start
      
      
    // GSM Booster control line
      GPIO_InitStructure.GPIO_Pin = SBEN_PIN | LBEN_PIN | BOOSTEN_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_Init(BATTM_PORT, &GPIO_InitStructure);
      GPIO_LOW(BATTM_PORT,BOOSTEN_PIN); // Disable GSM Booster
 
      
  /* configure adc input pin and adc signal commutation ctrl pins */ 
      // Output Part  
      GPIO_InitStructure.GPIO_Pin = EN1_PIN | EN2_PIN | EN3_PIN | EN4_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_Init(ADC_SW_PORT, &GPIO_InitStructure);
    
      GPIO_LOW(ADC_SW_PORT,EN4_PIN);	// Push LOW
      GPIO_LOW(ADC_SW_PORT,EN3_PIN);	// Push LOW
      GPIO_LOW(ADC_SW_PORT,EN2_PIN);	// Push LOW
      GPIO_LOW(ADC_SW_PORT,EN1_PIN);	// Push LOW
      
      // ADC input
      GPIO_InitStructure.GPIO_Pin = ADC_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; /* GPIO Analog Mode */ 
      GPIO_Init(ADC_PORT, &GPIO_InitStructure);
      
      // unused H port 
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; /* GPIO Analog Mode */ 
      GPIO_Init(GPIOH, &GPIO_InitStructure);   
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, DISABLE);
}  


/*  set pulse count configuration
    CMN PushDown
    PulseCount lines Pullup with internal resistor
    Interrupt generation when PulseCount line go to High or low
*/
void  CNT_SetPulseWorkState (void) 
{
  /* GPIO, EXTI and NVIC Init structure declaration */
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  __disable_irq();
    /* Configure default state for pulse counter */
    // Pulse input part:
      GPIO_InitStructure.GPIO_Pin = PLS1_PIN | PLS2_PIN | PLS3_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // input mode
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // Pull UP
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz; // very low speed
      GPIO_Init(EXT_PORT, &GPIO_InitStructure);
     // Common  Part  
      GPIO_InitStructure.GPIO_Pin = PLSCMN_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_Init(EXT_PORT, &GPIO_InitStructure);
    
      GPIO_LOW(EXT_PORT,PLSCMN_PIN);	// Push LOW
   
      // interrupt setup for pulse lines - EXTI 0,1,2 line is used for pulse counts */
      RCC_AHBPeriphClockLPModeCmd(RCC_AHBPeriph_GPIOC, ENABLE); // enable sleep clock for port C
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // enable system clock for intr
      RCC_APB2PeriphClockLPModeCmd(RCC_APB2Periph_SYSCFG, ENABLE);
        
      SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource0);
      SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
      SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);
      
      EXTI_InitStructure.EXTI_Mode =  EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // Raising and falling trigger
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_InitStructure.EXTI_Line = EXTI_Line0;
      EXTI_Init(&EXTI_InitStructure);
      
      EXTI_InitStructure.EXTI_Mode =  EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // Raising and falling trigger
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_InitStructure.EXTI_Line = EXTI_Line1;
      EXTI_Init(&EXTI_InitStructure);      
      
      EXTI_InitStructure.EXTI_Mode =  EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // Raising and falling trigger
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_InitStructure.EXTI_Line = EXTI_Line2;
      EXTI_Init(&EXTI_InitStructure);      
      
      NVIC_DisableIRQ(EXTI0_IRQn);
            NVIC_DisableIRQ(EXTI1_IRQn);
                  NVIC_DisableIRQ(EXTI2_IRQn);
               
      // Interrupt Clear-Pending Register - clear all pending interrupts            
     //NVIC_ICPR_CLRPEND = 0xFF;             
                  
                  
 __enable_irq();                       // global enable interrupts   
      EXTI_ClearITPendingBit(EXTI_Line0);
      EXTI_ClearFlag(EXTI_Line0);  
      NVIC_EnableIRQ(EXTI0_IRQn); //разрешение прерывания EXTI0
      //NVIC_SetPriority(EXTI0_IRQn, 1); //задаем приоритет прерывания
      
      EXTI_ClearITPendingBit(EXTI_Line1);
      EXTI_ClearFlag(EXTI_Line1);  
      NVIC_EnableIRQ(EXTI1_IRQn); //разрешение прерывания EXTI1
      //NVIC_SetPriority(EXTI1_IRQn, 1); //задаем приоритет прерывания  
        
      EXTI_ClearITPendingBit(EXTI_Line2);
      EXTI_ClearFlag(EXTI_Line2);  
      NVIC_EnableIRQ(EXTI2_IRQn); //разрешение прерывания EXTI2
      //NVIC_SetPriority(EXTI2_IRQn, 1); //задаем приоритет прерывания
      
      
      EXTI_ClearITPendingBit(EXTI_Line0);  EXTI_ClearFlag(EXTI_Line0);       
      EXTI_ClearITPendingBit(EXTI_Line1);  EXTI_ClearFlag(EXTI_Line1);  
      EXTI_ClearITPendingBit(EXTI_Line2);  EXTI_ClearFlag(EXTI_Line2);  
      

}

void  CNT_SetAlarmWorkState (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  __disable_irq();
    /* Configure default state for pulse counter */
    // input part:
      GPIO_InitStructure.GPIO_Pin = AL1_PIN | AL2_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // input mode
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // Pull UP
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz; // very low speed
      GPIO_Init(EXT_PORT, &GPIO_InitStructure);
     // Output Part  
      GPIO_InitStructure.GPIO_Pin = ALCMN_PIN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
      GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
      GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
      GPIO_Init(EXT_PORT, &GPIO_InitStructure);
    
      GPIO_LOW(EXT_PORT,ALCMN_PIN); //alarm common node to low
  
      /* interrupt config */
      /* alarm generation on any edge of signal - connect or unconnect
         Может быть в дальнейшем сделать проверку алармов импульсно - без генерации прерываний
         Этим можно снизить энергопотребление, но снизит оперативность сообщения о событии*/
      SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4); // alarm on channel 1 - default PC4
      SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5); // alarm on channel 2 - default PC5

      EXTI_InitStructure.EXTI_Mode =  EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // Raising and falling trigger
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_InitStructure.EXTI_Line = EXTI_Line4;
      EXTI_Init(&EXTI_InitStructure);
      
      EXTI_InitStructure.EXTI_Mode =  EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // Raising and falling trigger
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_InitStructure.EXTI_Line = EXTI_Line5;
      EXTI_Init(&EXTI_InitStructure);      
        
      EXTI_ClearITPendingBit(EXTI_Line4);
      EXTI_ClearFlag(EXTI_Line4);  
      NVIC_EnableIRQ(EXTI4_IRQn); //разрешение прерывания EXTI4
      NVIC_SetPriority(EXTI4_IRQn, 17); //задаем приоритет прерывания
            
      EXTI_ClearITPendingBit(EXTI_Line5);
      EXTI_ClearFlag(EXTI_Line5);  
      NVIC_EnableIRQ(EXTI9_5_IRQn); //разрешение прерывания EXTI5
      NVIC_SetPriority(EXTI9_5_IRQn, 17); //задаем приоритет прерывания
        __enable_irq();                       // global enable interrupts
 }








void CNT_GPIO_GSM_Lines(uint8_t state)  // ON-OFF all gsm module interface lines
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  if (state==ON) // enable digital lines and USART
  {
          /* Configure default state for GSM Module */
          // !!! This pins INVerted !!! 
          GPIO_InitStructure.GPIO_Pin = NRST_PIN | PWRKEY_PIN;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
          GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // !!! inverted pins
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
          GPIO_Init(SIM_PORT, &GPIO_InitStructure);
      
          GPIO_LOW(SIM_PORT,NRST_PIN);	// Push LOW -
          GPIO_LOW(SIM_PORT,PWRKEY_PIN);    // GSM off
  
          GPIO_InitStructure.GPIO_Pin = GSM_STATUS_PIN;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
          //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // pull down status pin
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
          GPIO_Init(SIM_STATUS_PORT, &GPIO_InitStructure);
          
   }

  
   if (state==OFF) // disable digital lines and USART for reduce power consumption
   {
                /* Configure OFF state for GSM Module */
          GPIO_InitStructure.GPIO_Pin = NRST_PIN | PWRKEY_PIN;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
          //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // !!! No Pull
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
          GPIO_Init(SIM_PORT, &GPIO_InitStructure);
  
          GPIO_InitStructure.GPIO_Pin = GSM_STATUS_PIN;
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
          //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
          GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // pull down status pin
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz;
          GPIO_Init(SIM_STATUS_PORT, &GPIO_InitStructure);
          
    }
  
}  



void CNT_GPIO_SetPinMode(GPIO_TypeDef* CNTPORT, uint32_t CNTPIN, GPIOMode_TypeDef INOUTDIR, GPIOOType_TypeDef PUSHPULLMODE, GPIOPuPd_TypeDef PULLED)
{
    // setup line parameters
    GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin = CNTPIN;
      GPIO_InitStructure.GPIO_Mode = INOUTDIR; // input mode
      GPIO_InitStructure.GPIO_OType = PUSHPULLMODE;
      GPIO_InitStructure.GPIO_PuPd = PULLED; // Pull UP
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_400KHz; // very low speed
      GPIO_Init(CNTPORT, &GPIO_InitStructure);
  
}


void  CNT_GPIO_PrepToStop(void) // prepare GPIO for enter to stop mode
{ // set analog in mode for not used in stop mode pins
   GPIO_InitTypeDef GPIO_InitStructure;
   
  CNT_GPIO_GSM_Lines(OFF);

    // off unused lines
  CNT_GPIO_SetPinMode(GPIOA, 
                      GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2, 
                      GPIO_Mode_AN, 
                      GPIO_OType_PP, GPIO_PuPd_NOPULL);  
  
  CNT_GPIO_SetPinMode(GPIOB, 
                      GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, 
                      GPIO_Mode_AN, 
                      GPIO_OType_PP, GPIO_PuPd_NOPULL);  
  
  CNT_GPIO_SetPinMode(GPIOC, 
                      GPIO_Pin_12 | GPIO_Pin_13, 
                      GPIO_Mode_AN, 
                      GPIO_OType_PP, GPIO_PuPd_NOPULL);  
  
  
  // disable clock - automatically on stop mode?
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, DISABLE);
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, DISABLE);
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, DISABLE);
  
  // off control lines

  
  
  
}


void  CNT_GPIO_RetFromStop(void) // return GPIO after stop mode
{
  
  GPIO_InitTypeDef GPIO_InitStructure;
   
   // enable clock 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  // GPIO_TOGGLE(LED_PORT,LED1_PIN);	
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOH, DISABLE); // not used in normal - always off
  
    // on control lines
  
  
}


void CNT_GPIO_CheckChannel(void)   // check channel circuit
{

  uint32_t    chvolt[5] = {0,0,0,0,0};
  uint16_t     chadc[5] = {PLS1_CHAN, PLS2_CHAN,PLS3_CHAN,AL1_CHAN,AL2_CHAN};
  uint16_t     chpin[5] = {PLS1_PIN , PLS2_PIN, PLS3_PIN ,AL1_PIN, AL2_PIN};
  uint32_t i;
  
  // disable IRQ for channels
      EXTI->IMR &= ~EXTI_Line0;
      EXTI->IMR &= ~EXTI_Line1;
      EXTI->IMR &= ~EXTI_Line2;
      EXTI->IMR &= ~EXTI_Line4;
      EXTI->IMR &= ~EXTI_Line5;
    /*
      NVIC_DisableIRQ(EXTI9_5_IRQn); //запрет прерывания alarm2
      NVIC_DisableIRQ(EXTI4_IRQn); //запрет прерывания alarm1       
      NVIC_DisableIRQ(EXTI2_IRQn); //запрет прерывания PC3     
      NVIC_DisableIRQ(EXTI1_IRQn); //запрет прерывания PC2     
      NVIC_DisableIRQ(EXTI0_IRQn); //запрет прерывания PC1    */
      
  // check  channel for open & short circuit
  // 1) discharge channel
  GPIO_LOW(EXT_PORT, PLS1_PIN | PLS2_PIN | PLS3_PIN);
  GPIO_LOW(EXT_PORT, AL1_PIN | AL2_PIN);
  
   // 2) set PLS & alarm input to analog input mode
  CNT_GPIO_SetPinMode(EXT_PORT, PLS1_PIN | PLS2_PIN | PLS3_PIN, 
                      GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_DOWN);  
  CNT_GPIO_SetPinMode(EXT_PORT, AL1_PIN | AL2_PIN, 
                      GPIO_Mode_AN, GPIO_OType_PP, GPIO_PuPd_DOWN);      
  
  // 3) Push common to HIGH
  CNT_GPIO_SetPinMode(EXT_PORT, PLSCMN_PIN, 
                      GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
  GPIO_HIGH(EXT_PORT, PLSCMN_PIN);
  
  CNT_GPIO_SetPinMode(EXT_PORT, ALCMN_PIN, 
                      GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
  GPIO_HIGH(EXT_PORT, ALCMN_PIN);  
  

  // 4) Measure voltage on input pins
  
  CNT_MGMT_ExtLinesVolts( chadc, chvolt); // измерим напряжение на каждой линии внешних подключений
  channel_shorted = 0;
  channel_opened = 0;
  for (i=0; i< sizeof(chadc)/2; i++) // sizeof дает в байтах!! 
    {
      // shorted - delta vith Vcc lower than 300 mV - need calibration
      if (chvolt[i] > Volt_VCC -  300) channel_shorted |= chpin[i]; // set shorted bit
      // opened - Volt on pin lover than 1700 mV - need calibration
      if (chvolt[i] < 1700) channel_opened |= chpin[i]; // set shorted bit
    }
  
   
  CNT_SetPulseWorkState (); 
  CNT_SetAlarmWorkState ();
  
}








 
