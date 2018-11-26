#ifndef __SPI_ENC28J60C_H
#define __SPI_ENC28J60C_H

#include "stm32f2xx.h"

extern __IO uint32_t LocalTime; /* this variable is used to create a time reference incremented by 10ms */

void Ethernet_Init(uint8_t IP1,uint8_t IP2,uint8_t IP3,uint8_t IP4);


void ENC_SPI_Init(void);
uint8_t ENC_SPI_ReadWrite(uint8_t writedat);


#endif
