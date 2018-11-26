/********************************** (C) COPYRIGHT *******************************
* File Name          : CH395CMD.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395оƬ����ӿ��ļ�,�����ӳ����ⲿ����
*******************************************************************************/

#ifndef __CH395CMD_H__
#define __CH395CMD_H__
#include "CH395INC.h"

void CH395CMDReset(void);                                            /* ��λ */

void CH395CMDSleep(void);                                            /* ˯�� */

UINT8 CH395CMDGetVer(void);                                          /* ��ȡоƬ���̼��汾�� */

UINT8 CH395CMDCheckExist(UINT8 testdata);                            /* �������� */

void CH395CMDSetPHY(UINT8 phystat);                                  /* ����PHY״̬  */

UINT8 CH395CMDGetPHYStatus(void);                                    /* ��ȡPHY״̬ */

UINT8 CH395CMDGetGlobIntStatus(void);                                /* ��ȡCH395ȫ���ж�״̬ */

UINT8 CH395CMDInitCH395(void);                                       /* ��ʼ��CH395 */

void CH395CMDSetUartBaudRate(UINT32 baudrate);                       /* ���ò����� */

UINT8 CH395GetCmdStatus(void);                                       /* ��ȡ����ִ��״̬ */

void CH395CMDSetIPAddr(UINT8 *ipaddr);                               /* ����CH395��IP��ַ */

void CH395CMDSetGWIPAddr(UINT8 *gwipaddr);                           /* ����CH395������IP��ַ */

void CH395CMDSetMASKAddr(UINT8 *maskaddr);                           /* ������������ */

void CH395CMDSetMACAddr(UINT8 *amcaddr);                             /* ����CH395��MAC��ַ */

void CH395CMDGetMACAddr(UINT8 *amcaddr);                             /* ��ȡMAC��ַ */

void CH395CMDSetMACFilt(UINT8 filtype,UINT32 table0,UINT32 table1);  /* ����CH395��MAC���� */

void CH395CMDGetUnreachIPPT(UINT8 *list);                            /* ��ȡ���ɴ��ַ�Լ��˿� */

void CH395CMDGetRemoteIPP(UINT8 sockindex,UINT8 *list);              /* ��ȡԶ��IP�Ͷ˿ڣ�һ����TCP Server��ʹ�� */

void CH395SetSocketDesIP(UINT8 sockindex,UINT8 *ipaddr);             /* ����scoket n��Ŀ��IP��ַ */

void CH395SetSocketProtType(UINT8 sockindex,UINT8 prottype);         /* ����socket n��Э������ */

void CH395SetSocketDesPort(UINT8 sockindex,UINT16 desprot);          /* ����socket n��Ŀ�Ķ˿� */

void CH395SetSocketSourPort(UINT8 sockindex,UINT16 surprot);         /* ����socket n��Դ�˿� */

void CH395SetSocketIPRAWProto(UINT8 sockindex,UINT8 prototype);      /* ��IPRAWģʽ�£�����socket n��IP��Э���ֶ� */

void CH395SetRecvThresLen(UINT8 sockindex,UINT16 len);               /* ����socket n�Ľ����жϷ�ֵ */

void CH395SendData(UINT8 sockindex,UINT8 *databuf,UINT16 len);       /* ��socket n�ķ��ͻ�����д���� */

UINT16 CH395GetRecvLength(UINT8 sockindex);                          /* ��ȡsocket n�Ľ��ճ��� */

void CH395ClearRecvBuf(UINT8 sockindex);                             /* ���socket n�Ľ��ջ����� */

void CH395GetRecvData(UINT8 sockindex,UINT16 len,UINT8 *pbuf);       /* ��ȡ�������� */

void CH395CMDSetRetryCount(UINT8 count);                             /* ����������Դ��� */

void CH395CMDSetRetryPeriod(UINT16 period);                          /* ��������������� ��λ ����*/

void CH395CMDGetSocketStatus(UINT8 sockindex,UINT8 *status) ;        /* ��ȡsocket n��״̬ */

UINT8  CH395OpenSocket(UINT8 sockindex);                             /* ��socket n*/

UINT8  CH395CloseSocket(UINT8 sockindex);                             /* �ر�socket n*/

UINT8 CH395TCPConnect(UINT8 sockindex);                               /* TCP���� */

UINT8 CH395TCPListen(UINT8 sockindex);                                /* TCP���� */

UINT8 CH395TCPDisconnect(UINT8 sockindex);                            /* TCP�Ͽ����� */

UINT8 CH395GetSocketInt(UINT8 sockindex);                            /* ��ȡsocket n���ж�״̬ */
                                                                     
UINT8 CH395CRCRet6Bit(UINT8 *mac_addr);                              /* �ಥ��ַCRC32������HASH���� */
                                                                     /*Function Count = 36 */
void CH395GetIPInf(UINT8 *addr);

UINT8  CH395DHCPEnable(UINT8 flag);

void CH395WriteGPIOAddr(UINT8 regadd,UINT8 regval);

UINT8 CH395ReadGPIOAddr(UINT8 regadd);

UINT8 CH395EEPROMErase(void);

UINT8 CH395EEPROMWrite(UINT16 eepaddr,UINT8 *buf,UINT8 len);

void CH395EEPROMRead(UINT16 eepaddr,UINT8 *buf,UINT8 len);

void CH395SetTCPMss(UINT16 tcpmss);

void CH395SetSocketRecvBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum);

void CH395SetSocketSendBuf(UINT8 sockindex,UINT8 startblk,UINT8 blknum);

void CH395UDPSendTo(UINT8 *buf,UINT32 len,UINT8 *ip,UINT16 port,UINT8 sockindex);

void CH395SetStartPara(UINT32 mdata);

UINT16 CH395CMDGetGlobIntStatus_ALL(void);

void CH395SetKeepLive(UINT8 sockindex,UINT8 cmd);                                 /* keepalive ʹ�� */

void CH395KeepLiveCNT(UINT8 cnt);                                                /* keepalive �������� */

void CH395KeepLiveIDLE(UINT32 idle);                                             /* keepalive �������� */

void CH395KeepLiveINTVL(UINT32 intvl);                                           /* keepalive �������� */

void CH395SetTTLNum(UINT8 sockindex,UINT8 TTLnum);                               /* TTL�������� */
 
void CH395EnablePing(UINT8 enable);                                              /* PING ʹ�� */


#endif
/**************************** endfile *************************************/
