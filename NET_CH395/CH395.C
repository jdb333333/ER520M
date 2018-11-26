/********************************** (C) COPYRIGHT *********************************
* File Name          : CH395.C
* Author             : WCH
* Version            : V1.1
* Date               : 2014/8/1
* Description        : CH395功能演示
**********************************************************************************/

/**********************************************************************************
CH395 TCP/IP 协议族接口
MSC51 演示程序，用于演示Socket0工作在TCP Client模式下，单片机收到数据后，按位取反后
上传。MCS51@24MHZ,KEIL 3.51
**********************************************************************************/
/* 头文件包含*/

#include "stdio.h"
#include "string.h"
#include "CH395INC.H"
#include "CH395.H"

extern void xprintf (const char* fmt, ...);

/**********************************************************************************

CH395_OP_INTERFACE_MODE可以为1-5
1：硬件总线并口连接方式
2：软件模拟并口连接方式
3: 硬件SPI连接方式
4: 软件模拟SPI方式
5: 硬件异步串口连接方式
*/
#define   CH395_OP_INTERFACE_MODE             3                      
#if   (CH395_OP_INTERFACE_MODE == 1)                                 /* SEL = 0, TX = 1*/
#include "../PUB/CH395PARA_HW.C"                                           
#elif (CH395_OP_INTERFACE_MODE == 2)                                 /* SEL = 0, TX = 1*/
#include "../PUB/CH395PARA_SW.C"                                            
#elif (CH395_OP_INTERFACE_MODE == 3)                                 /* SEL = 1, TX = 0*/
#include "CH395SPI_HW.C"
#elif (CH395_OP_INTERFACE_MODE == 4)                                 /* SEL = 1, TX = 0*/
#include "../PUB/CH395SPI_SW.C"
#elif (CH395_OP_INTERFACE_MODE == 5)                                 /* SEL = 1, TX = 1*/
#include "../PUB/CH395UART.C"
#else
#error "Please Select Correct Communication Interface "
#endif

/**********************************************************************************/
/* 包含命令文件 */
#include "CH395CMD.C"

#define    CH395_DEBUG           1
#define DEF_KEEP_LIVE_IDLE                           (15*1000)        /* 空闲时间 */
#define DEF_KEEP_LIVE_PERIOD                         (20*1000)        /* 间隔为15秒，发送一次KEEPLIVE数据包 */                  
#define DEF_KEEP_LIVE_CNT                            200                /* 重试次数  */

/* 常用变量定义 */
UINT8  MyBuffer[512];                                           /* 数据缓冲区 */
UINT8  SMyBuffer[512];                                           /* 发送数据缓冲区 */ //jdb2018-04-20
unsigned int buflen, sbuflen = 0;				//jdb2018-04-02 缓冲区有效数据长度
struct _SOCK_INF  SockInf;                                      /* 保存Socket信息 */
struct _CH395_SYS  CH395Inf;                                    /* 保存CH395信息 */

struct _SOCK_INF  UDPSockInf;         //jdb2018-05-24                             /* 保存UDPSocket信息 */

/* CH395相关定义 */
//const UINT8 CH395IPAddr[4] = {192,168,99,222};//{192,168,111,100};                         /* CH395IP地址 */
UINT8 CH395IPAddr[4] = {192,168,99,222};//{192,168,111,100};                         /* CH395IP地址 *///jdb2018-05-03
const UINT8 CH395GWIPAddr[4] = {192,168,99,1};                        /* CH395网关 */
const UINT8 CH395IPMask[4] = {255,255,255,0};                        /* CH395子网掩码 */

/* socket 相关定义*/
const UINT8  Socket0DesIP[4] = {192,168,1,103};//{192,168,111,78};                      /* Socket 0目的IP地址 */
const UINT16 Socket0DesPort = 6000;                                  /* Socket 0目的端口 */
const UINT16 Socket0SourPort = 6000;                                 /* Socket 0源端口 */
const UINT8  BroadcastIP[4] = {255,255,255,255};                   //jdb2018-05-24  /* UDP 广播地址 */

const UINT16 Socket1SourPort = 11024;                                 /* Socket 1源端口 */


