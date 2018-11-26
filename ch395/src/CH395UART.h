#ifndef __CH395UART_h__
#define __CH395UART_h__

#include "CH395INC.h"

#define UART_INIT_BAUDRATE         9600                          /* Ĭ��ͨѶ������9600bps,����ͨ��Ӳ�������趨ֱ��ѡ����ߵ�CH395��Ĭ��ͨѶ������ */
//#define UART_WORK_BAUDRATE         57600                         /* ��ʽͨѶ������57600bps */

extern UINT8 CH395_INT_WIRE;                         /* �ٶ�CH395��INT#����,���δ������ôҲ����ͨ����ѯ�����ж�״̬��ʵ�� */
extern UINT8 eutron_devKey;

void eutron_dev_init(void);
void eutron_dev_read(void);

void mDelayuS(UINT8 us);
void mDelaymS(UINT8 ms);
/*******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : Ӳ����ʼ������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void);
void CH395_PORT_Set_speed(int nSpeed);


/*******************************************************************************
* Function Name  : Set_MCU_BaudRate
* Description    : ���õ�Ƭ��������
*                  ����Ƭ���л�����ʽͨѶ������ 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetMCUBaudRate(void)   ;                             



/********************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : ��CH395д����
* Input          : cmd 8λ��������
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(UINT8 cmd)  ;                                    


/********************************************************************************
* Function Name  : xWriteCH395Data
* Description    : ��CH395д����
* Input          : mdata 8λ����
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(UINT8 mdata);


/********************************************************************************
* Function Name  : xReadCH395Data
* Description    : ��CH395������
* Input          : None
* Output         : None
* Return         : 8λ����
*******************************************************************************/
UINT8  xReadCH395Data(void)         ;                                 /* ��CH395������ */


#define xEndCH395Cmd()     {}                                        /* �������������SPIģʽ����Ч */

#endif
/**************************** endfile *************************************/
