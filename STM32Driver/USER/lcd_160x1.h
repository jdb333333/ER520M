/****************************************Copyright (c)**************************************************
**                               Hunan EUTRON information Equipment Co.,LTD.
**
**                                 http://www.eutron.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:            LCD_Drv.h
** Last modified Date:   2011-12-05
** Last Version:         1.0
** Descriptions:         LCDµÄ²Ù×÷º¯Êý
**
**------------------------------------------------------------------------------------------------------
** Created By:           Ruby
** Created date:         2011-12-05
** Version:              1.0
** Descriptions:         First version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
********************************************************************************************************/

#ifndef __LCD_160X1_H
#define __LCD_160X1_H

#define     DISLEN 		16

extern volatile uint8_t LCD_BL_CTRL;

uint8_t LCD_WRCmd(uint8_t LCDn, uint8_t commmand);
uint8_t LCD_WRData(uint8_t LCDn, uint8_t data);

void LCD_Pos(uint8_t LCDn, uint8_t pos);

#endif	// __LCD_160X_H

/********************************************************************************************************
** End Of File
*********************************************************************************************************/

