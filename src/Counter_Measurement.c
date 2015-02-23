#include "Counter_Measurement.h"
#include "stm32l1xx_adc.h"
#include <stdlib.h>     /* atoi */

// from temp sensor example
__IO FLASH_Status FLASHStatus = FLASH_COMPLETE; 
TSCALIB_TypeDef calibdata;    /* field storing temp sensor calibration data */

/**
  * @brief ADC initialization (ADC_Channel_9 - PB1)
  * @caller main and ADC_Test
  * @param None
  * @retval None
  */ 
 

        
    
void ADC_Measure_Init(void)
{
  ADC_InitTypeDef ADC_InitStructure;
 
/* Enable ADC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
   
/* de-initialize ADC */
  ADC_DeInit(ADC1);

/*  ADC configured as follow:
  - NbrOfChannel = 1 (ADC_CH_NUM = ADC_Channel_9)
  - Mode = Single ConversionMode(ContinuousConvMode disabled)
  - Resolution = 12Bits
  - Prescaler = /1
  - sampling time 192 */

    /* ADC Configuration */
  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel 9 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_384Cycles);
  ADC_DelaySelectionConfig(ADC1, ADC_DelayLength_Freeze);

  ADC_PowerDownCmd(ADC1, ADC_PowerDown_Idle_Delay, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* Wait until ADC1 ON status */
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET);
}



/**
  * @brief volts measurement
  * @caller main and ...
  * @param Channel - 1...4 - external switch, 0 - temp ref, 5 - Vinternalref, 
  * @retval ADC conversion value
  */
uint16_t Volts_Measurement (uint8_t Channel)
{
  uint32_t res,i;
  if (Channel>5) {return 0;}  
  
  /* re-start ADC chanel for Current measurement */
  ADC_Measure_Init();	

  /* connect source */
  switch (Channel)
  { case 0:   /* ADC1  channel TrempSensor configuration */
            ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_384Cycles);
    break;
    
    case 1: // connect CH1 - ADC_IN_VBAT
            GPIO_HIGH(ADC_SW_PORT,EN1_PIN);	// Push High
    break;
    
    case 2: // connect CH2 - ADC_IN_HPOWER
            GPIO_HIGH(ADC_SW_PORT,EN2_PIN);	// Push High
    break;  
   
    case 3: // connect CH1 - ADC_IN_BATTVTG
            GPIO_HIGH(ADC_SW_PORT,EN3_PIN);	// Push High
    break;  
    
    case 4: // connect CH1 - ADC_IN_???
            GPIO_HIGH(ADC_SW_PORT,EN4_PIN);	// Push High
    break;  
    
    case 5:  /* ADC1  channel VREF configuration */
            ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1, ADC_SampleTime_384Cycles);
    break;  
  }
  

  for (i=0; i<50000; i++) { __NOP(); //__NOP(); __NOP();  // delay for transients 
                            //__NOP(); __NOP(); __NOP();
                            //__NOP(); __NOP(); __NOP();
                            }
  /* initialize result */
  res = 0;

  for(i=32; i>0; i--) // 32 измерения
  {
    /* create interrupt for enter to sleep */
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC); // clear flag
    //ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
    //NVIC_EnableIRQ(ADC1_IRQn);  
    //PWR_UltraLowPowerCmd(ENABLE); 
    //GPIO_HIGH(LED_PORT,LED2_PIN); // LED for info
    /* start ADC convertion by software */
    
    ADC_SoftwareStartConv(ADC1);
     
    /* minimize power!!! - go to sleep */
    //PWR_EnterSleepMode(PWR_Regulator_ON, PWR_SLEEPEntry_WFI);
    //GPIO_LOW(LED_PORT,LED2_PIN); // LED for info
     /* wait until end-of-covertion */
    while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 ); // wait
    
    /* read ADC convertion result */
    res += (uint32_t)ADC_GetConversionValue(ADC1);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC); // clear flag
    
  }
  
   /* disconnect all ext sources */   
      GPIO_LOW(ADC_SW_PORT,EN4_PIN);	// Push LOW
      GPIO_LOW(ADC_SW_PORT,EN3_PIN);	// Push LOW
      GPIO_LOW(ADC_SW_PORT,EN2_PIN);	// Push LOW
      GPIO_LOW(ADC_SW_PORT,EN1_PIN);	// Push LOW
  
  /* de-initialize ADC */
  ADC_DeInit(ADC1);
    ADC_Cmd(ADC1, DISABLE);
  return (res>>5);
}


