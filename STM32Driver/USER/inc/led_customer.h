
#ifndef __LED_CUSTOMER_H
#define __LED_CUSTOMER_H

#if defined(CASE_ER260)
#define DISP_WIDTH  	8
#else
#define DISP_WIDTH  	12
#endif


#if (DISP_WIDTH==8)
#define LED_G_CON  	0xff
#else
#if CASE_ER520U
#define LED_G_CON  	0x000
#else
#define LED_G_CON  	0xfff
#endif
#endif


void LED_Init(void);
void LED_DispData(uint8_t* pBuffer, uint8_t BufferSize);
void LED_Refresh(uint8_t SigBit);
void LED_On(void);
void LED_Off(void);
void SysDelay_ms(uint16_t delay_ms);

#endif // __LED_CUSTOMER_H
