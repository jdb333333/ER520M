#ifndef __LWIP_INIT_H
#define __LWIP_INIT_H

#include "stm32f2xx.h"

#include "err.h"
#include "init.h"
#include "etharp.h"
#include "udp.h"
#include "tcp.h"


void LwIP_Init( uint8_t IP1,uint8_t IP2,uint8_t IP3,uint8_t IP4);
void Time_Update(void);
void LwIP_Periodic_Handle(__IO uint32_t localtime,uint8_t fromINT);
void Set_ETHERNET_Ready(void);
uint8_t Get_ETHERNET_Ready(void);
#endif