/**********************************************************************************
* Function Name  : mStopIfError
* Description    : 调试使用，显示错误代码，并停机
* Input          : iError
* Output         : None
* Return         : None
**********************************************************************************/
void mStopIfError(UINT8 iError)
{	
		#if 1
    if (iError == CMD_ERR_SUCCESS) return;                           /* 操作成功 */
		#else
		//jdb2018-03-16
		if (iError == CH395_ERR_OPEN) return;
		#endif
#if CH395_DEBUG
    xprintf("Error: %02X\n", (UINT16)iError);                         /* 显示错误 */
#endif
    while ( 1 ) 
    {
        mDelaymS(200);
        mDelaymS(200);
    }
}

/**********************************************************************************
* Function Name  : InitCH395InfParam
* Description    : 初始化CH395Inf参数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
//jdb2018-03-16
#if 0
void InitCH395InfParam(void)
#else
void InitCH395InfParam(UINT8 IP1, UINT8 IP2, UINT8 IP3, UINT8 IP4)
#endif
{
	//jdb2018-05-03 IP地址可配置
	CH395IPAddr[0] = IP1;
	CH395IPAddr[1] = IP2;
	CH395IPAddr[2] = IP3;
	CH395IPAddr[3] = IP4;
	
    memset(&CH395Inf,0,sizeof(CH395Inf));                            /* 将CH395Inf全部清零*/
    memcpy(CH395Inf.IPAddr,CH395IPAddr,sizeof(CH395IPAddr));         /* 将IP地址写入CH395Inf中 */
    memcpy(CH395Inf.GWIPAddr,CH395GWIPAddr,sizeof(CH395GWIPAddr));   /* 将网关IP地址写入CH395Inf中 */
    memcpy(CH395Inf.MASKAddr,CH395IPMask,sizeof(CH395IPMask));       /* 将子网掩码写入CH395Inf中 */
}

/**********************************************************************************
* Function Name  : InitSocketParam
* Description    : 初始化socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void InitSocketParam(void)
{
    memset(&SockInf,0,sizeof(SockInf));                              /* 将SockInf[0]全部清零*/
		
		#if 0
    memcpy(SockInf.IPAddr,Socket0DesIP,sizeof(Socket0DesIP));        /* 将目的IP地址写入 */
    SockInf.DesPort = Socket0DesPort;                                /* 目的端口 */
    SockInf.SourPort = Socket0SourPort;                              /* 源端口 */
    SockInf.ProtoType = PROTO_TYPE_TCP;                              /* TCP模式 */
    SockInf.TcpMode = TCP_CLIENT_MODE;
		#else
		//jdb2018-03-16
    SockInf.SourPort = Socket1SourPort;                              /* 源端口 */
    SockInf.ProtoType = PROTO_TYPE_TCP;                              /* TCP模式 */
		SockInf.TcpMode = TCP_SERVER_MODE;															
		#endif

		//jdb2018-05-24   UDP模式
	memset(&UDPSockInf,0,sizeof(UDPSockInf));                        /* 将SockInf[0]全部清零*/
    memcpy(UDPSockInf.IPAddr,BroadcastIP,sizeof(BroadcastIP));     /* 将目的IP地址写入 */
    UDPSockInf.DesPort = Socket0DesPort;                             /* 目的端口 */
    UDPSockInf.SourPort = Socket0SourPort;                           /* 源端口 */
    UDPSockInf.ProtoType = PROTO_TYPE_UDP;                           /* UDP模式 */
}

