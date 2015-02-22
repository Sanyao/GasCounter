/* initialization procedure file header */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_GSM_H
#define __GCOUNTER_GSM_H

/* Includes ------------------------------------------------------------------*/
//#include "main.h"
//#include "Counter_GPIO_define.h"
#include "Counter_GPIO_utils.h"
#include "Counter_Misc.h"
#include "Counter_UART.h"
#include "Counter_PwrMgmt.h"


#define DYMMYPWRKEYWIDTH 3000000
#define DEFPHONENUMBER  "\"+79853520273\",145"
#define DEFPHONENUMBER2 "\"+79036326317\",145"
#define DEFWAITINGCOMMANDSMSTIME (uint16_t)60 // seconds



// Variant for SMS
#define SMSVAR_DEF              1       // default operational SMS
#define SMSVAR_VarnLOWBATT      2       // SMS for low battery alarm
#define SMSVAR_Alarm            3       // alarm input active!



 /* define external global variables */
extern uint32_t Pulse1Overall;
extern uint32_t Pulse2Overall;
extern uint32_t Pulse3Overall;
extern uint32_t Pulse1Current;
extern uint32_t Pulse2Current;
extern uint32_t Pulse3Current;

extern uint32_t Alarms1Count;
extern uint32_t Alarms2Count;

extern unsigned char* s;
extern volatile unsigned char* sms;

extern uint16_t channel_shorted; // закороченные каналы
extern uint16_t channel_opened; // обрыв каналов

// global voltages variables
extern uint16_t Volt_LIBAT; 
extern uint16_t Volt_SOBAT;
extern uint16_t Volt_INSTR;
extern uint16_t Volt_GSMMO;
extern uint16_t Volt_REFIN;
extern uint16_t Volt_TSENS;

extern unsigned char currentphonenumber[];

uint16_t CNT_GSM_Module_ON      (void); // GSM power On - return on for normal or off for abnormal
void     CNT_GSM_Module_OFF     (void);// GSM module power OFF
uint16_t CNT_GSM_SendDefaultSMS (void);
uint16_t CNT_GSM_SendAlarmSMS   (void);


uint16_t CNT_GSM_PutSMS(unsigned char* phone, unsigned char* smstext);  // подать смс в буффер и отправить return on - off
void CNT_GSM_CreateSMSText(uint16_t variant);  // create various text for sms

#endif /* __GCOUNTER_GSM_H*/