void CNT_GetVoltages(void)
{ /* measure all voltage channels*/
   
  uint32_t ADC_Result,  refAVG, Address = 0;
  float koeff; 
  int32_t INTemperature, tempAVG, temperature_C; 

      /* Enable HSI Clock */
    RCC_HSICmd(ENABLE);
    /*!< Wait till HSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
    {}
    //RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
  
  
  /* Test user or factory temperature sensor calibration value */
  if ( testUserCalibData() == SUCCESS ) calibdata = *USER_CALIB_DATA;
  else if ( testFactoryCalibData() == SUCCESS ) getFactoryTSCalibData(&calibdata);
  else {   /* User calibration or factory calibration TS data are not available */
		calibdata.TS_CAL_1 = DEFAULT_COLD_VAL;
    calibdata.TS_CAL_2 = DEFAULT_HOT_VAL;
    writeCalibData(&calibdata); 
    calibdata = *USER_CALIB_DATA;
  }  
  
    // get reference and Vcc
  Volt_REFIN = Volts_Measurement(5) ;
  Volt_VCC = (uint16_t)(3000 * calibdata.VREF / Volt_REFIN) ;

  tempAVG = (int32_t) (Volts_Measurement(0) + Volts_Measurement(0))* 5 ; // *10 /2 = *5
  tempAVG = (int32_t) (tempAVG * calibdata.VREF/Volt_REFIN  );
  
    /* Calculate temperature in °C from Interquartile mean */
  temperature_C = ( (int32_t) tempAVG - ((int32_t) calibdata.TS_CAL_1*10 ) ) ;	
  temperature_C = temperature_C * (int32_t)(HOT_CAL_TEMP*10 - COLD_CAL_TEMP*10);                      
  temperature_C = temperature_C / (int32_t)(calibdata.TS_CAL_2*10 - calibdata.TS_CAL_1*10); 
  temperature_C = temperature_C + COLD_CAL_TEMP*10; 
    
    TemperatureInternal = (int16_t)( temperature_C );
    Volt_TSENS = tempAVG;
    
    /*
    #define VOLTKOEFF(b,a)          b = (uint16_t)((float)a*( (float)Volt_VCC/(float)4095.0) * (float)33.0/ (float)22.0); // for millivolts ADC result*/
    koeff = ((float)Volt_VCC/(float)4096.0) * 33.001 * koeffRAIN / 21.501;
   
    ADC_Result = Volts_Measurement(4);
    Volt_SOBAT = (uint16_t)(koeff*ADC_Result); //
    
    ADC_Result = Volts_Measurement(3);
    Volt_LIBAT = (uint16_t)(koeff*ADC_Result); // 
    
    ADC_Result = Volts_Measurement(2);
    Volt_INSTR = (uint16_t)(koeff*ADC_Result); 
    
    ADC_Result = Volts_Measurement(1);
    Volt_GSMMO = (uint16_t)(koeff*ADC_Result); // VBAT
    
    //RCC_SYSCLKConfig(RCC_SYSCLKSource_MSI);
    RCC_HSICmd(DISABLE);

}  

// from tempsensor example
/*---------------------------------------------------------------------------*/


void getFactoryTSCalibData(TSCALIB_TypeDef* data)
{
uint32_t deviceID;

	deviceID = DBGMCU_GetDEVID();
	
	if (deviceID == 0x427) *data = *FACTORY_TSCALIB_MDP_DATA;
		else if (deviceID == 0x416) *data = *FACTORY_TSCALIB_MD_DATA;
			else while(1); // add error handler - device cannot be identified calibration data not loaded!
}


ErrorStatus  testUserCalibData(void)
{
  int32_t testdiff;
  ErrorStatus retval = ERROR;
  
  testdiff = USER_CALIB_DATA->TS_CAL_2 - USER_CALIB_DATA->TS_CAL_1;
  
  if ( testdiff > TEST_CALIB_DIFF )    retval = SUCCESS;
  
  return retval;
}

