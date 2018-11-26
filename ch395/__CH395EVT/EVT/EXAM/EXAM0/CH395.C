/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395������ʾ����
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP Э����ӿ�
MSC51 ��ʾ������ʾ4��socket �ֱ�ʹ��UDP,TCP Client,TCP Server�Լ�IP RAW ,��Ƭ��
�յ����ݺ󣬰�λȡ�����ϴ���MCS51@24MHZ,KEIL 4
**********************************************************************************/
/* ͷ�ļ�����*/
#include <reg52.h>
#include "stdio.h"
#include "string.h"
#include "../PUB/CH395INC.H"
#include "CH395.H"

/**********************************************************************************/
/*
CH395_OP_INTERFACE_MODE����Ϊ1-5
1��Ӳ�����߲������ӷ�ʽ
2�����ģ�Ⲣ�����ӷ�ʽ
3: Ӳ��SPI���ӷ�ʽ
4: ���ģ��SPI��ʽ
5: Ӳ���첽�������ӷ�ʽ
*/
#define   CH395_OP_INTERFACE_MODE             3                     
#if   (CH395_OP_INTERFACE_MODE == 1)                                 /* SEL = 0, TX = 1*/
#include "../PUB/CH395PARA_HW.C"                                           
#elif (CH395_OP_INTERFACE_MODE == 2)                                 /* SEL = 0, TX = 1*/
#include "../PUB/CH395PARA_SW.C"                                            
#elif (CH395_OP_INTERFACE_MODE == 3)                                 /* SEL = 1, TX = 0*/
#include "../PUB/CH395SPI_HW.C"
#elif (CH395_OP_INTERFACE_MODE == 4)                                 /* SEL = 1, TX = 0*/
#include "../PUB/CH395SPI_SW.C"
#elif (CH395_OP_INTERFACE_MODE == 5)                                 /* SEL = 1, TX = 1*/
#include "../PUB/CH395UART.C"
#else
#error "Please Select Correct Communication Interface "
#endif

/**********************************************************************************/
/* ���������ļ� */
#include "../PUB/CH395CMD.C"

#define     CH395_DEBUG                0
/* ���ñ������� */
UINT8 xdata MyBuffer[4][100];                                        /* ���ݻ����� ,�����ݻ��������ȿɸ��ݵ�Ƭ����Դ�ʵ�����,�����ͨѶ�ٶ�*/
struct _SOCK_INF xdata SockInf[4];                                   /* ����Socket��Ϣ */
struct _CH395_SYS xdata CH395Inf;                                    /* ����CH395��Ϣ */

/* CH395��ض��� */
const UINT8 CH395IPAddr[4] = {192,168,1,10};                         /* CH395IP��ַ */
const UINT8 CH395GWIPAddr[4] = {192,168,1,1};                        /* CH395���� */
const UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395�������� */

/* socket ��ض���,Ϊ������ʾ��socket0-2������Դ�˿ں�Ŀ�Ķ˿�,socket3 */
/* ΪIP RAW����Ҫ����IP Э���ֶε�Э������ */
const UINT8  Socket0DesIP[4] = {192,168,1,100};                      /* Socket 0Ŀ��IP��ַ */
const UINT16 Socket0DesPort = 1000;                                  /* Socket 0Ŀ�Ķ˿� */
const UINT16 Socket0SourPort = 5000;                                 /* Socket 0Դ�˿� */

const UINT8  Socket1DesIP[4] = {192,168,1,100};                      /* Socket 1Ŀ��IP��ַ */
const UINT16 Socket1SourPort = 6000;                                 /* Socket 1Դ�˿� */

const UINT8  Socket2DesIP[4] = {192,168,1,100};                      /* Socket 2Ŀ��IP��ַ */
const UINT16 Socket2DesPort = 3000;                                  /* Socket 2Ŀ�Ķ˿� */
const UINT16 Socket2SourPort = 7000;                                 /* Socket 2Դ�˿� */

const UINT8  Socket3DesIP[4] = {192,168,1,100};                      /* Socket 3Ŀ��IP��ַ */
const UINT8  IPRawProto = 0xE2; 
                                                                     /* IP��Э������ */

