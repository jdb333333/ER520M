/****************************************Copyright (c)**************************************************
**                               Hunan EUTRON information Equipment Co.,LTD.
**
**                                 http://www.eutron.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:       LCD_Drv.c
** Descriptions:    S6A0069驱动
**
**------------------------------------------------------------------------------------------------------
** Created by:      Ruby
** Created date:    2011-12-05
** Version:         1.0
** Descriptions:    The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
********************************************************************************************************/
#include "stm32f2xx.h"
#include "lcd_160x1.h"
#include "fsmc_sram.h"
#include "SysTick.h"


#define LCD_D0	GPIO_Pin_8
#define LCD_D1	GPIO_Pin_9
#define LCD_D2	GPIO_Pin_10
#define LCD_D3	GPIO_Pin_11
#define LCD_DataPort	GPIOF

#define LCDC_E	GPIO_Pin_15//GPIO_Pin_14  //Customer
#define LCDO_E	GPIO_Pin_14//GPIO_Pin_15  //Operator
#define LCD_E_Port		GPIOG

#define LCD_RS	GPIO_Pin_7
#define LCD_RW	GPIO_Pin_6
#define LCD_BL	GPIO_Pin_2

#define	RS_Low()	GPIOF->BSRRH =	LCD_RS
#define	RS_High()	GPIOF->BSRRL =	LCD_RS

#define	RW_Low()	GPIOF->BSRRH =	LCD_RW
#define	RW_High()	GPIOF->BSRRL =	LCD_RW

#define	C_E_Low()		LCD_E_Port->BSRRH =	LCDC_E
#define	C_E_High()	LCD_E_Port->BSRRL =	LCDC_E
#define	O_E_Low()		LCD_E_Port->BSRRH =	LCDO_E
#define	O_E_High()	LCD_E_Port->BSRRL =	LCDO_E
#if defined(CASE_ITALIA)
const char  Disp[] = "   ECR - 1001   ";
#elif defined(CASE_MALTA)
const char  Disp[] = "                ";
#else
const char  Disp[] = "Fiscal System OK";
#endif
char SaveDispO[DISLEN],SaveDispC[DISLEN];//保护显示内容

volatile uint8_t LCD_BL_CTRL;

void LCD_CheckBusy(uint8_t LCDn);
void LCD_ModeInit4Bit(uint8_t LCDn);
void PutsO_Only(const char *str);
void PutsC_Only(const char *str);
void LCD160x_Test(void);

// 延时1ms
void LCD160x_Delay_ms(uint16_t cnt)
{
	uint16_t i;
	while (cnt--)
		for(i=0; i<20000; i++);
}

void LCD160x_Dly80ns(uint8_t cnt)
{
	while(cnt--)
		__NOP(); // __NOP()约80ns
    //>>>>>>>>>>>>>ouhs 20151106
    __NOP();__NOP();__NOP();__NOP();__NOP();
}

// 发送4bit数据
void LCD_Sendbit(uint16_t ch)
{
	ch <<= 8;
	LCD_DataPort->BSRRL = ch;
	LCD_DataPort->BSRRH = (~ch)&0xF00;

}

// 发送数据到LCD
void LCD_SendData(uint8_t LCDn, uint8_t data)
{
	if(LCDn)
		C_E_Low();
	else
		O_E_Low();

	LCD_Sendbit(data);
	//LCD160x_Dly80ns(1);
	if(LCDn)
		C_E_High();
	else
		O_E_High();
	LCD160x_Dly80ns(5);	 //410ns

	if(LCDn)
		C_E_Low();
	else
		O_E_Low();
	LCD160x_Dly80ns(1);
}

// 设置4位数据线模式
void LCD_ModeInit4Bit(uint8_t LCDn)
{
	RW_Low();
	RS_Low();
	LCD_SendData(LCDn, 0x02);		// 命令：0x28  4bit,16x1-line,5x8dots
	LCD_SendData(LCDn, 0x08);
}

