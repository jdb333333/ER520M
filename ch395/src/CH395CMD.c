/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395CMD.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ����ӿ��ļ�
*                      
*******************************************************************************/

/* ͷ�ļ�����*/
#include "CH395INC.h"
#include "CH395CMD.h"
#include "CH395UART.h"

/********************************************************************************
* Function Name  : CH395CMDReset
* Description    : ��λCH395оƬ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDReset(void)
{
    xWriteCH395Cmd(CMD00_RESET_ALL);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDSleep
* Description    : ʹCH395����˯��״̬
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSleep(void)
{
    xWriteCH395Cmd(CMD00_ENTER_SLEEP);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSleep
* Description    : ��ȡоƬ�Լ��̼��汾�ţ�1�ֽڣ�����λ��ʾоƬ�汾��
                   ����λ��ʾ�̼��汾
* Input          : None
* Output         : None
* Return         : 1�ֽ�оƬ���̼��汾��
*******************************************************************************/
UINT8 CH395CMDGetVer(void)
{
	UINT8 i;
	xWriteCH395Cmd(CMD01_GET_IC_VER);
	i = xReadCH395Data();
	xEndCH395Cmd();
	return i;
}

/********************************************************************************
* Function Name  : CH395CMDCheckExist
* Description    : ����������ڲ���Ӳ���Լ��ӿ�ͨѶ
* Input          : testdata 1�ֽڲ�������
* Output         : None
* Return         : Ӳ��OK������ testdata��λȡ��
*******************************************************************************/
UINT8 CH395CMDCheckExist(UINT8 testdata)
{
    UINT8 i;

    xWriteCH395Cmd(CMD11_CHECK_EXIST);
    xWriteCH395Data(testdata);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetPHY
* Description    : ����PHY����Ҫ����CH395 PHYΪ100/10M ����ȫ˫����˫����CH395Ĭ
                    Ϊ�Զ�Э�̡�
* Input          : phystat �ο�PHY �������/״̬
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetPHY(UINT8 phystat)
{
    xWriteCH395Cmd(CMD10_SET_PHY);
    xWriteCH395Data(phystat);
	
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395CMDGetPHYStatus
* Description    : ��ȡPHY��״̬
* Input          : None
* Output         : None
* Return         : ��ǰCH395PHY״̬���ο�PHY����/״̬����
*******************************************************************************/
UINT8 CH395CMDGetPHYStatus(void)
{
    UINT8 i;

    xWriteCH395Cmd(CMD01_GET_PHY_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : ��ȡȫ���ж�״̬���յ�������CH395�Զ�ȡ���жϣ�0x43�����°汾ʹ��
* Input          : None
* Output         : None
* Return         : ���ص�ǰ��ȫ���ж�״̬
*******************************************************************************/
UINT8 CH395CMDGetGlobIntStatus(void)
{
    UINT8 init_status;

    xWriteCH395Cmd(CMD01_GET_GLOB_INT_STATUS);
    init_status = xReadCH395Data();
    xEndCH395Cmd();
    return  init_status;
}

/********************************************************************************
* Function Name  : CH395CMDInitCH395
* Description    : ��ʼ��CH395оƬ��
* Input          : None
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8 CH395CMDInitCH395(void)
{
    UINT8 i = 0;
    UINT8 s = 0;

    xWriteCH395Cmd(CMD0W_INIT_CH395);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(10);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�,��������Ҫ500MS����ִ����� */
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395CMDSetUartBaudRate
* Description    : ����CH395���ڲ����ʣ����ڴ���ģʽ����Ч
* Input          : baudrate ���ڲ�����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetUartBaudRate(UINT32 baudrate)
{
	UINT8 i;
    xWriteCH395Cmd(CMD31_SET_BAUDRATE);
    xWriteCH395Data((UINT8)baudrate);
    xWriteCH395Data((UINT8)((UINT16)baudrate >> 8));
    xWriteCH395Data((UINT8)(baudrate >> 16));
	i = xReadCH395Data();
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetCmdStatus
* Description    : ��ȡ����ִ��״̬��ĳЩ������Ҫ�ȴ�����ִ�н��
* Input          : None
* Output         : None
* Return         : ������һ������ִ��״̬
*******************************************************************************/
UINT8 CH395GetCmdStatus(void)
{
    UINT8 i;

    xWriteCH395Cmd(CMD01_GET_CMD_STATUS);
    i = xReadCH395Data();
    xEndCH395Cmd();
    return i;
}

/********************************************************************************
* Function Name  : CH395CMDSetIPAddr
* Description    : ����CH395��IP��ַ
* Input          : ipaddr ָIP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetIPAddr(UINT8 *ipaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_IP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetGWIPAddr
* Description    : ����CH395������IP��ַ
* Input          : ipaddr ָ������IP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetGWIPAddr(UINT8 *gwipaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_GWIP_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*gwipaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMASKAddr
* Description    : ����CH395���������룬Ĭ��Ϊ255.255.255.0
* Input          : maskaddr ָ���������ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMASKAddr(UINT8 *maskaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD40_SET_MASK_ADDR);
    for(i = 0; i < 4;i++)xWriteCH395Data(*maskaddr++);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetMACAddr
* Description    : ����CH395��MAC��ַ��
* Input          : amcaddr MAC��ַָ��
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMACAddr(UINT8 *amcaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD60_SET_MAC_ADDR);
    for(i = 0; i < 6;i++)xWriteCH395Data(*amcaddr++);
    xEndCH395Cmd();
    mDelaymS(100); 
}

/********************************************************************************
* Function Name  : CH395CMDGetMACAddr
* Description    : ��ȡCH395��MAC��ַ��
* Input          : amcaddr MAC��ַָ��
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetMACAddr(UINT8 *amcaddr)
{
    UINT8 i;

    xWriteCH395Cmd(CMD06_GET_MAC_ADDR);
    for(i = 0; i < 6;i++)*amcaddr++ = xReadCH395Data();
    xEndCH395Cmd();
 }

/*******************************************************************************
* Function Name  : CH395CMDSetMACFilt
* Description    : ����MAC���ˡ�
* Input          : filtype �ο� MAC����
                   table0 Hash0
                   table1 Hash1
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetMACFilt(UINT8 filtype,UINT32 table0,UINT32 table1)
{
    xWriteCH395Cmd(CMD90_SET_MAC_FILT);
    xWriteCH395Data(filtype);
    xWriteCH395Data((UINT8)table0);
    xWriteCH395Data((UINT8)((UINT16)table0 >> 8));
    xWriteCH395Data((UINT8)(table0 >> 16));
    xWriteCH395Data((UINT8)(table0 >> 24));

    xWriteCH395Data((UINT8)table1);
    xWriteCH395Data((UINT8)((UINT16)table1 >> 8));
    xWriteCH395Data((UINT8)(table1 >> 16));
    xWriteCH395Data((UINT8)(table1 >> 24));
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetUnreachIPPT
* Description    : ��ȡ���ɴ���Ϣ (IP,Port,Protocol Type)
* Input          : list �����ȡ���Ĳ��ɴ�
                        ��1���ֽ�Ϊ���ɴ���룬��ο� ���ɴ����(CH395INC.H)
                        ��2���ֽ�ΪIP��Э������
                        ��3-4�ֽ�Ϊ�˿ں�
                        ��4-8�ֽ�ΪIP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetUnreachIPPT(UINT8 *list)
{
    UINT8 i;

    xWriteCH395Cmd(CMD08_GET_UNREACH_IPPORT);
    for(i = 0; i < 8; i++)
    {
        *list++ = xReadCH395Data();
    }   
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetRemoteIPP
* Description    : ��ȡԶ�˵�IP�Ͷ˿ڵ�ַ��һ����TCP Serverģʽ��ʹ��
* Input          : sockindex Socket����
                   list ����IP�Ͷ˿�
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDGetRemoteIPP(UINT8 sockindex,UINT8 *list)
{
    UINT8 i;

    xWriteCH395Cmd(CMD06_GET_REMOT_IPP_SN);
    xWriteCH395Data(sockindex);
    for(i = 0; i < 6; i++)
    {
        *list++ = xReadCH395Data();
    }   
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketDesIP
* Description    : ����socket n��Ŀ��IP��ַ
* Input          : sockindex Socket����
                   ipaddr ָ��IP��ַ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesIP(UINT8 sockindex,UINT8 *ipaddr)
{
    xWriteCH395Cmd(CMD50_SET_IP_ADDR_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xWriteCH395Data(*ipaddr++);
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketProtType
* Description    : ����socket ��Э������
* Input          : sockindex Socket����
                   prottype Э�����ͣ���ο� socketЭ�����Ͷ���(CH395INC.H)
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketProtType(UINT8 sockindex,UINT8 prottype)
{
    xWriteCH395Cmd(CMD20_SET_PROTO_TYPE_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(prottype);
    xEndCH395Cmd();
}

/*******************************************************************************

* Function Name  : CH395SetSocketDesPort
* Description    : ����socket n��Э������
* Input          : sockindex Socket����
                   desprot 2�ֽ�Ŀ�Ķ˿�
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketDesPort(UINT8 sockindex,UINT16 desprot)
{
    xWriteCH395Cmd(CMD30_SET_DES_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)desprot);
    xWriteCH395Data((UINT8)(desprot >> 8));
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395SetSocketSourPort
* Description    : ����socket n��Э������
* Input          : sockindex Socket����
                   desprot 2�ֽ�Դ�˿�
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSourPort(UINT8 sockindex,UINT16 surprot)
{
    xWriteCH395Cmd(CMD30_SET_SOUR_PORT_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)surprot);
    xWriteCH395Data((UINT8)(surprot>>8));
    xEndCH395Cmd();
}

/******************************************************************************
* Function Name  : CH395SetSocketIPRAWProto
* Description    : IPģʽ�£�socket IP��Э���ֶ�
* Input          : sockindex Socket����
                   prototype IPRAWģʽ1�ֽ�Э���ֶ�
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketIPRAWProto(UINT8 sockindex,UINT8 prototype)
{
    xWriteCH395Cmd(CMD20_SET_IPRAW_PRO_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(prototype);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395EnablePing
* Description    : ����/�ر� PING
* Input          : enable : 1  ����PING
                          ��0  �ر�PING
* Output         : None
* Return         : None
*******************************************************************************/
void CH395EnablePing(UINT8 enable)
{
    xWriteCH395Cmd(CMD01_PING_ENABLE);
    xWriteCH395Data(enable);
    xEndCH395Cmd();
}


/********************************************************************************
* Function Name  : CH395SendData
* Description    : ���ͻ�����д����
* Input          : sockindex Socket����
                   databuf  ���ݻ�����
                   len   ����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SendData(UINT8 sockindex,UINT8 *databuf,UINT16 len)
{
    UINT16 i;

    xWriteCH395Cmd(CMD30_WRITE_SEND_BUF_SN);
    xWriteCH395Data((UINT8)sockindex);
    xWriteCH395Data((UINT8)len);
    xWriteCH395Data((UINT8)(len>>8));
   
    for(i = 0; i < len; i++)
    {
        xWriteCH395Data(*databuf++);
    }
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : ��ȡ���ջ���������
* Input          : sockindex Socket����
* Output         : None
* Return         : ���ؽ��ջ�������Ч����
*******************************************************************************/
UINT16 CH395GetRecvLength(UINT8 sockindex)
{
    UINT16 i;

    xWriteCH395Cmd(CMD12_GET_RECV_LEN_SN);
    xWriteCH395Data((UINT8)sockindex);
    i = xReadCH395Data();
    i = (UINT16)(xReadCH395Data()<<8) + i;
    xEndCH395Cmd();
    return i;
}

/*******************************************************************************
* Function Name  : CH395ClearRecvBuf
* Description    : ������ջ�����
* Input          : sockindex Socket����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395ClearRecvBuf(UINT8 sockindex)
{
    xWriteCH395Cmd(CMD10_CLEAR_RECV_BUF_SN);
    xWriteCH395Data((UINT8)sockindex);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395GetRecvLength
* Description    : ��ȡ���ջ���������
* Input          : sockindex Socket����
                   len   ����
                   pbuf  ������
* Output         : None
* Return         : None
*******************************************************************************/
void CH395GetRecvData(UINT8 sockindex,UINT16 len,UINT8 *pbuf)
{
    UINT16 i;
    if(!len)return;
    xWriteCH395Cmd(CMD30_READ_RECV_BUF_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data((UINT8)len);
    xWriteCH395Data((UINT8)(len>>8));
    mDelayuS(1);
    for(i = 0; i < len; i++)
    {
       *pbuf = xReadCH395Data();
       pbuf++;
    }   
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetRetryCount
* Description    : �������Դ���
* Input          : count ����ֵ�����Ϊ20��
* Output         : None
* Return         : None
********************************************************************************/
void CH395CMDSetRetryCount(UINT8 count)
{
    xWriteCH395Cmd(CMD10_SET_RETRAN_COUNT);
    xWriteCH395Data(count);
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDSetRetryPeriod
* Description    : ������������
* Input          : period �������ڵ�λΪ���룬���1000ms
* Output         : None
* Return         : None
*******************************************************************************/
void CH395CMDSetRetryPeriod(UINT16 period)
{
    xWriteCH395Cmd(CMD10_SET_RETRAN_COUNT);
    xWriteCH395Data((UINT8)period);
    xWriteCH395Data((UINT8)(period>>8));
    xEndCH395Cmd();
}

/********************************************************************************
* Function Name  : CH395CMDGetSocketStatus
* Description    : ��ȡsocket
* Input          : None
* Output         : socket n��״̬��Ϣ����1�ֽ�Ϊsocket �򿪻��߹ر�
                   ��2�ֽ�ΪTCP״̬
* Return         : None
*******************************************************************************/
void CH395CMDGetSocketStatus(UINT8 sockindex,UINT8 *status)
{
    xWriteCH395Cmd(CMD12_GET_SOCKET_STATUS_SN);
    xWriteCH395Data(sockindex);
    *status++ = xReadCH395Data();
    *status++ = xReadCH395Data();
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395OpenSocket
* Description    : ��socket����������Ҫ�ȴ�ִ�гɹ�
* Input          : sockindex Socket����
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8  CH395OpenSocket(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_OPEN_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395CloseSocket
* Description    : �ر�socket��
* Input          : sockindex Socket����
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8  CH395CloseSocket(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_CLOSE_SOCKET_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395TCPConnect
* Description    : TCP���ӣ�����TCPģʽ����Ч����������Ҫ�ȴ�ִ�гɹ�
* Input          : sockindex Socket����
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8 CH395TCPConnect(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_TCP_CONNECT_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/******************************************************************************
* Function Name  : CH395TCPListen
* Description    : TCP����������TCPģʽ����Ч����������Ҫ�ȴ�ִ�гɹ�
* Input          : sockindex Socket����
* Output         : None
* Return         : ����ִ�н��
*******************************************************************************/
UINT8 CH395TCPListen(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_TCP_LISTEN_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/********************************************************************************
* Function Name  : CH395TCPDisconnect
* Description    : TCP�Ͽ�������TCPģʽ����Ч����������Ҫ�ȴ�ִ�гɹ�
* Input          : sockindex Socket����
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 CH395TCPDisconnect(UINT8 sockindex)
{
    UINT8 i = 0;
    UINT8 s = 0;
    xWriteCH395Cmd(CMD1W_TCP_DISNCONNECT_SN);
    xWriteCH395Data(sockindex);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(5);                                                 /* ��ʱ��ѯ������2MS����*/
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/*******************************************************************************
* Function Name  : CH395GetSocketInt
* Description    : ��ȡsocket n���ж�״̬
* Input          : sockindex   socket����
* Output         : None
* Return         : �ж�״̬
*******************************************************************************/
UINT8 CH395GetSocketInt(UINT8 sockindex)
{
    UINT8 intstatus;
    xWriteCH395Cmd(CMD11_GET_INT_STATUS_SN);
    xWriteCH395Data(sockindex);
    mDelayuS(2);
    intstatus = xReadCH395Data();
    xEndCH395Cmd();
    return intstatus;
}

/*******************************************************************************
* Function Name  : CH395CRCRet6Bit
* Description    : �Զಥ��ַ����CRC���㣬��ȡ��6λ��
* Input          : mac_addr   MAC��ַ
* Output         : None
* Return         : ����CRC32�ĸ�6λ
*******************************************************************************/
UINT8 CH395CRCRet6Bit(UINT8 *mac_addr)
{
    INT32 perByte;
    INT32 perBit;
    const UINT32 poly = 0x04C11DB7;
    UINT32 crc_value = 0xFFFFFFFF;
    UINT8 c;
    for ( perByte = 0; perByte < 6; perByte ++ ) 
    {
        c = *(mac_addr++);
        for ( perBit = 0; perBit < 8; perBit++ ) 
        {
            crc_value = (crc_value<<1)^((((crc_value>>31)^c)&0x01)?poly:0);
            c >>= 1;
        }
    }
    crc_value=crc_value>>26;                                      
    return ((UINT8)crc_value);
}

/******************************************************************************
* Function Name  : CH395DHCPEnable
* Description    : ����/ֹͣDHCP
* Input          : flag   1:����DHCP;0��ֹͣDHCP
* Output         : None
* Return         : ִ��״̬
*******************************************************************************/
UINT8  CH395DHCPEnable(UINT8 flag)
{
    UINT8 i = 0;
    UINT8 s;
    xWriteCH395Cmd(CMD10_DHCP_ENABLE);
    xWriteCH395Data(flag);
    xEndCH395Cmd();
    while(1)
    {
        mDelaymS(20);
        s = CH395GetCmdStatus();                                     /* ���ܹ���Ƶ����ѯ*/
        if(s !=CH395_ERR_BUSY)break;                                 /* ���CH395оƬ����æ״̬*/
        if(i++ > 200)return CH395_ERR_UNKNOW;                        /* ��ʱ�˳�*/
    }
    return s;
}

/******************************************************************************
* Function Name  : CH395GetDHCPStatus
* Description    : ��ȡDHCP״̬
* Input          : None
* Output         : None
* Return         : DHCP״̬��0Ϊ�ɹ�������ֵ��ʾ����
*******************************************************************************/
UINT8 CH395GetDHCPStatus(void)
{
    UINT8 status;
    xWriteCH395Cmd(CMD01_GET_DHCP_STATUS);
    status = xReadCH395Data();
    xEndCH395Cmd();
    return status;
}

/*******************************************************************************
* Function Name  : CH395GetIPInf
* Description    : ��ȡIP��������������ص�ַ
* Input          : None
* Output         : 12���ֽڵ�IP,������������ص�ַ
* Return         : None
*******************************************************************************/
void CH395GetIPInf(UINT8 *addr)
{
    UINT8 i;
    xWriteCH395Cmd(CMD014_GET_IP_INF);
    for(i = 0; i < 20; i++)
    {
     *addr++ = xReadCH395Data();
    }
    xEndCH395Cmd();
}

/*******************************************************************************
* Function Name  : CH395WriteGPIOAddr
* Description    : дGPIO�Ĵ���
* Input          : regadd   �Ĵ�����ַ
*                ��regval   �Ĵ���ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395WriteGPIOAddr(UINT8 regadd,UINT8 regval)
{
    xWriteCH395Cmd(CMD20_WRITE_GPIO_REG);
    xWriteCH395Data(regadd);
    xWriteCH395Data(regval);
}

/*******************************************************************************
* Function Name  : CH395ReadGPIOAddr
* Description    : ��GPIO�Ĵ���
* Input          : regadd   �Ĵ�����ַ
* Output         : None
* Return         : �Ĵ�����ֵ
*******************************************************************************/
UINT8 CH395ReadGPIOAddr(UINT8 regadd)
{
    UINT8 i;
    xWriteCH395Cmd(CMD10_READ_GPIO_REG);
    xWriteCH395Data(regadd);
    mDelaymS(1);
    i = xReadCH395Data();
    return i;
}

/*******************************************************************************
* Function Name  : CH395EEPROMErase
* Description    : ����EEPROM
* Input          : None
* Output         : None
* Return         : ִ��״̬
*******************************************************************************/
UINT8 CH395EEPROMErase(void)
{
    UINT8 i;    
    xWriteCH395Cmd(CMD00_EEPROM_ERASE);
    while(1)
    {
        mDelaymS(20);
       i = CH395GetCmdStatus();
       if(i == CH395_ERR_BUSY)continue;
       break;
    }
    return i;
}

/*******************************************************************************
* Function Name  : CH395EEPROMWrite
* Description    : дEEPROM
* Input          : eepaddr  EEPROM��ַ
*                ��buf      ��������ַ
*                ��len      ����
* Output         : None
* Return         : ִ��״̬
*******************************************************************************/
UINT8 CH395EEPROMWrite(UINT16 eepaddr,UINT8 *buf,UINT8 len)
{
    UINT8 i;
    xWriteCH395Cmd(CMD30_EEPROM_WRITE);
    xWriteCH395Data((UINT8)(eepaddr));
    xWriteCH395Data((UINT8)(eepaddr >> 8));
    xWriteCH395Data(len);  
    while(len--)xWriteCH395Data(*buf++);
    while(1)
    {
        mDelaymS(20);
       i = CH395GetCmdStatus();
       if(i == CH395_ERR_BUSY)continue;
       break;
    }
    return i;
}
  
/*******************************************************************************
* Function Name  : CH395EEPROMRead
* Description    : дEEPROM
* Input          : eepaddr  EEPROM��ַ
*                ��buf      ��������ַ
*                ��len      ����
* Output         : None
* Return         : None
*******************************************************************************/
void CH395EEPROMRead(UINT16 eepaddr,UINT8 *buf,UINT8 len)
{
    xWriteCH395Cmd(CMD30_EEPROM_READ);
    xWriteCH395Data((UINT8)(eepaddr));
    xWriteCH395Data((UINT8)(eepaddr >> 8));
    xWriteCH395Data(len);  
    mDelaymS(1);
    while(len--)*buf++ = xReadCH395Data();
}

/*******************************************************************************
* Function Name  : CH395SetTCPMss
* Description    : ����TCP MSSֵ
* Input          : tcpmss 
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetTCPMss(UINT16 tcpmss)
{
    xWriteCH395Cmd(CMD20_SET_TCP_MSS);
    xWriteCH395Data((UINT8)(tcpmss));
    xWriteCH395Data((UINT8)(tcpmss >> 8));
}

/*******************************************************************************
* Function Name  : CH395SetSocketRecvBuf
* Description    : ����Socket���ջ�����
* Input          : sockindex  socket����
                 ��startblk   ��ʼ��ַ
                 ��blknum     ��λ���������� ����λΪ512�ֽ�
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketRecvBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum)
{
    xWriteCH395Cmd(CMD30_SET_RECV_BUF);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(startblk);
    xWriteCH395Data(blknum);
}

/*******************************************************************************
* Function Name  : CH395SetSocketSendBuf
* Description    : ����Socket���ͻ�����
* Input          : sockindex  socket����
                 ��startblk   ��ʼ��ַ
                 ��blknum     ��λ����������
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetSocketSendBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum)
{
    xWriteCH395Cmd(CMD30_SET_SEND_BUF);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(startblk);
    xWriteCH395Data(blknum);
}

/*******************************************************************************
* Function Name  : CH395UDPSendTo
* Description    : UDP��ָ����IP�Ͷ˿ڷ�������
* Input          : buf     : �������ݻ�����
                   len     : �������ݳ���
				   ip      : Ŀ��IP
				   port    : Ŀ��˿�
				   sockeid : socket����ֵ
* Output         : None
* Return         : None
*******************************************************************************/
void CH395UDPSendTo(UINT8 *buf,UINT32 len,UINT8 *ip,UINT16 port,UINT8 sockindex)
{
    CH395SetSocketDesIP(sockindex,ip);                            /* ����socket 0Ŀ��IP��ַ */         
    CH395SetSocketDesPort(sockindex,port);
    CH395SendData(sockindex,buf,len);    
}

/*******************************************************************************
* Function Name  : CH395SetStartPara
* Description    : ����CH395��������
* Input          : mdata
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetStartPara(UINT32 mdata)
{
    xWriteCH395Cmd(CMD40_SET_FUN_PARA);
    xWriteCH395Data((UINT8)mdata);
    xWriteCH395Data((UINT8)((UINT16)mdata>>8));
    xWriteCH395Data((UINT8)(mdata >> 16));
    xWriteCH395Data((UINT8)(mdata >> 24));
}

/*******************************************************************************
* Function Name  : CH395CMDGetGlobIntStatus
* Description    : ��ȡȫ���ж�״̬���յ�������CH395�Զ�ȡ���ж�,0x44�����ϰ汾ʹ��
* Input          : None
* Output         : None
* Return         : ���ص�ǰ��ȫ���ж�״̬
*******************************************************************************/
UINT16 CH395CMDGetGlobIntStatus_ALL(void)
{
		UINT16 init_status;
		xWriteCH395Cmd(CMD02_GET_GLOB_INT_STATUS_ALL);
		mDelayuS(2);
		init_status = xReadCH395Data();
		init_status = (UINT16)(xReadCH395Data()<<8) + init_status;
		xEndCH395Cmd();
		return 	init_status;
}

/*******************************************************************************
* Function Name  : CH395SetKeepLive
* Description    : ����keepalive����
* Input          : sockindex Socket��
*                  cmd 0���ر� 1������
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetKeepLive(UINT8 sockindex,UINT8 cmd)
{
    xWriteCH395Cmd(CMD20_SET_KEEP_LIVE_SN);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(cmd);
}

/*******************************************************************************
* Function Name  : CH395KeepLiveCNT
* Description    : ����keepalive���Դ���
* Input          : cnt ���Դ�������
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveCNT(UINT8 cnt)
{
    xWriteCH395Cmd(CMD10_SET_KEEP_LIVE_CNT);
    xWriteCH395Data(cnt);
}

/*******************************************************************************
* Function Name  : CH395KeepLiveIDLE
* Description    : ����KEEPLIVE����
* Input          : idle ����ʱ�䣨��λ��ms��
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveIDLE(UINT32 idle)
{
    xWriteCH395Cmd(CMD40_SET_KEEP_LIVE_IDLE);
    xWriteCH395Data((UINT8)idle);
    xWriteCH395Data((UINT8)((UINT16)idle>>8));
    xWriteCH395Data((UINT8)(idle >> 16));
    xWriteCH395Data((UINT8)(idle >> 24));
}

/*******************************************************************************
* Function Name  : CH395KeepLiveINTVL
* Description    : ����KeepLive���ʱ�� 
* Input          : intvl ���ʱ�䣨��λ��ms��
* Output         : None
* Return         : None
*******************************************************************************/
void CH395KeepLiveINTVL(UINT32 intvl)
{
    xWriteCH395Cmd(CMD40_SET_KEEP_LIVE_INTVL);
    xWriteCH395Data((UINT8)intvl);
    xWriteCH395Data((UINT8)((UINT16)intvl>>8));
    xWriteCH395Data((UINT8)(intvl >> 16));
    xWriteCH395Data((UINT8)(intvl >> 24));
}

/*******************************************************************************
* Function Name  : CH395SetTTLNum
* Description    : ����TTL
* Input          : sockindex Socket��
*                  TTLnum:TTL��
* Output         : None
* Return         : None
*******************************************************************************/
void CH395SetTTLNum(UINT8 sockindex,UINT8 TTLnum)
{
    xWriteCH395Cmd(CMD20_SET_TTL);
    xWriteCH395Data(sockindex);
    xWriteCH395Data(TTLnum);
}

/**************************** endfile *************************************/



