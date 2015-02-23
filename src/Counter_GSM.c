#include "Counter_GSM.h"
#include <stdlib.h>     /* itoa */
#include <string.h> // strcat

unsigned char* gsmimei = "123456789012345";
unsigned char smstext[150]; //  = "reserve space for create sms text procedure" ;
unsigned char hs1[100]; //  "reserve space for create sms text procedure" ;
unsigned char hs2[100]; //"reserve space for create sms text procedure" ;   




// GSM module power On
uint16_t CNT_GSM_Module_ON(void)
{
  // check last voltages
  uint32_t waitingtime;
  // PWR On Line define defines
  // SIM_PORT,  NRST_PIN,  PWRKEY_PIN
  
  CNT_GPIO_GSM_Lines(ON);  // activate all gsm module lines
  if (GPIO_ReadInputDataBit(SIM_STATUS_PORT, GSM_STATUS_PIN) == 1) return ON; // уже включен
  
  /* Enable Booster for GSM power and wait power good*/
  CNT_PWR_BOOSTER(ON);
  
  /* PWRkey impulse for GSM Start*/
    CNT_DummyDelay(DYMMYPWRKEYWIDTH); // delay x10 nops
  //GPIO_HIGH(LED_PORT,LED1_PIN); // LED for info
  GPIO_HIGH(SIM_PORT,PWRKEY_PIN);
  CNT_DummyDelay(DYMMYPWRKEYWIDTH); // delay x10 nops
  GPIO_LOW(SIM_PORT,PWRKEY_PIN);
  
    //GPIO_LOW(LED_PORT,LED1_PIN); // LED for info
  /* Check status pin from GSM */
    waitingtime = DYMMYPWRKEYWIDTH*4; // max 4 sec to On
    
    while (     (GPIO_ReadInputDataBit(SIM_STATUS_PORT, GSM_STATUS_PIN) != 1 )
            &&  ( waitingtime-- > 1) ) 
    { /* wait*/ }
      
    if (GPIO_ReadInputDataBit(SIM_STATUS_PORT, GSM_STATUS_PIN) == 1) 
    { 
      return ON;
    }
    else 
    {
      GPIO_HIGH(LED_PORT,LED1_PIN); // LED for info
      CNT_PWR_BOOSTER(OFF);
      return OFF;
    }
}


// GSM module power OFF
void CNT_GSM_Module_OFF(void)
{
   //CNT_GPIO_GSM_Lines(ON);  // activate all gsm module lines
   /* Disable Booster for GSM power*/
   CNT_PWR_BOOSTER(OFF);
   CNT_GPIO_GSM_Lines(OFF);  // de-activate all gsm module lines
   //GPIO_LOW(LED_PORT,LED2_PIN); // LED for info
  
}



