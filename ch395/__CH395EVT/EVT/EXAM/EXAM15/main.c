/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : ������������ʾ��λ�����ͨ��UDP�㲥�ķ�ʽ����CH395����,CH395������һ��
                       UDP Socket �����շ��㲥��Ϣ��
             
**********************************************************************************/
#include <reg52.h>
#include "stdio.h"
#include "string.h"
#include "../PUB/CH395INC.H"
/***********************************************************************************
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

#include "CH395.H"
#include "../PUB/CH395CMD.C"
#include "ModuleConfig.h"


UINT8 RecvFlag = 0;
UINT8 Recvbuf[300] ;  
UINT8 Configbuf[60]= { 
   'C','H','3','9','5','M','O','D','U','L','E',0,0,0,0,0,0,0,0,0,0,/*ģ�鱾���IP��ַ*/    
  1,                                                               /*��ʶģ�鴦����ģʽ(TCP client)*/
  192,168,1,10,                                                    /*ģ�鱾���IP��ַ  */
  255,255,255,0,                                                   /*ģ�鱾����������� */
  192,168,1,1,                                                     /*ģ���Ӧ�����ص�ַ */
  2000%256,2000/256,                                               /*ģ��Դ�˿� */
  192,168,1,100,                                                   /*Ŀ��IP��ַ */
    1000%256,1000/256 ,                                            /*Ŀ�Ķ˿�  */
}; 
 pmodule_cfg CFG = (pmodule_cfg)Configbuf;

/**********************************************************************************/
/*********************************************************************************
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
  UINT8 i,*p;
 
  mDelaymS(100);
  mInitSTDIO();                                                   /* ��ʱ100���� */
  printf("CH395EVT Test Demo\n");

  CH395_PORT_INIT();
CH395INIT:  
  memcpy(CH395IPAddr,CFG->src_ip,sizeof(CFG->src_ip));         
  memcpy(CH395GWIPAddr,CFG->getway,sizeof(CFG->getway));   
  memcpy(CH395IPMask,CFG->getway,sizeof(CFG->getway));       

  InitCH395InfParam();                                              /* ��ʼ��CH395��ر��� */
  i = CH395Init();                                                  /* ��ʼ��CH395оƬ */
  mStopIfError(i);
  while(1)
  {                                                                 /* �ȴ���̫�����ӳɹ�*/
  if(CH395CMDGetPHYStatus() == PHY_DISCONN)                         /* ��ѯCH395�Ƿ����� */
  {
     mDelaymS(200);                                                 /* δ������ȴ�200MS���ٴβ�ѯ */
  }
  else 
  {
     printf("CH395 Connect Ethernet\n");                            /* CH395оƬ���ӵ���̫������ʱ������ж� */
     break;
  }
  }
  InitSocketParam();                                                /* ��ʼ��socket��ر��� */
  CH395SocketInitOpen();
  while(1)
  {
   if(CH395_INT_WIRE == 0)CH395GlobalInterrupt();
   if(RecvFlag){
   RecvFlag = 0;
   p = &Recvbuf[8] ;
   i = ParseConfigbuf(p) ;                                          /*��������*/
   if(i){
        printf("RESET CH395\n");
        CH395CMDReset();
        mDelaymS(200);
        goto CH395INIT;
        }
  }
  }

}