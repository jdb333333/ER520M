/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395.C���ݽṹ����
*                      
**********************************************************************************/

#ifndef __CH395_H__
#define __CH395_H__
#include "CH395INC.h"


/* TCPģʽ��0Ϊ�ͻ��ˣ�1Ϊ������ */
#define TCP_CLIENT_MODE                                      0
#define TCP_SERVER_MODE                                      1

struct _CH395_SYS
{
    UINT8   IPAddr[4];                                           /* CH395IP��ַ 32bit*/
    UINT8   GWIPAddr[4];                                         /* CH395���ص�ַ 32bit*/
    UINT8   MASKAddr[4];                                         /* CH395�������� 32bit*/
    UINT8   MacAddr[6];                                          /* CH395MAC��ַ 48bit*/
    UINT8   PHYStat;                                             /* CH395 PHY״̬�� 8bit*/
    UINT8   MackFilt;                                            /* CH395 MAC���ˣ�Ĭ��Ϊ���չ㲥�����ձ���MAC 8bit*/
    UINT32  RetranCount;                                         /* ���Դ��� Ĭ��Ϊ10��*/
    UINT32  RetranPeriod;                                        /* ��������,��λMS,Ĭ��200MS */
    UINT8   IntfMode;                                            /* �ӿ�ģʽ */
    UINT8   UnreachIPAddr[4];                                    /* ���ɵ���IP */
    UINT16  UnreachPort;                                         /* ���ɵ���˿� */
};

struct _SOCK_INF
{
    UINT8  IPAddr[4];                                           /* socketĿ��IP��ַ 32bit*/
    UINT8  MacAddr[6];                                          /* socketĿ���ַ 48bit*/
    UINT8  ProtoType;                                           /* Э������ */
    UINT8  ScokStatus;                                          /* socket״̬���ο�scoket״̬���� */
    UINT8  TcpMode;                                             /* TCPģʽ */
    UINT32 IPRAWProtoType;                                      /* IPRAW Э������ */
    UINT16 DesPort;                                             /* Ŀ�Ķ˿� */
    UINT16 SourPort;                                            /* Ŀ�Ķ˿� */
    UINT16 SendLen;                                             /* �������ݳ��� */
    UINT16 RemLen;                                              /* ʣ�೤�� */
    UINT8  *pSend;                                              /* ����ָ�� */                                               
};   


void net_init1(void); //�������ڣ���λ����оƬ
void net_init2(void); //��������IP�͹���ģʽ
void net_main_loop(void); //���������¼�

#endif

/************************** endfile **************************************/
