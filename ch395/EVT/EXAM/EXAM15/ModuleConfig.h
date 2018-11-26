
/********************************** (C) COPYRIGHT *********************************
* File Name          : Moduleconfig.H
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395.C���ݽṹ����
*                      
**********************************************************************************/
//  NetModuleProtol.h: ��������CH395����ģ��ͨ�ŵĻ������ݽṹ�������ýṹ��������

#ifndef __MODULECONFIG_H__
#define __MODULECONFIG_H__

#define NET_MODULE_DATA_LENGTH    60     //��ģ��ͨ��ʱ����������󳤶�

#define MODULE_CFG_LEN            42

//ͨ��������
#define NET_MODULE_CMD_SET      0X01     //���������е�ģ��
#define NET_MODULE_CMD_GET      0X02     //��ȡĳ��ģ�������
#define NET_MODULE_CMD_RESET    0X03   //��ȡĳ��ģ�������
#define NET_MODULE_CMD_SEARCH   0X04    //���������е�ģ��

//Ӧ��������
#define NET_MODULE_ACK_SET      0X81     //��Ӧ����������
#define NET_MODULE_ACK_GET      0X82     //��Ӧ��ȡ������
#define NET_MODULE_ACK_RESET    0X83     //��ȡĳ��ģ�������
#define NET_MODULE_ACK_SEARCH   0X84     //��Ӧ����������

#define NET_MODULE_FLAG        "CH395NET_MODULE"    //������ʶͨ��

#define SynCode1          0x55
#define SynCode2          0xaa
//Ĭ������

//����ͨ�Žṹ��
typedef struct NET_COMM {
  unsigned char flag[16];                       //ͨ�ű�ʶ����Ϊ�����ù㲥��ʽ����ͨ�ŵģ����������һ���̶�ֵ
  unsigned char cmd;                            //����ͷ
  unsigned char id[6];                          //��ʶ��CH395MAC��ַ
  unsigned char len;                            //����������
  unsigned char dat[NET_MODULE_DATA_LENGTH];    //������������
}net_comm,*pnet_comm;

//ģ���ʶ
#define  NET_MODULE_TYPE_TCP_S    0X00    //ģ����ΪTCP SERVER
#define  NET_MODULE_TYPE_TCP_C    0X01    //ģ����ΪTCP CLIENT
#define  NET_MODULE_TYPE_UDP_S    0X02    //ģ����ΪUDP SERVER
#define NET_MODULE_TYPE_UDP_C     0X03    //ģ����ΪUDP CLIENT 



//ģ������ýṹ
typedef struct MODULE_CFG {
  unsigned char module_name[21];             //ģ�鱾���IP��ַ    
  unsigned char type;                        //��ʶģ�鴦����ģʽ(TCP/UDP server/client)
  unsigned char src_ip[4];                   //ģ�鱾���IP��ַ
  unsigned char mask[4];                     //ģ�鱾�����������
  unsigned char getway[4];                   //ģ���Ӧ�����ص�ַ
  unsigned char src_port[2];                 //ģ��Դ�˿�
  unsigned char dest_ip[4];                  //Ŀ��IP��ַ
  unsigned char dest_port[2];                //Ŀ�Ķ˿�
}module_cfg,*pmodule_cfg;

//Ĭ������
extern UINT8 DefaultSrcIp[4] ;
extern UINT8 DefaultMask[4]  ;
extern UINT8 DefaultGetway[4] ;
extern UINT16 DefaultSrcPort;
extern UINT8  ParseConfigbuf(PUINT8 buf);  
extern UINT8 DefaultDesIp[4];
extern UINT16 DefaultDesPort;
extern UINT8   MODULE_NAME[21];

#endif
