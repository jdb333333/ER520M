/**********************************************************************************/

#include "spi_enc28j60.h"
#include "usart.h"

#include "err.h"
#include "etharp.h"
#include "netconfig.h"

__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */


void Ethernet_Init(uint8_t IP1,uint8_t IP2,uint8_t IP3,uint8_t IP4)
{
	/*初始化 以太网SPI接口*/
	ENC_SPI_Init();
  	/* 初始化LWIP协议栈*/
	LwIP_Init(IP1,IP2,IP3,IP4);
}


/*
 * 函数名：ENC_SPI_Init
 * 描述  ：ENC28J60 SPI 接口初始化
 *            ------------------------------------
 *           |PA4-SPI1-NSS ：ENC28J60-CS          |
 *           |PA5-SPI1-SCK ：ENC28J60-SCK         |
 *           |PA6-SPI1-MISO：ENC28J60-SO          |
 *           |PA7-SPI1-MOSI：ENC28J60-SI          |
 *           |PA8          ：ENC28J60-INT (没用到)|
 *            ------------------------------------
*/

void ENC_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;


	// 使能SPI1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	// 使能GPIOA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// 连接SPI1---AF5, 配置SPI2(SCK/MISO/MOSI)推挽、下拉
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);	  // SPI1_SCK
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);	  // SPI1_MISO
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);	  // SPI1_MOSI

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |  GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置SPI FLASH CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			  // SPI1_CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);    //置高 分配给ENC28J60的 SPI1_NSS信号

	// MAC (Init)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// SPI1 配置:全双工、主机、8位帧、SCK上升沿采样(SCK极性为0、相位为0)、CS由软件控制、
	//			 SCK最大速度APB2/2=30MHz、高位在前
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	  // SPI1位APB2总线下，速度最大可达30MHz  //
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	  // SPI1位APB2总线下，速度最大可达30MHz  //

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	// 使能SPI1
	SPI_Cmd(SPI1, ENABLE);


}

/*
 * 函数名：ENC_SPI_ReadWrite
 * 描述  ：最底层的使用SPI1收发一节字数据
 * 输入  ：要写入的数据
 * 输出  : 接收到的数据
 * 调用  ：外部调用
 */
uint8_t ENC_SPI_ReadWrite(uint8_t dat)
{
	/* Loop while DR register in not emplty */
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI1, dat);

	/* Wait to receive a byte */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI1);
}