uint16_t CNT_GSM_SendDefaultSMS(void) // send sms with default params
{   
    //int32_t waitingtime; // для таймаута
    //unsigned char* phone = DEFPHONENUMBER;

    uint16_t result = OFF;
    
    if (CNT_GSM_Module_ON()== ON) 
     {  CNT_UART_state (ON);
        put_atcmd("AT");
        CNT_DummyDelay(70000); // delay x10 nops - debug
        get_string();
        
        put_atcmd("ATE1");
        CNT_DummyDelay(70000); // delay x10 nops - debug
        get_string();
        
        /*put_atcmd("AT+GSV"); // simcom info
        CNT_DummyDelay(70000); // delay x10 nops - debug
        get_string();
        */
        
        /*put_atcmd("AT+CGID"); // sim group id
        CNT_DummyDelay(70000); // delay x10 nops - debug
        get_string();
        
        put_atcmd("AT+CSPN"); // sim service provider name
        CNT_DummyDelay(70000); // delay x10 nops - debug
        get_string();
        */
        
        /*put_atcmd("AT+CLTS=1"); // local timestamp mode
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();*/
                
        put_atcmd("AT+CMGD=1,4");  // delete !!!ALL!!! SMS from sim
        CNT_DummyDelay(100000); // delay x10 nops - debug
        get_string();
       
    
        put_atcmd("AT+CBC");  // read supply voltage Voltgsm
        CNT_GetVoltages();
        CNT_DummyDelay(100000); // delay x10 nops - debug
        get_string();  
        CNT_MGMT_GetRAIN(); // get adc correction
        CNT_GetVoltages(); 
        CNT_GPIO_CheckChannel();   // check channel circuit
                
        put_atcmd("AT+CREG?");  // registered in network?
        CNT_DummyDelay(100000); // delay x10 nops - debug
        get_string();
        
        put_atcmd("AT+CSQ");   // Signal Quality?
        CNT_DummyDelay(100000); // delay x10 nops - debug
        get_string();
        
        put_atcmd("AT+GSN");    // IMEI request
        CNT_DummyDelay(300000); // delay x10 nops - debug
        get_string();  // get imei
     
        //put_atcmd("AT+CCLK?");    // current clock request - не работает - оператор редко передает время.
        //CNT_DummyDelay(300000); // delay x10 nops - debug
        //get_string();  // get clock
        
        put_atcmd("AT+CMTE?");    // current temp request
        CNT_DummyDelay(300000); // delay x10 nops - debug
        get_string();  // get temp

        put_atcmd("AT+CMGF=1"); // select SMS format - text
        CNT_DummyDelay(300000); // delay x10 nops - debug
        get_string();        
        
          
        /*put_atcmd("AT+CSMP=?");
        CNT_DummyDelay(300000); // delay x10 nops - debug
        get_string();  // get ok
         */      
          
        put_atcmd("AT+CSMP=49,169,0,0");  // sms delivery request config
        CNT_DummyDelay(300000); // delay x10 nops - debug
        get_string();  // get ok
 
        /*put_atcmd("AT+CNMI=2,1,0,1,0"); // configere new message actions 
        CNT_DummyDelay(300000); // delay x10 nops - debug
        get_string();  // get ok
        CNT_DummyDelay(30); // delay x10 nops - debug
        */
        
       /* put_atcmd("ATD*100#");    // current balance request - для мегафона не работает стандартный #100#
                                  // на остальных операторах - ок. #100# возвращает баланс латиницей
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();  // get balance
        CNT_DummyDelay(10000000); // delay x10 nops - debug
        get_string();  // get balance
       */ 
        CNT_GSM_CreateSMSText(SMSVAR_DEF);
        result = CNT_GSM_PutSMS((unsigned char*)currentphonenumber, (unsigned char*)smstext); 
         
     }
       
    CNT_UART_state (OFF);  
    return result; 
                  
}
     

uint16_t CNT_GSM_PutSMS(unsigned char* phone, unsigned char* smstext)  // подать смс в буффер и отправить return on - off
{
        uint16_t result = OFF;  // результат отправки
        uint32_t waitingtime; 
        
        put_atcmd_noCRLF("AT+CMGS=");
        put_atcmd_noCRLF(phone);
        //put_atcmd_noCRLF("\"");
        put_char(CR);
          
        waitingtime = 30;
        while ( (get_char() != '>')   &&(waitingtime-- > 1 )  ) {;}// wait for welcome
        
        if (waitingtime ==0 ) return result;
        put_atcmd_noCRLF(smstext);
        put_char(CTRLZ);
        put_char(CR);
        
        CNT_DummyDelay(400000); // delay x10 nops - debug
        get_string();
         
        
        waitingtime = 10; // wait delivery report
        
        while ((waitingtime-- >0) && (result==OFF) ) 
          {     CNT_DummyDelay(100000); // delay x10 nops - debug
                get_string();
                if (  (at_in[2] == '+') &&
                      (at_in[3] == 'C') &&   // check CDS receiving - sms delivery report
                      (at_in[4] == 'D') &&
                      (at_in[5] == 'S')
                     )
                  { CNT_TIME_SetTimeFromSMSDeliveryReport();
                    result = ON;   // sms sended and delivery
                  }
          }
        
        // waiting command SMS 
        
        
        
        
        return result;
}