// 读取状态标志
void LCD_CheckBusy(uint8_t LCDn)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint8_t ReadData, ReadData_H, ReadData_L;
	uint16_t temp = 0xfff;

    LCD_DataPort->BSRRL = LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3; //先置1再读取

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3;
	GPIO_Init(LCD_DataPort, &GPIO_InitStructure);
	RW_High();
	RS_Low();
	do{

		if(LCDn)
			C_E_High();
		else
			O_E_High();
		LCD160x_Dly80ns(5);
		ReadData_H = (LCD_DataPort->IDR)>>4;	// Pin11|Pin10|Pin9|Pin8

		if(LCDn)
			C_E_Low();
		else
			O_E_Low();
		LCD160x_Dly80ns(5);
	   	if(LCDn)
			C_E_High();
		else
			O_E_High();
		LCD160x_Dly80ns(5);
		ReadData_L = (LCD_DataPort->IDR)>>8;	// Pin11|Pin10|Pin9|Pin8

		if(LCDn)
			C_E_Low();
		else
			O_E_Low();
		LCD160x_Dly80ns(5);

		ReadData = ((ReadData_H) | (ReadData_L & 0x0F));
	}while((ReadData & 0x80) && (temp--));

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(LCD_DataPort, &GPIO_InitStructure);
}


// 写命令
uint8_t LCD_WRCmd(uint8_t LCDn, uint8_t commmand)
{
	LCD_CheckBusy(LCDn);
	RW_Low();
	RS_Low();
   	LCD_SendData(LCDn, commmand>>4);	   // 先发送高4位
	LCD_SendData(LCDn, commmand&0x0F);	   // 后发送低4位
	return 1;
}

// 写数据
uint8_t LCD_WRData(uint8_t LCDn, uint8_t data)
{
	LCD_CheckBusy(LCDn);
	RW_Low();
	RS_High();
   	LCD_SendData(LCDn, data>>4);	   // 先发送高4位
	LCD_SendData(LCDn, data&0x0F);	   // 后发送低4位
	return 1;
}

void LCD_Pos(uint8_t LCDn, uint8_t pos)
{
	pos &= 0x0f;
	if(pos<8)
		LCD_WRCmd(LCDn, pos | 0x80);
	else
		LCD_WRCmd(LCDn, (pos&0x07)|0xC0);
}

void LCD160x_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

	GPIO_InitStructure.GPIO_Pin = LCD_D0 | LCD_D1 | LCD_D2 | LCD_D3 | LCD_RS | LCD_RW;
	GPIO_Init(LCD_DataPort, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCD_BL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LCDC_E | LCDO_E;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	GPIO_Init(LCD_E_Port, &GPIO_InitStructure);
}


void LCDInit(void)
{
	LCD160x_IO_Init();

	// 初始化显示模式设置4次
	LCD_ModeInit4Bit(0);	// 4bit, 16x1-line, 5x8dots
	LCD_ModeInit4Bit(1);
	LCD160x_Delay_ms(30);	//ouhs 20151106	// 3ms

	LCD_ModeInit4Bit(0);
	LCD_ModeInit4Bit(1);
	LCD160x_Delay_ms(30);  //ouhs 20151106
    //>>>>>>>>>>>>>>ouhs 20151106
    LCD_ModeInit4Bit(0);
    LCD_ModeInit4Bit(1);
    LCD160x_Delay_ms(30);
    LCD_ModeInit4Bit(0);
    LCD_ModeInit4Bit(1);
    LCD160x_Delay_ms(30);
    //<<<<<<<<<<<<<<<<<<
    LCD_WRCmd(0, 0x08);		// 关显示
	LCD_WRCmd(1, 0x08);
	LCD_WRCmd(0, 0x01);		// 清屏
	LCD_WRCmd(1, 0x01);
	LCD160x_Delay_ms(10);	//ouhs 20151106 // 3ms

	LCD_WRCmd(0, 0x06);		// 写入新数据后,光标向右移动,画面不移动
	LCD_WRCmd(1, 0x06);

	LCD_WRCmd(0, 0x0c);		// 显示功能开、有光标、 无闪烁
	LCD_WRCmd(1, 0x0c);

	LCDOpen();			//开背光
    LCD_BL_CTRL |= 2;//=1,客显与主显显示相同的内容

	PutsO_Only(Disp);
	//LCD160x_Test();
}

#if 0
const char  Disp1[] = "0123456789abcdef";
void LCD160x_Test(void)
{
	uint8_t i;
	while(1)
	{
		for(i=0; i<50; i++)
		{
			PutsO_Only(Disp1);
			PutsO_Only(Disp);
		}
		LCD160x_Delay_ms(1000);

	}}
#endif