ErrorStatus  testFactoryCalibData(void)
{
  int32_t testdiff;
  ErrorStatus retval = ERROR;
  TSCALIB_TypeDef datatotest;
	
	getFactoryTSCalibData (&datatotest);
	
  testdiff = datatotest.TS_CAL_2 - datatotest.TS_CAL_1;
  
  if ( testdiff > TEST_CALIB_DIFF )    retval = SUCCESS;
  
  return retval;
}

void  writeCalibData(TSCALIB_TypeDef* calibStruct)
{

  uint32_t  Address = 0;
  uint32_t  dataToWrite;
  
  /* Unlock the FLASH PECR register and Data EEPROM memory */
  DATA_EEPROM_Unlock();
  
  /* Clear all pending flags */      
  FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR
                  | FLASH_FLAG_SIZERR | FLASH_FLAG_OPTVERR);	
  
  /*  Data EEPROM Fast Word program of FAST_DATA_32 at addresses defined by 
      DATA_EEPROM_START_ADDR and DATA_EEPROM_END_ADDR */  
 
  Address = (uint32_t) USER_CALIB_DATA;


  dataToWrite = 0x00;
  dataToWrite = (uint32_t)(calibStruct->TS_CAL_1) << 16;
  
  FLASHStatus = DATA_EEPROM_ProgramWord(Address, dataToWrite);

  if(FLASHStatus != FLASH_COMPLETE)
  {
    while(1); /* stay in loop in case of crittical programming error */
  }

  Address += 4;

  dataToWrite = 0x00;
  dataToWrite = (uint32_t)(calibStruct->TS_CAL_2) << 16;
  
  FLASHStatus = DATA_EEPROM_ProgramWord(Address, dataToWrite);
  
}



void CNT_MGMT_GetRAIN(void) // расчет коэффициента для АЦП
{ // данные в ответе на команду АТ=CDC в массиве extern unsigned char at_in[MAX_AT_SIZE];
  
  uint16_t vsupply; // напряжение питания от sim900
  uint16_t strpoint=0;
  while (  (at_in[strpoint++]!=',') && (at_in[strpoint] != 0x00) ) ; // find first comma
  while (  (at_in[strpoint++]!=',') && (at_in[strpoint] != 0x00) ) ; // find second comma
  vsupply = atoi( (char const*)&at_in[strpoint]);
  if ( (vsupply > 3500) && (Volt_GSMMO > 2000))
      { koeffRAIN = koeffRAIN* ((float)vsupply / (float)Volt_GSMMO);
      }
  }
  
  
void CNT_MGMT_ExtLinesVolts( uint16_t *ChanList, uint32_t *VoltList)
{ // Измерение напряжений по списку каналов (должны быть сконфигурированы)
  uint32_t ADC_Result;
  uint32_t i,j, res; 
  float t;

    /* Enable HSI Clock */
    RCC_HSICmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}    /*!< Wait till HSI is ready */
    //RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    
    /* re-start ADC chanel for Current measurement */
    ADC_Measure_Init();	
    i = sizeof(ChanList);
    for (i=0; i<= sizeof(ChanList); i++) 
    { ADC_RegularChannelConfig(ADC1, ChanList[i], 1, ADC_SampleTime_384Cycles);
      CNT_DummyDelayLP_TO(50);
      res=0; // clear result
         for(j=16; j>0; j--) // 16 измерения
         {    
            ADC_ClearFlag(ADC1, ADC_FLAG_EOC); // clear flag
            ADC_SoftwareStartConv(ADC1);
             /* wait until end-of-covertion */
            while( ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == 0 ) {;} // wait
            res += ADC_GetConversionValue(ADC1);/* read ADC convertion result */
            ADC_ClearFlag(ADC1, ADC_FLAG_EOC); // clear flag
         }
    
        t =  ((float)Volt_VCC/(float)4096.0)* (float)( res>>4);
          VoltList[i] = (uint32_t) t;
    }
    
    RCC_HSICmd(DISABLE); // clock off for energy save

}

