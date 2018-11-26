
#ifndef __TEST_H
#define __TEST_H


#include "include.h"




/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SRAM_DEBUG		1
#define	SPIFLASH_DEBUG		1
#define NOR_FLASH_TEST 1

uint32_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

void TIM6_Init_Count(void);
uint8_t Test_SRAM(void);
BYTE Test_BKPSRAM(void);
void Test_SPIFlash(void);

void NOR_Flash_Test( void );

void Ethernet_Test(void);

#endif   //__TEST_H


