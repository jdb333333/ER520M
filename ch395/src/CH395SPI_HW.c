/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395SPI_HW.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ Ӳ��SPI�������ӵ�Ӳ������� V1.0
*                     
*******************************************************************************/
#include "CH395INC.h"


static int fpEutronSpi;

#define EUTRON_SPI_IOCTL_BASE '&'
#define EUTRON_SPI_CS_ENABLE _IO(EUTRON_SPI_IOCTL_BASE, 0x0b)
#define EUTRON_SPI_CS_DISABL _IO(EUTRON_SPI_IOCTL_BASE, 0x0c)

#define xCH395CmdStart();    ioctl(fpEutronSpi, EUTRON_SPI_CS_ENABLE, NULL);
#define xEndCH395Cmd();      ioctl(fpEutronSpi, EUTRON_SPI_CS_DISABL, NULL);

UINT8 CH395_INT_WIRE; 
void eutron_dev_init(void);

/*******************************************************************************
* Function Name  : mDelayuS
* Description    : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : us---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelayuS(UINT8 us)
{
    //while(us --);                                                    /* MCS51@24MHz */
	usleep(us);
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : ms---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS(UINT8 ms)
{
	usleep(ms*1000);
}

/******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : Ӳ����ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void)
{
    fpEutronSpi = open("/dev/eutron_spi", O_RDWR);
    if (fpEutronSpi < 0)
    { 
        printf("---can not open eutron_spi \n"); 
        return; 
    }
	else
	    { 
        printf("== open eutron_spi \n"); 
    } 
	
    xEndCH395Cmd();
	eutron_dev_init();
}

/******************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : ��CH395д����
* Input          : cmd 8λ��������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(UINT8 cmd)                                          
{                                                                    
    xEndCH395Cmd();                                                  /* ��ֹCSԭ��Ϊ�ͣ��Ƚ�CD�ø� */
    xCH395CmdStart( );                                               /* ���ʼ��CS���� */
	write(fpEutronSpi, &cmd, 1);
    //mDelayuS(2);                                                     /* ��Ҫ��ʱ,��ʱ1.5uSȷ����д���ڲ�С��1.5uS */
}

/******************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395д����
* Input          : mdata 8λ����
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(UINT8 mdata)
{   
	write(fpEutronSpi, &mdata, 1);
}

/*******************************************************************************
* Function Name  : xReadCH395Data
* Description    : ��CH395������
* Input          : None
* Output         : None
* Return         : 8λ����
*******************************************************************************/
UINT8   xReadCH395Data( void )                                                  
{
    UINT8 i;
	read(fpEutronSpi, &i, 1);
    return i;
}

static int fpEutronDev;
UINT8 eutron_devKey;
void eutron_dev_init(void)
{
    fpEutronDev = open("/dev/eutron_dev", O_RDWR);
    if (fpEutronDev < 0)
    { 
        printf("---can not open eutron_dev \n"); 
        return; 
    }
	else
	    { 
        printf("== open eutron_dev \n"); 
        return; 
    } 

}
void eutron_dev_read(void)
{
	UINT8 eutron_devIO;
	if(read(fpEutronDev, &eutron_devIO, 1)<0)
		eutron_devIO = 0x40;

	eutron_devKey	= eutron_devIO & (1<<5);
	CH395_INT_WIRE	= eutron_devIO & (1<<6); //eth�жϣ��͵�ƽ��Ч
}
/**************************** endfile *************************************/
 
