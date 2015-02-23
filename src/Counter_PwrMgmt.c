#include "Counter_PwrMgmt.h"


uint8_t CNT_PWR_BOOSTER(uint8_t STATE)
{
  
  if (STATE == ON) 
  {
    CNT_GPIO_SetPinMode(BATTM_PORT, BOOSTEN_PIN, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);
    GPIO_HIGH(BATTM_PORT,BOOSTEN_PIN);
    CNT_DummyDelay(DUMMYWAITFORBOOSTERON); // delay for booster stabilized
  }
  else 
  { GPIO_LOW(BATTM_PORT,BOOSTEN_PIN);
    CNT_GPIO_SetPinMode(BATTM_PORT, BOOSTEN_PIN, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL); // off EN pin
  }
    
  return STATE ; //newState;
}


uint8_t CNT_PWR_ChgIonstr(void) // charge ionistor from LiBat
{
  uint16_t iter;
  
  CNT_GPIO_SetPinMode(BATTM_PORT, LBEN_PIN,    // enable LiEnable output for switch on and off regulator
                      GPIO_Mode_OUT,  GPIO_OType_PP, GPIO_PuPd_NOPULL);
  
  iter = MAXCHARGECYCLES;
  
  while (  (Volt_INSTR < VOLTIONSTRMIN+VOLTIONSTRGIST) && (iter-- > 0) )
  {
    GPIO_HIGH(BATTM_PORT,LBEN_PIN); // connect LiBat to ionistor thru regulator
    GPIO_HIGH(LED_PORT,LED2_PIN); // LED for info
    CNT_DummyDelay(CHARGECYCLESWIDTH); // wait one cycle
    GPIO_LOW(LED_PORT,LED2_PIN); // LED for info
    GPIO_LOW(BATTM_PORT,LBEN_PIN); // disconnect LiBat to ionistor thru regulator
    CNT_DummyDelay(COOLINGCYCLESWIDTH); // wait for cooling
    CNT_GetVoltages();
    
  }
  
    CNT_GPIO_SetPinMode(BATTM_PORT, LBEN_PIN,    // disable LiEnable output for switch on and off regulator
                      GPIO_Mode_AN,  GPIO_OType_PP, GPIO_PuPd_NOPULL);
    
  
    if (Volt_INSTR >= VOLTIONSTRMIN) return ON;
    else return OFF;
}