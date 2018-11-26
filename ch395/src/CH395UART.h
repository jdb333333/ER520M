#ifndef __CH395UART_h__
#define __CH395UART_h__

#include "CH395INC.h"

#define UART_INIT_BAUDRATE         9600                          /* 默认通讯波特率9600bps,建议通过硬件引脚设定直接选择更高的CH395的默认通讯波特率 */
//#define UART_WORK_BAUDRATE         57600                         /* 正式通讯波特率57600bps */

extern UINT8 CH395_INT_WIRE;                         /* 假定CH395的INT#引脚,如果未连接那么也可以通过查询串口中断状态码实现 */
extern UINT8 eutron_devKey;

void eutron_dev_init(void);
void eutron_dev_read(void);

void mDelayuS(UINT8 us);
void mDelaymS(UINT8 ms);
/*******************************************************************************
* Function Name  : CH395_PORT_INIT
* Description    : 硬件初始化部分
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH395_PORT_INIT(void);
void CH395_PORT_Set_speed(int nSpeed);


/*******************************************************************************
* Function Name  : Set_MCU_BaudRate
* Description    : 设置单片机波特率
*                  将单片机切换到正式通讯波特率 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetMCUBaudRate(void)   ;                             



/********************************************************************************
* Function Name  : xWriteCH395Cmd
* Description    : 向CH395写命令
* Input          : cmd 8位的命令码
* Output         : None
* Return         : None
*******************************************************************************/
void xWriteCH395Cmd(UINT8 cmd)  ;                                    


/********************************************************************************
* Function Name  : xWriteCH395Data
* Description    : 向CH395写数据
* Input          : mdata 8位数据
* Output         : None
* Return         : None
*******************************************************************************/
void  xWriteCH395Data(UINT8 mdata);


/********************************************************************************
* Function Name  : xReadCH395Data
* Description    : 从CH395读数据
* Input          : None
* Output         : None
* Return         : 8位数据
*******************************************************************************/
UINT8  xReadCH395Data(void)         ;                                 /* 从CH395读数据 */


#define xEndCH395Cmd()     {}                                        /* 命令结束，仅在SPI模式下有效 */

#endif
/**************************** endfile *************************************/