/**********************************************************************************
* Function Name  : mStopIfError
* Description    : ����ʹ�ã���ʾ������룬��ͣ��
* Input          : iError   �������
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{
    if (iError == CMD_ERR_SUCCESS) return;                           /* �����ɹ� */
#if CH395_DEBUG
    printf("Error: %02X\n", (UINT16)iError);                         /* ��ʾ���� */
#endif
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : ��ʼ��CH395Inf����
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitCH395InfParam(void)
{
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* ��CH395Infȫ������*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* ��IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* ������IP��ַд��CH395Inf�� */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* ����������д��CH395Inf�� */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : ��ʼ��socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
    memset(&SockInf[0],0,sizeof(SockInf[0]));                        /* ��SockInf[0]ȫ������*/
    memcpy(SockInf[0].IPAddr,Socket0DesIP,sizeof(Socket0DesIP));     /* ��Ŀ��IP��ַд�� */
    SockInf[0].DesPort = Socket0DesPort;                             /* Ŀ�Ķ˿� */
    SockInf[0].SourPort = Socket0SourPort;                           /* Դ�˿� */
    SockInf[0].ProtoType = PROTO_TYPE_UDP;                           /* UDPģʽ */

    memset(&SockInf[1],0,sizeof(SockInf[1]));                        /* ��SockInf[1]ȫ������*/
    SockInf[1].SourPort = Socket1SourPort;                           /* Դ�˿� */
    SockInf[1].ProtoType = PROTO_TYPE_TCP;                           /* TCPģʽ */
    SockInf[1].TcpMode = TCP_SERVER_MODE;                            /* TCP������ģʽ */

    memset(&SockInf[2],0,sizeof(SockInf[2]));                        /* ��SockInf[2]ȫ������*/
    memcpy(SockInf[2].IPAddr,Socket2DesIP,sizeof(Socket2DesIP));     /* ��Ŀ��IP��ַд�� */
    SockInf[2].DesPort = Socket2DesPort;                             /* Ŀ�Ķ˿� */
    SockInf[2].SourPort = Socket2SourPort;                           /* Դ�˿� */
    SockInf[2].ProtoType = PROTO_TYPE_TCP;                           /* TCPģʽ */
    SockInf[2].TcpMode = TCP_CLIENT_MODE;                            /* TCP�ͻ���ģʽ */

    memset(&SockInf[3],0,sizeof(SockInf[3]));                        /* ��SockInf[3]ȫ������*/
    memcpy(SockInf[3].IPAddr,Socket3DesIP,sizeof(Socket3DesIP));     /* ��Ŀ��IP��ַд�� */
    SockInf[3].ProtoType = PROTO_TYPE_IP_RAW;                        /* IPRAW */
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : ����CH395 socket ��������ʼ������socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
    UINT8 i;

    /* socket 0ΪUDPģʽ */
    CH395SetSocketDesIP(0,SockInf[0].IPAddr);                        /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketProtType(0,PROTO_TYPE_UDP);                        /* ����socket 0Э������ */
    CH395SetSocketDesPort(0,SockInf[0].DesPort);                     /* ����socket 0Ŀ�Ķ˿� */
    CH395SetSocketSourPort(0,SockInf[0].SourPort);                   /* ����socket 0Դ�˿� */
    i = CH395OpenSocket(0);                                          /* ��socket 0 */
    mStopIfError(i);

    /* socket 1ΪTCP Serverģʽ,��������Ŀ��IP��Դ�˿� */
    CH395SetSocketProtType(1,PROTO_TYPE_TCP);                        /* ����socket 1Э������ */
    CH395SetSocketSourPort(1,SockInf[1].SourPort);                   /* ����socket 1Դ�˿� */
    i = CH395OpenSocket(1);                                          /* ��socket 1 */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
    i = CH395TCPListen (1);                                          /* ��ʼ���� */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */

    /* socket 2ΪTCP Clientģʽ */
    CH395SetSocketDesIP(2,SockInf[2].IPAddr);                        /* ����socket 2Ŀ��IP��ַ */
    CH395SetSocketProtType(2,PROTO_TYPE_TCP);                        /* ����socket 2Э������ */
    CH395SetSocketDesPort(2,SockInf[2].DesPort);                     /* ����socket 2Ŀ�Ķ˿� */
    CH395SetSocketSourPort(2,SockInf[2].SourPort);                   /* ����socket 2Դ�˿� */
    i = CH395OpenSocket(2);                                          /* ��socket 2 */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
    i = CH395TCPConnect(2);                                          /* ��ʼ���� */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ�,���ʧ�ܣ���socket��ر� */

    /* socket 3ΪIP RAWģʽ */
    CH395SetSocketDesIP(3,SockInf[3].IPAddr);                        /* ����socket 3Ŀ��IP��ַ */
    CH395SetSocketProtType(3,PROTO_TYPE_IP_RAW);                     /* ����IP RAWģʽ�µ�IP��Э���ֶ� */
    CH395SetSocketIPRAWProto(3,IPRawProto);                          /* ����Э���ֶ� */
    i = CH395OpenSocket(3);                                          /* ��socket 3 */
    mStopIfError(i);                                                 /* ����Ƿ�ɹ� */
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket �ж�,��ȫ���ж��б�����
* Input          : sockindex  Socket����
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
    UINT8  sock_int_socket;
    UINT8  i;
    UINT16 len;
    UINT16 tmp;
    UINT8 idata buf[10];

    sock_int_socket = CH395GetSocketInt(sockindex);                  /* ��ȡsocket ���ж�״̬ */
    if(sock_int_socket & SINT_STAT_SENBUF_FREE)                      /* ���ͻ��������У����Լ���д��Ҫ���͵����� */
    {

    }
    if(sock_int_socket & SINT_STAT_SEND_OK)                          /* ��������ж� */
    {
    }
    if(sock_int_socket & SINT_STAT_RECV)                             /* �����ж� */
    {
        len = CH395GetRecvLength(sockindex);                         /* ��ȡ��ǰ�����������ݳ��� */
#if CH395_DEBUG
        printf("receive len = %d\n"	,len);
#endif
        if(len == 0)return;
        if(len > 100)len = 100;                                      /* ���ͻ��������Ϊ2048 */
        CH395GetRecvData(sockindex,len,MyBuffer[sockindex]);         /* ��ȡ���� */
        for(tmp =0; tmp < len; tmp++)                                /* ���������ݰ�λȡ�� */
        {
            MyBuffer[sockindex][tmp] = ~MyBuffer[sockindex][tmp];
        }
        CH395SendData(sockindex,MyBuffer[sockindex],len);

   }
   if(sock_int_socket & SINT_STAT_CONNECT)                            /* �����жϣ�����TCPģʽ����Ч*/
   {
#if CH395_DEBUG
       printf("Tcp Connect\n");
#endif
       if(SockInf[sockindex].TcpMode == TCP_SERVER_MODE)              /* ���socket Ϊ������ģʽ���û����Ի�ȡԶ�˵�IP�Ͷ˿�*/
       {
           CH395CMDGetRemoteIPP(sockindex,buf);
           tmp = (UINT16)(buf[5]<<8) + buf[4];
#if CH395_DEBUG
	       printf("IP address = %d.%d.%d.%d\n",(UINT16)buf[0],(UINT16)buf[1],(UINT16)buf[2],(UINT16)buf[3]);                    
	       printf("Port = %d\n",tmp);    
#endif

       }
   }
   /*
   **�����Ͽ������жϺͳ�ʱ�ж�ʱ��CH395Ĭ���������ڲ������رգ��û�����Ҫ�Լ��رո�Socket����������óɲ������ر�Socket��Ҫ����
   **SOCK_CTRL_FLAG_SOCKET_CLOSE��־λ��Ĭ��Ϊ0��������ñ�־Ϊ1��CH395�ڲ�����Socket���йرմ����û��������жϺͳ�ʱ�ж�ʱ����
   **CH395CloseSocket������Socket���йرգ�������ر����SocketһֱΪ���ӵ�״̬����ʵ���Ѿ��Ͽ������Ͳ�����ȥ�����ˡ�
   */
   if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* �Ͽ��жϣ�����TCPģʽ����Ч */
   {
		//i = CH395CloseSocket(sockindex);                             
		//mStopIfError(i);
   }
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* ��ʱ�жϣ�����TCPģʽ����Ч */
   {
		//i = CH395CloseSocket(sockindex);
		//mStopIfError(i);
       if(SockInf[sockindex].TcpMode == TCP_CLIENT_MODE)             
       {
           mDelaymS(200);                                            /* ��ʱ200MS���ٴ����ԣ�û�б�Ҫ����Ƶ������ */
           i = CH395OpenSocket(sockindex);
           mStopIfError(i);
           CH395TCPConnect(sockindex);                               /* ��ʼ���� */
           mStopIfError(i);
       }
    }
}