void LCDClose(void)
{
	LCD_BL_OFF();

	LCD_WRCmd(0, 0x01);
	LCD_WRCmd(1, 0x01);
}

void LCDOpen(void)
{
	LCD_BL_ON();
}

void PutsC(const char *str)
{//在顾客显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

	memset(SaveDispC,' ',DISLEN);

	LCD_WRCmd(1, 0x01);
	//LCD160x_Delay_ms(3);	// 3ms
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		LCD_Pos(1, addr);
		LCD_WRData(1, *str);
		SaveDispC[addr]=*str;
        if( ++addr >= DISLEN)
			break;
	}
}

void PutsC_Only(const char *str)
{//在顾客显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

	LCD_WRCmd(1, 0x01);
	//LCD160x_Delay_ms(3);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		LCD_Pos(1, addr);
		LCD_WRData(1, *str);
        if( ++addr >= DISLEN)
			break;
	}
}

//显示被保护的内容
void PutsC_Saved(void)
{
	PutsC_Only(SaveDispC);
}

//只显示，同时保存内容
void PutsO(const char *str)
{//在操作员显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;


	PutsC(str);

	memset(SaveDispO,' ',DISLEN);
	LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		LCD_Pos(0, addr);
		SaveDispO[addr]=*str;
		LCD_WRData(0, *str);
		if( ++addr >= DISLEN)
			break;
	}
}

uint8_t GetsO_At(uint8_t addr)
{
    if (addr<DISLEN)
        return SaveDispO[addr];
    else
        return 0xff;
}

////ccr20131120 在屏幕第一行指定位置显示字符
void PutsO_At(uint8_t ch,uint8_t addr)
{//在操作员显示屏幕上显示内容,16位1行显示

	//LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
    if (addr<DISLEN)
    {
        SaveDispO[addr] = ch;
        addr=0;
        LCD_WRCmd(0, 0x01);
        for(addr=0;addr<DISLEN;addr++) //需要修改结束字符标识
        {
            LCD_Pos(0, addr);
            //skip LCD_Pos(0, 1, addr);
            LCD_WRData(0, SaveDispO[addr]);
        }
    }
}

//只显示，不保存内容
void PutsO_Only(const char *str)
{//在操作员显示屏幕上显示内容,16位1行显示
	unsigned char addr = 0;

	PutsC_Only(str);

	LCD_WRCmd(0, 0x01);
	//LCD160x_Delay_ms(3);
	for(;*str!=0;str++) //需要修改结束字符标识
	{
		LCD_Pos(0, addr);
		LCD_WRData(0, *str);
		if( ++addr >= DISLEN)
			break;
	}
}

//显示被保护的内容
void PutsO_Saved(void)
{
	PutsO_Only(SaveDispO);
	PutsC_Only(SaveDispO);
}

//保护的LCD上显示的nrong内容
void Save_LCD(char *saveO,char *saveC)
{
	memcpy(saveO,SaveDispO,sizeof(SaveDispO));
	memcpy(saveC,SaveDispC,sizeof(SaveDispC));
}

/**
 * 在屏幕指定的ASCII字符位置显示电池容量
 *
 * @author EutronSoftware (2014-12-31)
 *
 * @param ctrl :0-不显示;=1测试并显示;=2显示
 */
#if defined(CASE_ASCIIDISPLAY)
#define VIN_FULL  VIN_8V7 //8.7V---3075mV---0xEE8   +修正值0x6B
#define VIN_NONE  VIN_7V0 //7.0V---1375mV---0x6AA
void mDrawPower(uint8_t ctrl)
{
    static uint16_t gPower=0;

		uint16_t pDot,j;

    //if (BatteryExist())//&& !DC_DET_GET())//只有在电池供电时,才显示电池容量
    {
        if (ctrl==0)
        {
            gPower = 0;
            return;
        }
        else if (ctrl==1)
        {
            if (gPower==0)
                return;
        }
        gPower = Vin_Vrtc_Val[0];

        if (gPower>=VIN_FULL)
            j=9;
        else if (gPower<=VIN_NONE)
            j = 0;
        else
            j = ((gPower-VIN_NONE)*9)/(VIN_FULL-VIN_NONE);
        PutsO_At(j +'0',0);
    }
}
#endif
/*********************************************************************************************************
** End Of File
*********************************************************************************************************/