/**********************************************************************************
* Function Name  : CH395SocketInitOpen
* Description    : 配置CH395 socket 参数，初始化并打开socket
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInitOpen(void)
{
    UINT8 i;

	//jdb2018-05-24  UDP模式
	    /* socket 0为UDP模式 */
    CH395SetSocketDesIP(0,UDPSockInf.IPAddr);                        /* 设置socket 0目标IP地址 */         
    CH395SetSocketProtType(0,PROTO_TYPE_UDP);                        /* 设置socket 0协议类型 */
    CH395SetSocketDesPort(0,UDPSockInf.DesPort);                     /* 设置socket 0目的端口 */
    CH395SetSocketSourPort(0,UDPSockInf.SourPort);                   /* 设置socket 0源端口 */
    i = CH395OpenSocket(0);                                          /* 打开socket 0 */
	if (i == CH395_ERR_OPEN){
			i = CH395CloseSocket(0);
			mStopIfError(i);
			i = CH395OpenSocket(0);
		}
    mStopIfError(i);

		#if 0
    /* socket 0为TCP 客户端模式 */
    CH395SetSocketDesIP(0,SockInf.IPAddr);                           /* 设置socket 0目标IP地址 */         
    CH395SetSocketProtType(0,SockInf.ProtoType);                     /* 设置socket 0协议类型 */
    CH395SetSocketDesPort(0,SockInf.DesPort);                        /* 设置socket 0目的端口 */
    CH395SetSocketSourPort(0,SockInf.SourPort);                      /* 设置socket 0源端口 */
	
    i = CH395OpenSocket(0);                                          /* 打开socket 0 */
    mStopIfError(i);                                                 /* 检查是否成功 */
	
    i = CH395TCPConnect(0);                                          /* TCP连接 */
    mStopIfError(i);                                                 /* 检查是否成功 */
		#else
		//jdb2018-03-16
    CH395SetSocketProtType(1,PROTO_TYPE_TCP); 
    CH395SetSocketSourPort(1,SockInf.SourPort);
    i = CH395OpenSocket(1);
		if (i == CH395_ERR_OPEN){
			i = CH395CloseSocket(1);
			mStopIfError(i);
			i = CH395OpenSocket(1);
		}
    mStopIfError(i); 
    i = CH395TCPListen (1);                                 
    mStopIfError(i);                                           
		#endif
}



/*********************************************************************************
* Function Name  : keeplive_set
* Description    : 保活定时器参数设置
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void keeplive_set(void)
{
	CH395KeepLiveCNT(DEF_KEEP_LIVE_CNT);
	CH395KeepLiveIDLE(DEF_KEEP_LIVE_IDLE);
	CH395KeepLiveINTVL(DEF_KEEP_LIVE_PERIOD);
}

/**********************************************************************************
* Function Name  : CH395SocketInterrupt
* Description    : CH395 socket 中断,在全局中断中被调用
* Input          : sockindex
* Output         : None
* Return         : None
**********************************************************************************/
void CH395SocketInterrupt(UINT8 sockindex)
{
   UINT8  sock_int_socket;
   UINT8  i;
   UINT16 len;
   UINT16 tmp;
	 UINT8 buf[10];

	 //return; //jdb2018-04-22 调整为查询方式

	 //if(sockindex != 1){ //jdb2018-04-20
	 	//return;
	 //}

   sock_int_socket = CH395GetSocketInt(sockindex);                   /* 获取socket 的中断状态 */
	#if 1 //jdb2018-04-22 调整为查询方式
   if(sock_int_socket & SINT_STAT_SENBUF_FREE)                       /* 发送缓冲区空闲，可以继续写入要发送的数据 */
   {
#if CH395_DEBUG
       xprintf("SINT_STAT_SENBUF_FREE \n");
#endif
	   //if(sbuflen > 0)
	   		//CH395SendData(sockindex,SMyBuffer,sbuflen); //jdb2018-04-20
   }
   #endif
   #if 1
   if(sock_int_socket & SINT_STAT_SEND_OK)                           /* 发送完成中断 */
   {
#if CH395_DEBUG
       xprintf("SINT_STAT_SEND_OK \n");
#endif
	   sbuflen = 0; //jdb2018-04-20
   }
   #endif
   #if 1 //调整为查询方式
   if(sock_int_socket & SINT_STAT_RECV)                              /* 接收中断 */
   {
       len = CH395GetRecvLength(sockindex);                          /* 获取当前缓冲区内数据长度 */
#if CH395_DEBUG
       xprintf("receive len = %d\n",len);
#endif
       if(len == 0)return;
       if(len > 512)len = 512;                                       /* MyBuffer缓冲区长度为512 */
       CH395GetRecvData(sockindex,len,MyBuffer);                     /* 读取数据 */
	   buflen = len; //jdb2018-04-02
	   #if 1 //jdb2018-04-02
       for(tmp =0; tmp < len; tmp++)                                 /* 将所有数据按位取反 */
       {
          MyBuffer[tmp] = ~MyBuffer[tmp];
       }
       CH395SendData(sockindex,MyBuffer,len);
	   #endif

   }
   #endif
   #if 1
	if(sock_int_socket & SINT_STAT_CONNECT)                          /* 连接中断，仅在TCP模式下有效*/
	{
		#if 0
		CH395SetKeepLive(sockindex,1);								 /*打开KEEPALIVE保活定时器*/
		CH395SetTTLNum(sockindex,40); 								 /*设置TTL*/
		#else
		//jdb2018-03-16
    if(SockInf.TcpMode == TCP_SERVER_MODE)             
    {
			CH395CMDGetRemoteIPP(sockindex,buf);
			tmp = (UINT16)(buf[5]<<8) + buf[4];
#if CH395_DEBUG
			xprintf("IP address = %d.%d.%d.%d\n",(UINT16)buf[0],(UINT16)buf[1],(UINT16)buf[2],(UINT16)buf[3]);                    
			xprintf("Port = %d\n",tmp);    
#endif
		}
		#endif
	}
	#endif
   /*
   **产生断开连接中断和超时中断时，CH395默认配置是内部主动关闭，用户不需要自己关闭该Socket，如果想配置成不主动关闭Socket需要配置
   **SOCK_CTRL_FLAG_SOCKET_CLOSE标志位（默认为0），如果该标志为1，CH395内部不对Socket进行关闭处理，用户在连接中断和超时中断时调用
   **CH395CloseSocket函数对Socket进行关闭，如果不关闭则该Socket一直为连接的状态（事实上已经断开），就不能再去连接了。
   */
	if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
	{
		//jdb2018-03-16
#if CH395_DEBUG		 
		xprintf("SINT_STAT_DISCONNECT \n");
#endif
		//i = CH395CloseSocket(sockindex);                             
		//mStopIfError(i);
		mDelaymS(200); 
    i = CH395OpenSocket(sockindex);
    mStopIfError(i);
    i = CH395TCPListen (sockindex);                                 
    mStopIfError(i);
	}
	#if 1
   if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断 */
   { 
#if CH395_DEBUG		 
		xprintf("time out \n");
#endif
		 //		i = CH395CloseSocket(sockindex);                             
//		mStopIfError(i);
		//jdb2018-03-16
		if(SockInf.TcpMode == TCP_CLIENT_MODE)             
		{
			mDelaymS(200);                                       
      i = CH395OpenSocket(sockindex);
      mStopIfError(i);
      CH395TCPConnect(sockindex);                        
      mStopIfError(i);
		}
		if(SockInf.TcpMode == TCP_SERVER_MODE)             
		{
			mDelaymS(200); 
			i = CH395OpenSocket(sockindex);
			mStopIfError(i);
			i = CH395TCPListen (sockindex);                                 
			mStopIfError(i);
		}
   }
   #endif
}

