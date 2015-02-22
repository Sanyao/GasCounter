// GPIO define for counter layout


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_GPIO_H
#define __GCOUNTER_GPIO_H
#include "stm32l1xx.h"

// debug LEDs:
#define LED1_PIN GPIO_Pin_6
#define LED2_PIN GPIO_Pin_7
#define LED_PORT GPIOB

// GPIO on-off-toggle
#define GPIO_HIGH(a,b) 		a->BSRRL = b
#define GPIO_LOW(a,b)		a->BSRRH = b
#define GPIO_TOGGLE(a,b) 	a->ODR ^= b 


// ADC Signal Commutations and inputs
#define ADC_SW_PORT     GPIOC
#define EN1_PIN         GPIO_Pin_7
#define EN2_PIN         GPIO_Pin_8
#define EN3_PIN         GPIO_Pin_9
#define EN4_PIN         GPIO_Pin_10

#define ADC_PORT        GPIOB
#define ADC_PIN         GPIO_Pin_1
#define ADC_CH_NUM      ADC_Channel_9



//Pulse counter and extrernal inputs
#define EXT_PORT           GPIOC
#define PLS1_PIN        GPIO_Pin_0
#define PLS2_PIN        GPIO_Pin_1
#define PLS3_PIN        GPIO_Pin_2
#define PLSCMN_PIN      GPIO_Pin_3
#define AL1_PIN         GPIO_Pin_4
#define AL2_PIN         GPIO_Pin_5
#define ALCMN_PIN       GPIO_Pin_6
#define PLS1_CHAN       ADC_Channel_10
#define PLS2_CHAN       ADC_Channel_11
#define PLS3_CHAN       ADC_Channel_12
#define AL1_CHAN        ADC_Channel_14
#define AL2_CHAN        ADC_Channel_15


// SIM signal define
#define SIM_PORT        GPIOB
#define NRST_PIN        GPIO_Pin_15
#define PWRKEY_PIN      GPIO_Pin_5

#define SIM_STATUS_PORT GPIOC
#define GSM_STATUS_PIN  GPIO_Pin_11 

// Battery management define
#define BATTM_PORT      GPIOA
#define SBEN_PIN        GPIO_Pin_4
#define LBEN_PIN        GPIO_Pin_5
#define SRCSTAT_PIN     GPIO_Pin_6
#define BATSTAT_PIN     GPIO_Pin_7 // not used ?
#define BOOSTEN_PIN     GPIO_Pin_3      // GSM power booster enable


// EEPROM and Flash definitions






#endif /* __GCOUNTER_GPIO_H*/