void CNT_GSM_CreateSMSText(uint16_t variant)
{
  unsigned char  space[] = " "; 
  unsigned char  comma[] = ", ";
  unsigned char  twopo[] = ":";
  unsigned char  slash[] = "/";  
  unsigned char alcnt[] = "Alarms: ";
  unsigned char vlts[] = "V=";
  unsigned char tmps[] = "T=";
  unsigned char head[] = "CNT Online. Pulses= ";
  unsigned char headalarm[] = "CNT ALARM! Pulses=";
  unsigned char openshort[] = "Ch open/short = ";
  
        smstext[0] = '\0';
        
        CNT_Get_RTC_Time();
        CNT_Get_RTC_Date();
       
        itoa(day, hs1); concat(smstext, hs1); concat(smstext, slash); 
        itoa(month, hs1); concat(smstext, hs1); concat(smstext, slash); 
        itoa(year, hs1); concat(smstext, hs1); concat(smstext, space); 
        itoa(hours, hs1); concat(smstext, hs1); concat(smstext, twopo); 
        itoa(minutes, hs1); concat(smstext, hs1); concat(smstext, twopo);
        itoa(seconds, hs1); concat(smstext, hs1); concat(smstext, comma);
        
        
        
   switch (variant)
   {
    case SMSVAR_DEF:
     {  concat(smstext, head);
        itoa(Pulse1Overall, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Pulse2Overall, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Pulse3Overall, hs1); concat(smstext, hs1); concat(smstext, comma);
        
        concat(smstext, alcnt);
        itoa(Alarms1Count, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Alarms2Count, hs1); concat(smstext, hs1); concat(smstext, comma);
        
        concat(smstext, vlts);
        itoa(Volt_LIBAT, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Volt_SOBAT, hs1); concat(smstext, hs1); concat(smstext, comma);
        break;
     }
   
   case SMSVAR_Alarm:
    {
        concat(smstext, headalarm);
        itoa(Pulse1Overall, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Pulse2Overall, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Pulse3Overall, hs1); concat(smstext, hs1); concat(smstext, comma);
        
        concat(smstext, alcnt);
        itoa(Alarms1Count, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Alarms2Count, hs1); concat(smstext, hs1); concat(smstext, comma);
        
        concat(smstext, vlts);
        itoa(Volt_LIBAT, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(Volt_SOBAT, hs1); concat(smstext, hs1); concat(smstext, comma);
        break;
    }
  }


        concat(smstext, openshort);  
        itoa(channel_opened, hs1); concat(smstext, hs1); concat(smstext, slash);
        itoa(channel_shorted, hs1); concat(smstext, hs1); concat(smstext, comma);
        
        concat(smstext, tmps);
        itoa(TemperatureInternal, hs1); concat(smstext, hs1);

}


uint16_t CNT_GSM_SendAlarmSMS   (void)  // immediatelly charge battery and send sms
{
    //int32_t waitingtime; // для таймаута
    //unsigned char* phone = DEFPHONENUMBER;

    uint16_t result = OFF;
    
    if (CNT_GSM_Module_ON()== ON) 
     {  CNT_UART_state (ON);
        put_atcmd("AT");
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();
        
        put_atcmd("ATE0");
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();
        
        /*put_atcmd("AT+CLTS=1"); // local timestamp mode
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();*/
                
        put_atcmd("AT+CMGD=1,4");  // delete !!!ALL!!! SMS from sim
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();
       
        put_atcmd("AT+CREG?");  // registered in network?
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();
        
        put_atcmd("AT+CSQ");   // Signal Quality?
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();
        
        put_atcmd("AT+GSN");    // IMEI request
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();  // get imei
          
        CNT_GetVoltages();
        put_atcmd("AT+CBC");  // read supply voltage Voltgsm
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();  
        CNT_MGMT_GetRAIN(); // get adc correction
        CNT_GetVoltages(); 
        CNT_GPIO_CheckChannel();   // check channel circuit
 
        put_atcmd("AT+CMGF=1"); // select SMS format - text
        CNT_DummyDelay(1000000); // delay x10 nops - debug
        get_string();        

        CNT_GSM_CreateSMSText(SMSVAR_Alarm);
        result = CNT_GSM_PutSMS(currentphonenumber, (unsigned char*)smstext); 
     }
       
    CNT_UART_state (OFF);  
    return result; 
  
}