//jdb2018-04-02
int GetFromBuf(char *buf, unsigned int size)
{
	int ret = 0;
	char tmpbuf[512] = {0};

	#if 0 //test
	if(buflen > size){
		memcpy(buf, MyBuffer, size);
		buflen -= size;
		memcpy(tmpbuf, MyBuffer+size, buflen);
		memcpy(MyBuffer, tmpbuf, buflen);
		ret = size;
	}else if(buflen > 0){
		memcpy(buf, MyBuffer, buflen);
		ret = buflen;
		buflen = 0;
	}else{
		ret = -1;
	}
	#endif

	return ret;
}

//jdb2018-04-02
void SendData(char *buf, unsigned int len)
{
	while(1){
		if(sbuflen == 0){
			//memcpy(SMyBuffer, buf, len);
			CH395SendData(1,buf,len);
			sbuflen = len;
			break;
		}
	}
	
}

//jdb2018-04-02
int CheckConnect()
{
	int ret = 0;
	
	if(CH395CMDGetPHYStatus() == PHY_DISCONN)
	{
		ret = -1;
	}

	return ret;
}

/**********************************************************************************
* Function Name  : CH395GlobalInterrupt
* Description    : CH395全局中断函数
* Input          : None
* Output         : None
* Return         : None
**********************************************************************************/
void CH395GlobalInterrupt(void)
{
   UINT16  init_status;
   UINT8   buf[10]; 
 
    init_status = CH395CMDGetGlobIntStatus_ALL();
    if(init_status & GINT_STAT_UNREACH)                              /* 不可达中断，读取不可达信息 */
    {
        CH395CMDGetUnreachIPPT(buf);                                
    }
    if(init_status & GINT_STAT_IP_CONFLI)                            /* 产生IP冲突中断，建议重新修改CH395的 IP，并初始化CH395*/
    {
    }
    if(init_status & GINT_STAT_PHY_CHANGE)                           /* 产生PHY改变中断*/
    {
        xprintf("Init status : GINT_STAT_PHY_CHANGE\n");
    }
    if(init_status & GINT_STAT_SOCK0)
    {
        CH395SocketInterrupt(0);                                     /* 处理socket 0中断*/
    }
    if(init_status & GINT_STAT_SOCK1)                                
    {
        CH395SocketInterrupt(1);                                     /* 处理socket 1中断*/
    }
    if(init_status & GINT_STAT_SOCK2)                                
    {
        CH395SocketInterrupt(2);                                     /* 处理socket 2中断*/
    }
    if(init_status & GINT_STAT_SOCK3)                                
    {
        CH395SocketInterrupt(3);                                     /* 处理socket 3中断*/
    }
    if(init_status & GINT_STAT_SOCK4)
    {
        CH395SocketInterrupt(4);                                     /* 处理socket 4中断*/
    }
    if(init_status & GINT_STAT_SOCK5)                                
    {
        CH395SocketInterrupt(5);                                     /* 处理socket 5中断*/
    }
    if(init_status & GINT_STAT_SOCK6)                                
    {
        CH395SocketInterrupt(6);                                     /* 处理socket 6中断*/
    }
    if(init_status & GINT_STAT_SOCK7)                                
    {
        CH395SocketInterrupt(7);                                     /* 处理socket 7中断*/
    }
}

