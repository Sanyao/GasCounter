/* initialization procedure file header */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_PWRMGMT_H
#define __GCOUNTER_PWRMGMT_H

#include "Counter_Misc.h"
#include "main.h"
#include "Counter_GPIO_define.h"
#include "Counter_GPIO_utils.h"

uint8_t CNT_PWR_BOOSTER(uint8_t STATE); // on or off booster for gsm
uint8_t CNT_PWR_ChgIonstr(void); // charge ionistor from LiBat


#define DUMMYWAITFORBOOSTERON 1000000

#define MAXCHARGECYCLES 150  // макс. количество циклов подзаряда ионистора
#define CHARGECYCLESWIDTH 10000000 // длительность одного цикла заряда
#define COOLINGCYCLESWIDTH 100000 // длительность одного цикла заряда




#endif /* __GCOUNTER_PWRMGMT_H*/