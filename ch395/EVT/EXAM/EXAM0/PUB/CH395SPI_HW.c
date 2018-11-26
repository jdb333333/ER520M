/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395SPI_HW.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ Ӳ��SPI�������ӵ�Ӳ������� V1.0
*                     
*******************************************************************************/

/* Ӳ����غ궨�� */
/* �����е�Ӳ�����ӷ�ʽ����(ʵ��Ӧ�õ�·���Բ����޸��������弰�ӳ���) */
/* ��Ƭ��������    CH395оƬ������
      P1.4                 SCS
      P1.5                 SDI
      P1.6                 SDO
      P1.7                 SCK      */
      
sbit        P14         =          P1^4;
#define CH395_SPI_SCS              P14                               /* CH395��SCS���� */

sfr SPDR = 0x86;                                                     /* SPI���ݼĴ���0x86 */
sfr SPSR = 0xaa;                                                     /* SPI״̬�Ĵ���0xaa */
sfr SPCR = 0xd5;                                                     /* SPI���ƼĴ���0xd5 */

#define SPI_IF_TRANS    0x80                                         /* SPI�ֽڴ�����ɱ�־,��SPSR��λ7 */

#define CH395_INT_WIRE              INT1                            /* CH395��INT#���� */

#define xCH395CmdStart( )         {CH395_SPI_SCS = 0;}               /* ���ʼ */
#define xEndCH395Cmd()            {CH395_SPI_SCS = 1;}               /* �������*/

/*******************************************************************************
* Function Name  : mDelayuS
* Description    : ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : us---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelayuS( UINT8 us )
{
    while( us -- );                                                  /* MCS51@24MHz */
}

/*******************************************************************************
* Function Name  : mDelaymS
* Description    : ��ʱָ������ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ
* Input          : ms---��ʱʱ��ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void mDelaymS( UINT8 ms )
{
    while( ms -- ) 
    {
        mDelayuS( 250 );
        mDelayuS( 250 );
        mDelayuS( 250 );
        mDelayuS( 250 );
    }
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
    xEndCH395Cmd();
   /* �����Ӳ��SPI�ӿ�,��ô��ʹ��mode3(CPOL=1&CPHA=1)��mode0(CPOL=0&CPHA=0),
    CH395��ʱ�������ز�������,�½������,����λ�Ǹ�λ��ǰ */
    SPCR = 0x5C;  
    /* ����˫��I/O����ģ��SPI�ӿ�,��ô�����ڴ�����SPI_SCS,SPI_SCK,SPI_SDIΪ�������,
       SPI_SDOΪ���뷽�� */
    /* ����SPIģʽ3, DORD=0(MSB first), CPOL=1, CPHA=1, CH395Ҳ֧��SPIģʽ0 */
    SPSR |= 0x08;
        
}

/*******************************************************************************
* Function Name  : Spi395Exchange
* Description    : Ӳ��SPI���������8��λ����
* Input          : d---��Ҫ���뵽CH395������
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 Spi395Exchange( UINT8 d )  
{  
    /* Ϊ������ٶ�,���Խ����ӳ������ɺ��Լ����ӳ�����ò�� */
    SPDR = d;                                                    /* �Ƚ�����д��SPI���ݼĴ���,Ȼ���ѯSPI״̬�Ĵ����Եȴ�SPI�ֽڴ������ */
    while( ( SPSR & SPI_IF_TRANS ) == 0 );                       /* ��ѯSPI״̬�Ĵ����Եȴ�SPI�ֽڴ������ */
    SPSR &= ~ SPI_IF_TRANS;                                      /* ���SPI�ֽڴ�����ɱ�־,�еĵ�Ƭ�����Զ���� */
    return( SPDR );                                              /* �Ȳ�ѯSPI״̬�Ĵ����Եȴ�SPI�ֽڴ������,Ȼ���SPI���ݼĴ����������� */
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
    Spi395Exchange(cmd);                                             /* SPI���������� */
    mDelayuS(2);                                                     /* ��Ҫ��ʱ,��ʱ1.5uSȷ����д���ڲ�С��1.5uS */
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
    Spi395Exchange(mdata);                                           /* SPI�������� */
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
    i = Spi395Exchange(0xff);                                        /* SPI������ */
    return i;
}

/**************************** endfile *************************************/
 