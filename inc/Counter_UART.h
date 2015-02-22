 /* initialization procedure file header */

/* Includes ------------------------------------------------------------------*/



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GCOUNTER_UART_H
#define __GCOUNTER_UART_H

#include "main.h"
#include "Counter_GPIO_utils.h"
#include "Counter_Misc.h"
#include "Counter_GPIO_define.h"
#include "Counter_GSM.h"



#define LF 0x0A // \n symbol
#define NULL 0x00 // NULL
#define CR 0x0D
#define CTRLZ 0x1A // CTRL-Z 



void CNT_UART_init(void);
void CNT_UART_state(uint16_t state);  // on or off uart
void USART1_IRQHandler(void);
uint8_t get_char(void); //прием данных
void put_char(uint8_t c); //вывод данных
void put_string(unsigned char *s);
void get_string(void);
void put_atcmd(unsigned char *s);
void put_atcmd_noCRLF(unsigned char *s);


void  CNT_UART_initat_in(void);  // clear at in buffer





#endif /* __GCOUNTER_uart_H*/