/*********************************************************************************
* Function Name  : CH395Init
* Description    : 配置CH395的IP,GWIP,MAC等参数，并初始化
* Input          : None
* Output         : None
* Return         : 函数执行结果
**********************************************************************************/
UINT8  CH395Init(void)
{
    UINT8 i;
    
    i = CH395CMDCheckExist(0x65);                      
    if(i != 0x9a)return CH395_ERR_UNKNOW;                            /* 测试命令，如果无法通过返回0XFA */
                                                                     /* 返回0XFA一般为硬件错误或者读写时序不对 */
#if (CH395_OP_INTERFACE_MODE == 5)                                   
#ifdef UART_WORK_BAUDRATE
    CH395CMDSetUartBaudRate(UART_WORK_BAUDRATE);                     /* 设置波特率 */   
    mDelaymS(1);
    SetMCUBaudRate();
#endif
#endif
    CH395CMDSetIPAddr(CH395Inf.IPAddr);                              /* 设置CH395的IP地址 */
    CH395CMDSetGWIPAddr(CH395Inf.GWIPAddr);                          /* 设置网关地址 */
    CH395CMDSetMASKAddr(CH395Inf.MASKAddr);                          /* 设置子网掩码，默认为255.255.255.0*/ 	

    //CH395SetStartPara(FUN_PARA_FLAG_LOW_PWR|SOCK_DISABLE_SEND_OK_INT); //jdb2018-05-16/* CH395进入低能耗模式,屏蔽SINT_STAT_SEND_OK中断*/
	
    mDelaymS(100);
    i = CH395CMDInitCH395();                                         /* 初始化CH395芯片 */
    return i;
}

//jdb2018-03-16
#if 0
void Ethernet_Start(void)
#else
void Ethernet_Start(UINT8 IP1, UINT8 IP2, UINT8 IP3, UINT8 IP4)
#endif
{
	UINT8 i;
	
	CH395_PORT_INIT();

//jdb2018-03-16
#if 0
	InitCH395InfParam();                                             /* 初始化CH395相关变量 */
#else
	InitCH395InfParam(IP1, IP2, IP3, IP4);
#endif
	i = CH395Init();                                                 /* 初始化CH395芯片 */
	mStopIfError(i);
	//CH395CMDSetPHY(PHY_10M_FLL); //jdb2018-04-23 10M全双工模式

	#if 1
	keeplive_set(); 												/* 保活定时器参数设置  */  
	#else
	//jdb2018-03-16
    while(1)
    {               
       if(CH395CMDGetPHYStatus() == PHY_DISCONN)                    
       {
           mDelaymS(200);                                           
       }
       else 
       {
#if CH395_DEBUG
           xprintf("CH395 Connect Ethernet\n");                    
#endif
           break;
       }
    }
	#endif

	//jdb2018-03-16
	InitSocketParam();
	CH395SocketInitOpen();
}