/********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395ȫ���жϺ���
* Input          : None
* Output         : None
* Return         : None
*********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;
   UINT8  buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* ���ɴ��жϣ���ȡ���ɴ���Ϣ */
    {
        CH395CMDGetUnreachIPPT(buf);           
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* ����IP��ͻ�жϣ����������޸�CH395�� IP������ʼ��CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* ����PHY�ı��ж�*/
    {
#if CH395_DEBUG
        printf("Init status : GINT_STAT_PHY_CHANGE\n");
#endif
	}
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* ����socket 0�ж�*/
    }
    if(init_status & GINT_STAT_SOCK1)                               
    {
        CH395SocketInterrupt(1);                                     /* ����socket 1�ж�*/
    }
    if(init_status & GINT_STAT_SOCK2)                                
    {
        CH395SocketInterrupt(2);                                     /* ����socket 2�ж�*/
    }
    if(init_status & GINT_STAT_SOCK3)                                
    {
        CH395SocketInterrupt(3);                                     /* ����socket 3�ж�*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* ����socket 4�ж�*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* ����socket 5�ж�*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* ����socket 6�ж�*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* ����socket 7�ж�*/
    }
}

/**********************************************************************************
* Function Name  : CH395Init
* Description    : ����CH395��IP,GWIP,MAC�Ȳ���������ʼ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
**********************************************************************************/
UINT8  CH395Init(void)
{
    UINT8 i;
    
    i = CH395CMDCheckExist(0x65);   
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* �����������޷�ͨ������0XFA */
                                                                     /* ����0XFAһ��ΪӲ��������߶�дʱ�򲻶� */
#if (CH395_OP_INTERFACE_MODE == 5)                                   
#ifdef UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* ���ò����� */   
    mDelaymS(1);
    SetMCUBaudRate();
#endif
#endif
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* ����CH395��IP��ַ */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* �������ص�ַ */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* �����������룬Ĭ��Ϊ255.255.255.0*/   
    mDelaymS(10);
    i = CH395CMDInitCH395();                                         /* ��ʼ��CH395оƬ */
    return i;
}

