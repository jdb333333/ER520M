/**********************************************************************************/

#include "spi_enc28j60.h"
#include "usart.h"

#include "err.h"
#include "etharp.h"
#include "netconfig.h"

__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */


void Ethernet_Init(uint8_t IP1,uint8_t IP2,uint8_t IP3,uint8_t IP4)
{
	/*��ʼ�� ��̫��SPI�ӿ�*/
	ENC_SPI_Init();
  	/* ��ʼ��LWIPЭ��ջ*/
	LwIP_Init(IP1,IP2,IP3,IP4);
}


/*
 * ��������ENC_SPI_Init
 * ����  ��ENC28J60 SPI �ӿڳ�ʼ��
 *            ------------------------------------
 *           |PA4-SPI1-NSS ��ENC28J60-CS          |
 *           |PA5-SPI1-SCK ��ENC28J60-SCK         |
 *           |PA6-SPI1-MISO��ENC28J60-SO          |
 *           |PA7-SPI1-MOSI��ENC28J60-SI          |
 *           |PA8          ��ENC28J60-INT (û�õ�)|
 *            ------------------------------------
*/

void ENC_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;


	// ʹ��SPI1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	// ʹ��GPIOAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// ����SPI1---AF5, ����SPI2(SCK/MISO/MOSI)���졢����
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

	// ����SPI FLASH CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			  // SPI1_CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_4);    //�ø� �����ENC28J60�� SPI1_NSS�ź�

	// MAC (Init)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// SPI1 ����:ȫ˫����������8λ֡��SCK�����ز���(SCK����Ϊ0����λΪ0)��CS��������ơ�
	//			 SCK����ٶ�APB2/2=30MHz����λ��ǰ
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	  // SPI1λAPB2�����£��ٶ����ɴ�30MHz  //
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	  // SPI1λAPB2�����£��ٶ����ɴ�30MHz  //

	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);

	// ʹ��SPI1
	SPI_Cmd(SPI1, ENABLE);


}

/*
 * ��������ENC_SPI_ReadWrite
 * ����  ����ײ��ʹ��SPI1�շ�һ��������
 * ����  ��Ҫд�������
 * ���  : ���յ�������
 * ����  ���ⲿ����
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


