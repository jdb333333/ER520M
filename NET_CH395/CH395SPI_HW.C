/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395SPI_HW.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395芯片 硬件SPI串行连接的硬件抽象层 V1.0
*                     
*******************************************************************************/
#include "stm32f10x.h"

#define xCH395CmdStart( )         GPIO_ResetBits(GPIOA, GPIO_Pin_4);               /* 命令开始 */
#define xEndCH395Cmd()            GPIO_SetBits(GPIOA, GPIO_Pin_4);              /* 命令结束*/


void mDelayuS(uint8_t us )
{
	uint8_t i;
	
	while (us--)
		for(i=0; i<20; i++);	          
}

void mDelaymS( uint8_t ms )
{
	uint16_t i;
	while (ms--)
		for(i=0; i<20000; i++);
}

/******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : 硬件初始化部分
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;


	// 使能SPI1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |  GPIO_Pin_7;	//SCK,MOSI
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // MISO
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//SS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	GPIO_SetBits(GPIOA, GPIO_Pin_4);    //置高 分配给ENC28J60的 SPI1_NSS信号


	// SPI1 配置:全双工、主机、8位帧、SCK上升沿采样(SCK极性为0、相位为0)、CS由软件控制、
	//			 SCK最大速度APB2/2=30MHz、高位在前
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
#else
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
#endif	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	// 使能SPI1
	SPI_Cmd(SPI1, ENABLE);
	
}

/*******************************************************************************
* Function Name  : Spi395Exchange
* Description    : 硬件SPI输出且输入8个位数据
* Input          : d---将要送入到CH395的数据
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t Spi395Exchange( uint8_t d )  
{  
	while ((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = d;
	while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET);
	return SPI1->DR;
}

/******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : 向CH395写命令
* Input          : cmd 8位的命令码
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(uint8_t cmd)                                          
{                                                                    
    xEndCH395Cmd();                                                  /* 防止CS原来为低，先将CD置高 */
    xCH395CmdStart( );                                               /* 命令开始，CS拉低 */
    Spi395Exchange(cmd);                                             /* SPI发送命令码 */
    mDelayuS(2);                                                     /* 必要延时,延时1.5uS确保读写周期不小于1.5uS */
}

/******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : 向CH395写数据
* Input          : mdata 8位数据
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(uint8_t mdata)
{   
    Spi395Exchange(mdata);                                           /* SPI发送数据 */
}

/*******************************************************************************
* Function Name  : xReadCH395Data
* Description    : 从CH395读数据
* Input          : None
* Output         : None
* Return         : 8位数据
*******************************************************************************/
uint8_t   xReadCH395Data( void )                                                  
{
    uint8_t i;
    i = Spi395Exchange(0xff);                                        /* SPI读数据 */
    return i;
}

/**************************** endfile *************************************/