/**********************************************************************************
* Function Name  : mInitSTDIO
* Description    : ���ڳ�ʼ��,������ʹ��
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void mInitSTDIO( void )
{
    SCON = 0x50;
    PCON = 0x80;
    TMOD = 0x21;
    TH1 = 0xf3;                                                      /* 24MHz����, 9600bps */
    TR1 = 1;
    TI = 1;
}

/**********************************************************************************
* Function Name  : main
* Description    : main������
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
int main(void)
{
    UINT8 i;
    mDelaymS(100);
    mInitSTDIO();                                                    /* ��ʱ100���� */ 
#if CH395_DEBUG
    printf("CH395EVT Test Demo\n");
#endif
    CH395_PORT_INIT();
    InitCH395InfParam();                                             /* ��ʼ��CH395��ر��� */
    i = CH395Init();                                                 /* ��ʼ��CH395оƬ */
    mStopIfError(i);                                                                     
  
    while(1)
    {                                                                /* �ȴ���̫�����ӳɹ�*/
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                     /* ��ѯCH395�Ƿ����� */
       {
           mDelaymS(200);                                            /* δ������ȴ�200MS���ٴβ�ѯ */
       }
       else 
       {
#if CH395_DEBUG
           printf("CH395 Connect Ethernet\n");                       /* CH395оƬ���ӵ���̫������ʱ������ж� */
#endif
           break;
       }
    }
    InitSocketParam();                                                /* ��ʼ��socket��ر��� */
    CH395SocketInitOpen();
    while(1)
    {
       if(CH395_INT_WIRE == 0)CH395GlobalInterrupt();
    }
}


