/* initialization procedure file header */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_MEASURE_H
#define __GCOUNTER_MEASURE_H

#include "Counter_GPIO_define.h"
#include "Counter_Misc.h"

//#include "main.h" // for global vars

/* Theorically BandGAP 1.224volt */
#define VREFINT 1.224
//#define VREF 		1.224L

/*
	ADC Converter 
	LSBIdeal = VREF/4096 or VDA/4096
*/
#define ADC_CONV 	4096

/*
 VDD Factory for VREFINT measurement 
*/
#define VDD_FACTORY 3.0L

typedef struct
{
    uint16_t VREF;
    uint16_t TS_CAL_1; // low temperature calibration data
    uint16_t reserved;
    uint16_t TS_CAL_2; // high temperature calibration data
} TSCALIB_TypeDef;

#define FACTORY_TSCALIB_MD_BASE         ((uint32_t)0x1FF80078)    /*!< Calibration Data Bytes base address for medium density devices*/
#define FACTORY_TSCALIB_MDP_BASE        ((uint32_t)0x1FF800F8)    /*!< Calibration Data Bytes base address for medium density plus devices*/
#define FACTORY_TSCALIB_MD_DATA         ((TSCALIB_TypeDef *) FACTORY_TSCALIB_MD_BASE)
#define FACTORY_TSCALIB_MDP_DATA        ((TSCALIB_TypeDef *) FACTORY_TSCALIB_MDP_BASE)
#define USER_CALIB_BASE                 ((uint32_t)0x08080000)    /*!< USER Calibration Data Bytes base address */
#define USER_CALIB_DATA                 ((TSCALIB_TypeDef *) USER_CALIB_BASE)
#define TEST_CALIB_DIFF                 (int32_t) 50  /* difference of hot-cold calib  data to be considered as valid */ 

#define HOT_CAL_TEMP 	110
#define COLD_CAL_TEMP  	25

#define DEFAULT_HOT_VAL 0x362
#define DEFAULT_COLD_VAL 0x2A8

#define MAX_TEMP_CHNL 16

#define ADC_CONV_BUFF_SIZE 20


 
// global voltages variables from main
extern uint16_t Volt_LIBAT;
extern uint16_t Volt_SOBAT;
extern uint16_t Volt_INSTR;
extern uint16_t Volt_GSMMO;
extern uint16_t Volt_REFIN;
extern uint16_t Volt_TSENS;
extern int16_t  TemperatureInternal;
extern uint16_t Volt_VCC;
extern float koeffRAIN;
extern unsigned char at_in[MAX_AT_SIZE];
/* measurements procedures*/

void ADC_Measure_Init(void);
uint16_t Volts_Measurement (uint8_t Channel);

void CNT_GetVoltages(void); /* measure all voltage channels*/

void CNT_MGMT_GetRAIN(void); // расчет коэффициента для АЦП

void CNT_MGMT_ExtLinesVolts( uint16_t *ChanList, uint32_t *VoltList);


// from temp sensor example
void  writeCalibData(TSCALIB_TypeDef *calibStruct);
void getFactoryTSCalibData(TSCALIB_TypeDef *calibdata);
ErrorStatus  testUserCalibData(void);
ErrorStatus  testFactoryCalibData(void);



#endif /* __GCOUNTER_MEASURE_H*/