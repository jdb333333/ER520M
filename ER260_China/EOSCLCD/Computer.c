#define COMPUTER_C 7
#include "king.h"
#include "exthead.h"
#include "exth.h"
#include "flashb.h"
#include "Message.h"

#define MESSTOHOST  0		//为1时,COM命令用来把ChnMess发送给主机  //
#define ENABLE16	1		//是否开放更新16点阵显示字库的功能  //

//extern void EraseCCLib16();
void DowmLoadBINCLIB(BYTE libType);
void DowmLoadGRB(void);
void ProcessRecord(void);
void PrnBuffer(char *buf,short len);
//jdb2018-04-20
static signed char TcpProcessRecord(char cmd,unsigned char *hostString,unsigned char psize);

#if (PCREMOTE==1)
CONST char CommCanBe[]="PUDCISRFKE";	//K for PCREMOTE
#else
CONST char CommCanBe[]="PUDCISRFE";
#endif

BYTE    FromUSB=0;//=1,表示Computer收取的数据来源于USB口;=0,表述数据来源于串口
int	PCCounter=0;
BYTE	PCBuffer[PCBUFMAX];


void SendString(char *sStr,short len)
{

    if (len==-1)
        len = strlen(sStr);
    Bios_PortWrite(COMPUTER, sStr, len, 1);
}

/*
   short ReadString(char *sStr,short size)
   {
   short i,j,retry;
   i = retry = 0;
   while (i<size && retry<5)
   {
   j = ReadComm(COMPUTER_1);
   if (j!=-1)
   {
   sStr[i++]=j & 0xff;
   if (j==0x0d || j==0x0a)
   {
   sStr[i]=0;
   return i;
   }
   }
   else
   retry++;
   }
   return 1;
   }
   */
/*
   When CheckComm return SOH, system can read data from rs232 use readcomm.
   the first byte is the length of data-L, then are the Byte data-B;
   the last is verify byte-V;
   V = ~(L + B)+1
 *** 要使用p后面的一个字节存放校验码 ***
 */
BYTE SendRecord(BYTE *p ,BYTE length )
{
    BYTE bcc,retry,i,cpT;
    BYTE *record;
    short sACK;

#if defined(STM32F10X_HD_USBCOMM)
    if (FromUSB)
    {
        return SendUSBRecord(p,length);
    }
#endif

    if (!(length && COMPUTER))
        return FALSE;
    EmptyComm(COMPUTER_1);	/* empty port */
    //ccr2016-04-18    for (retry=0;retry<5;retry++)
    {
        SendComm(COMPUTER_1, SOH); // "发送SOH 建立通信；"
#if defined(CASE_INDONESIA)//印度版本:在发送流水时无须服务器应答SOH
        if (p[0]!='F')
#endif
        {
            for (i=3;(i && (sACK =ReadComm(COMPUTER_1)) != ACK && sACK != NAK);i--); //ccr2016-04-18
            if (!i || sACK==NAK)
                return FALSE;	// " failure"
        }
        record = p;
        bcc = length;

        SendComm(COMPUTER_1, length); //send the packet length;
        for (i = 0;i<length;i++)
        {
            bcc += *record;	// get the jiaoyanhe..BCC
            record += 1;
        }
        cpT = *record;
        *record = (~bcc+1);
        SendString(p,length+1); // Send the data..

        *record = cpT;

        for (i=3;(i && ((sACK =ReadComm(COMPUTER_1)) != ACK) && sACK != NAK);i--);//ccr2016-04-18
        if (sACK==NAK)
            return false;		// Send data failure..
        if (i)
            return TRUE;		// success.
    }
    return FALSE;
}


/**
 * 将数据发送给计算机
 *
 * @author EutronSoftware (2016-10-18)
 *
 * @param type :数据类型:
 *              2-销售次数
 *              3-销售数量
 *              4-退货数量
 *              5-销售金额
 *              6-折扣金额
 *              7-用于折扣的金额
 *              8-成本金额
 *              9-含税销售金额
 *              10-税额
 *              11-单品库存
 *              12-毛利
 * @param tot :数据(BCD)
 *
 * SysBuf[0]='S' or 'R'
 * SysBuf[1]='Z' or 'X' or 'C'
 * SysBuf[2]=type
 * SysBuf[3]=
 *
 * @return BYTE
 */
BYTE SendComp(BYTE type,BCD *tot)
{
    if (!ApplVar.RepComputer)
        return 0;
    SysBuf[2] = type;
    memcpy(SysBuf+7, tot, sizeof(BCD));
    return SendRecord(SysBuf, 7 + sizeof(BCD));
}

void PrnBuffer(char *buf,short len)
{
    short i,j;
    char sBuf[36];

    i = 0;
    for (j=0;j<len;j++)
    {
        if (!i)
            memset(sBuf,' ',sizeof(sBuf));
        HEXtoASC(sBuf+i*3,buf+j,1);
        i++;
        if (j % 10 == 9)
        {
            RJPrint(0,sBuf);
            i = 0;
        }
    }
    if (i)
        RJPrint(0,sBuf);
}

//jdb2018-04-02
//发送数据到PC
#define SINT_STAT_SENBUF_FREE					  (1<<0)
#define MSINT_STAT_RECV  						  (1<<2)
#define SINT_STAT_CONNECT                         (1<<3)                        /* 连接成功,TCP模式下产生此中断 */
#define SINT_STAT_DISCONNECT					  (1<<4)
#define SINT_STAT_TIM_OUT						  (1<<6)

#define CH395_ERR_OPEN                            0X20                          /* 已经打开 */

#define GINT_STAT_SOCK0                           (1<<4)             //jdb2018-05-24    
#define SINT_STAT_RECV                            (1<<2)            //jdb2018-05-24            /* socket端口接收到数据或者接收缓冲区不为空 *//* socket0 产生中断 */

#define GINT_STAT_SOCK1                           (1<<5)

int STH(char *buf, unsigned short len)
{
#if 0
    SendData(buf, len);
#else
    unsigned char  sock_int_socket;
    unsigned char  i;
    unsigned short  init_status;

#if 0
    sock_int_socket = CH395GetSocketInt(1);
    if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
    {
        mDelaymS(200); 
        i = CH395OpenSocket(1);
        mStopIfError(i);
        i = CH395TCPListen (1);                                 
        mStopIfError(i);
        return -1;
    }
    if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断 */
    { 
        mDelaymS(200); 
        i = CH395OpenSocket(1);
        mStopIfError(i);
        i = CH395TCPListen (1);                                 
        mStopIfError(i);
        return -1;
    }
#endif

    //__disable_irq();
    //__disable_fault_irq();

    CH395SendData(1,buf,len);

    //__enable_irq();
    //__enable_fault_irq();

    mDelaymS(10);

#if 0
    do{
        mDelaymS(100);

        init_status = CH395CMDGetGlobIntStatus_ALL();
        if(init_status & GINT_STAT_SOCK1) 
        {
        }
        else
        {
            continue;
        }

        sock_int_socket = CH395GetSocketInt(1);

        if(sock_int_socket & SINT_STAT_SENBUF_FREE)
        {
            break;
        }
    }while(1);
#endif

#if 0
    while(1){
        sock_int_socket = CH395GetSocketInt(1);
        if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
        {
            mDelaymS(200); 
            i = CH395OpenSocket(1);
            mStopIfError(i);
            i = CH395TCPListen (1);                                 
            mStopIfError(i);
            return -1;
        }
        if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断 */
        { 
            mDelaymS(200); 
            i = CH395OpenSocket(1);
            mStopIfError(i);
            i = CH395TCPListen (1);                                 
            mStopIfError(i);
            return -1;
        }
        if(sock_int_socket & SINT_STAT_SENBUF_FREE){
            break;
        }	
    }
#endif
#endif

    return 0;
}

//从PC接收数据
int RFH(char *buf, unsigned int size)
{
    int ret = 0;

	char udpbuf[64] = {0};
	int count;
	BYTE macaddr[6];

#if 0
    ret = GetFromBuf(buf, size);
#else
    unsigned char  sock_int_socket;
    unsigned short len;
    unsigned char  i;

    unsigned short  init_status;

	init_status = CH395CMDGetGlobIntStatus_ALL();

	//jdb2018-05-24 UDP SOCKET 0
//sock_int_socket = CH395GetSocketInt(0);

	if(init_status & GINT_STAT_SOCK0)  {
		sock_int_socket = CH395GetSocketInt(0);

	if(sock_int_socket & SINT_STAT_RECV){

len = CH395GetRecvLength(0);
if(len >= 8)
{
CH395GetRecvData(0,len,udpbuf);                     /* 读取数据 */

for(count = 0; count < len; count++)
{
	if(udpbuf[count] == 'I' && udpbuf[count + 1] == 'P' && udpbuf[count + 2] == '_' &&
		udpbuf[count + 3] == 'F' && udpbuf[count + 4] == 'O' && udpbuf[count + 5] == 'U' &&
		udpbuf[count + 6] == 'N' && udpbuf[count + 7] == 'D')
	{
		CH395CMDGetMACAddr(macaddr);
		memset(udpbuf, 0, sizeof(udpbuf));
		#if 0
		sprintf(udpbuf, "IP_ACK:%d.%d.%d.%d:%s-%d", ApplVar.IP.IPAddress[0], ApplVar.IP.IPAddress[1], 
			ApplVar.IP.IPAddress[2], ApplVar.IP.IPAddress[3], ApplVar.AP.SHOPNAME, ApplVar.AP.Flag[0x0f]); //jdb2018-07-12 增加商铺名 和收款机编号
		#else
		sprintf(udpbuf, "IP_ACK:%d.%d.%d.%d:%s-%d:%02x-%02x-%02x-%02x-%02x-%02x", ApplVar.IP.IPAddress[0], ApplVar.IP.IPAddress[1], 
			ApplVar.IP.IPAddress[2], ApplVar.IP.IPAddress[3], ApplVar.AP.SHOPNAME, ApplVar.AP.Flag[0x0f], 
			macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]); //jdb2018-07-12 增加商铺名 和收款机编号
		#endif
		CH395SendData(0,udpbuf,strlen(udpbuf));
		break;
	}
}

}
		}
		}



    //init_status = CH395CMDGetGlobIntStatus_ALL();

#if 1
    if(init_status & GINT_STAT_SOCK1)                                
    {
        //CH395SocketInterrupt(1);                                     /* 处理socket 1中断*/
    }else{
        return 0;
    }
#endif


#if 1
    sock_int_socket = CH395GetSocketInt(1);
	if(sock_int_socket & SINT_STAT_CONNECT) //jdb2018-06-20 连接中断
	{
		ApplVar.SPluNumber = 0;
	}
    if(sock_int_socket & SINT_STAT_DISCONNECT)                        /* 断开中断，仅在TCP模式下有效 */
    {
        mDelaymS(2); 
		//i = CH395CloseSocket(1);
		//mStopIfError(i);
        i = CH395OpenSocket(1);
        //mStopIfError(i);
        i = CH395TCPListen (1);                                 
        //mStopIfError(i);
        return 0;
    }
    if(sock_int_socket & SINT_STAT_TIM_OUT)                           /* 超时中断 */
    { 
        mDelaymS(2); 
		//i = CH395CloseSocket(1);
		//mStopIfError(i);
        i = CH395OpenSocket(1);
        //mStopIfError(i);
        i = CH395TCPListen (1);                                 
        //mStopIfError(i);
        return 0;
    }
#endif
#if 1
	if(sock_int_socket & SINT_STAT_RECV){ //jdb2018-06-04
    len = CH395GetRecvLength(1);                          /* 获取当前缓冲区内数据长度 */
    if(len == 0)
        return 0;
    if(len > 512)
        len = 512;                                       /* MyBuffer缓冲区长度为512 */
    CH395GetRecvData(1,len,buf);                     /* 读取数据 */
    mDelaymS(10);
    ret = len;
		}
#else
    if(sock_int_socket & MSINT_STAT_RECV){
        len = CH395GetRecvLength(1);                          /* 获取当前缓冲区内数据长度 */
        if(len == 0)
            return 0;
        if(len > 512)
            len = 512;                                       /* MyBuffer缓冲区长度为512 */
        CH395GetRecvData(1,len,buf);                     /* 读取数据 */
        ret = len;
    }	
#endif
#endif

    return ret;
}

/*********************************************************************
 * communicat with Host computer throw Rs232 port or USB
 * ccr2016-04-20:为扩展USB通讯修改,要避免USB和串口同时与主机通讯
 * @author EutronSoftware (2016-04-21)
 *********************************************************************/
#ifndef ISP_NET
void Computer()
{
    BYTE bcc;
    short i;
#if (defined(CASE_INNERDOTPRN) && defined(DEBUGBYPC))
    if (COMPUTER_1!=DOTPRNPORT)//testonly
#endif
        //ccr2016-04-20    while (COMPUTER)
#if defined(STM32F10X_HD_USBCOMM)
        while (USB_COMM_Ready() || COMPUTER)//ccr2016-06-24
#endif
        {
            //ccr2016-04-20    	Appl_PortNumber = COMPUTER_1;
            //ccr2016-04-20    	ReadPort();
            //ccr2016-04-20    	if (Appl_Port.Type=='1')//HOST mode
            {
                while (TRUE)
                {
                    if (COMPUTER && CheckComm(COMPUTER_1) && (i = ReadComm(COMPUTER_1))!=-1)
                    {
                        FromUSB = 0;
                        bcc = i & 0xff;
                    }
#if defined(STM32F10X_HD_USBCOMM)
                    else if (CheckUSB() && ReadUSB(&bcc))
                    {
                        FromUSB = 1;
                    }
#endif
                    else
                        return;

                    if (!PCCounter)
                    {
                        if (bcc == SOH)//Comm data not ok
                        {
                            //				    	EmptyComm(COMPUTER_1);
                            SendToHost(ACK);        /* sent ACK */
                            PCCounter = 0xff;				//set the flag,begin read data from host;
                            {
#if (SCRSTATDBL)
                                DispStrXY(Msg[DOWNLOAD].str,SCRDBLWD-3,STATELINE);//RG-DISPLAY
#else
                                DispStrXY(Msg[DOWNLOAD].str,SCREENWD-2,STATELINE);//RG-DISPLAY
#endif
                            }
                        }
                        continue;
                    }

                    if (PCCounter == 0xff)
                        PCCounter = 0;

                    PCBuffer[PCCounter++] = bcc;
                    if (PCCounter==2)
                    {
                        if (strchr(CommCanBe,bcc)==NULL)
                        {//不是合法的命令符,丢弃收到的两个数据(Len & CMD)
                            PCCounter = 0;
                        }
                    }


                    if (!PCBuffer[0] || PCBuffer[0]>=((PCBUFMAX<256)?(PCBUFMAX-2):(254)))//length out of the range
                    {
                        PCCounter = 0;
                        {
#if (SCRSTATDBL)
                            DispOnStatDBL((char*)cErrInfoN1,0);
#else
                            DispSt2C((char*)cErrInfoN1);//RG-DISPLAY
#endif
                            SendToHost(NAK);        /* sent NAK */
                        }
                        continue;
                    }

                    if (PCBuffer[0]==(PCCounter-2))
                    {//a package recived,include verify byte

                        bcc = PCBuffer[0];
                        for (i=1;i<=PCBuffer[0];i++)
                            bcc += PCBuffer[i];
                        if ((BYTE)(bcc+PCBuffer[PCCounter-1]))
                        {//the verify error
#if (SCRSTATDBL)
                            DispOnStatDBL((char*)cErrInfoN3,0);
#else
                            DispSt2C((char*)cErrInfoN3);//RG-DISPLAY
#endif
                            SendToHost(NAK);
                            PCCounter = 0;
                        }
                        else
                        {
                            i = PCBuffer[0];
                            memcpy(PCBuffer,PCBuffer+1,i);

#if (SCRSTATDBL)
                            DispOnStatDBL((char*)Msg[UPLOAD].str,0);
#else
                            DispStrXY(Msg[UPLOAD].str,SCREENWD-2,STATELINE);//RG-DISPLAY
#endif

                            SendToHost(ACK);
                            PCCounter = 0;
                            ProcessRecord();//
                        }
                    }
                }
            }
            //ccr2016-04-20        else
            //ccr2016-04-20            break;
        }
}
#else //jdb2018-04-02
#define ERR_OK          0    /* No error, everything OK. */

#define cmdUPDATE       'U'     //为更新数据请求
#define cmdDOWNLOAD     'D'     //为下载数据请求
#define cmdFLOWDATA     'F'     //为请求收款机流水数据
#define cmdANSWER       'A'     //为对host的应答命令
#define cmdGRAPHIC      'G'     //为下载图片
#define cmdCLEAR        'C'     //清除报表
#define cmdINIT         'I'     //收款机初始化
#define cmdSERVER       'S'     //设置收款机的服务器地址
#define cmdPASSWORD     'W'     //设置收款员密码

#define WAITFORTYPE 0//等待数据类型:'F'-为流水,'U'为请求
#define WAITFORSIZE 1
#define WAITFORDATA 2

static signed char TCP_DecodeRecord(unsigned char *data,unsigned char size)
{
    static int PackageReceived=0;
    unsigned char TCP_Command;//收到的数据类型;'F','U','D'....
    unsigned char TCP_SizeMust;//必须收取的数据数目

    unsigned char TCP_WaitFor=WAITFORTYPE;//=0,表示等待数据类型;=1标示等待长度字节;=2表示等待数据
    int i;
    int TCP_BytesReceived=0;//记录已经收到的数据数目

    for (i=0;i<size && (TCP_BytesReceived<(sizeof(PCBuffer)-2));i++)
    {
        switch (TCP_WaitFor)
        {
            case WAITFORTYPE://等待数据类型
                if (*data==cmdANSWER
                        || *data==cmdCLEAR
                        || *data==cmdUPDATE
                        || *data==cmdDOWNLOAD
                        || *data==cmdFLOWDATA
                        || *data==cmdGRAPHIC
                        || *data==cmdINIT
                        || *data==cmdPASSWORD
                        || *data==cmdSERVER //为传送流水和请求文件命令
                        || *data==0x5A //jdb2018-05-08 获取报表数据'Z'
						|| *data==0x50 //jdb2018-05-29 获取PLU总数'P'
						|| *data==0x52 //jdb2018-06-04 打印控制'R'
                   )
                {
                    TCP_Command = *data;
                    TCP_WaitFor = WAITFORSIZE;
                    TCP_BytesReceived = 0;
                }
                break;
            case WAITFORSIZE://等待数据长度,最长256字节
                //if (*data<=sizeof(union ECRFILEREC))
                {//为文件数据
                    TCP_SizeMust = *data;
                    TCP_WaitFor = WAITFORDATA;
                    PCBuffer[TCP_BytesReceived++]=TCP_SizeMust;
                    TCP_SizeMust++;
                }
                //else//非文件数据
                //     TCP_WaitFor = WAITFORTYPE;
                break;
            case WAITFORDATA://等待收取数据
                PCBuffer[TCP_BytesReceived++]=*data;
                if (TCP_BytesReceived==TCP_SizeMust)
                {//收取到一个完整的数据包
                    PackageReceived++;
                    TCP_WaitFor = WAITFORTYPE;
                    return TcpProcessRecord(TCP_Command, PCBuffer, TCP_BytesReceived);
                    //return tcp_write(tpcb, EcrRecord, TCP_BytesReceived, 1);
                }
                break;
        }
        data++;
    }
    return ERR_OK;//解析了一部分数据
}

void Computer()
{
    unsigned char tmpbuf[512] = {0};

    while (TRUE)
    {
        memset(tmpbuf, 0, sizeof(tmpbuf));
        PCCounter = RFH(tmpbuf, 512);
        if(PCCounter <= 0){
            return;
        }else{
            TCP_DecodeRecord(tmpbuf, PCCounter);
        }
    }
}
#endif

/*通讯测试数据
  WRITE[01][03][44][02][00][B7][06][06][06][06][06];读SYSFlags
  READ[06][06][01][42][02][00][00][01][00][02][28][00][00][00][06][10]
  [20][20][00][00][00][00][03][05][9A][30][01][05][00][58][42][01]
  [40][03][00][00][D2][00][01][00][07][02][00][00][02][00][00][02]
  [00][00][02][00][00][02][00][00][02][00][00][02][00][00][02][00]
  [42][20][2F][20][01][00][E9]
  READ [01][02][00][00][FE]
  */
/**
 * 处理收到的有PC发来的命令和数据
 * PCBuffer中包含了命令和数据
 * @author EutronSoftware (2015-04-22)
 */
extern void UpdateBIOS();
extern void EnterBootLoader();
#define SIZE_IAP    2048    //size of UpdateBios

#ifndef ISP_NET
void ProcessRecord()
{
    WORD start, end, sMax, id, save, type;
    short size;
    UnLong sAddr;
#if ((defined(CASE_EJSD) || defined(CASE_EJFLASH)))
    UnLong sAddrEnd;
#endif
    char *record;
    BYTE sPC,sBAR,sBAL,sEcrNo,sLocate,sEXTMem;
    void  (*RefreshBIOS)();


    type = *((WORD *)(PCBuffer + 1));   // type: get the DataType..
    id = 0;
    if (type > PLU1 || (type >= UPDPLU && type <= REPPLU))
        save = ApplVar.PluNumber;
    else if (type > DEPT)
        save = ApplVar.DeptNumber;
    else if (type > CLERK)
        save = ApplVar.ClerkNumber;
    else if (type > SALPER)
        save = ApplVar.SalPerNumber;
    switch(PCBuffer[0]){       /* determine record type */
        case 'U':
            {
#if !defined(DEBUGBYPC)
                if (PCBuffer[1]=='B' && PCBuffer[2]=='I' && PCBuffer[3]=='O' && PCBuffer[4]=='S')
                {
#if defined(STM32F10X_HD)
                    if (FromUSB)
                    {
                        strcpy(SysBuf,UPCOMM[UCA]);
                        SendRecord(SysBuf,3);
                        mClearScreen();
                        DispStrXY(XUPDATEBIOS,0,0);
                        return;
                    }
#endif
                    SETBIT(ApplVar.MyFlags, NEWFIRMWARE);
                    Save_ConfigApplVar();
                    //必须是SET档或者MAC跳线器短接
                    if (ApplVar.CentralLock == SET || Bios_TestMAC())
                    {
                        mClearScreen();
                        ClearLine(STATELINE);

                        // RJPrint(0,Msg[GXBIOS].str);
                        DispStrXY(Msg[GXBIOS].str,0,0);
                        // RFeed(10);

                        strcpy(SysBuf,UPCOMM[UOK]);
                        SendRecord(SysBuf,3);

                        //ccr2016-03-01                    PutsO("Update BIOS");

                        sAddr = ((ULONG)PCBuffer+260) & 0xfffffffcL;
                        if (sAddr+SIZE_IAP>=(SRAM_BASE+MCU_RAM_SIZE-256))
                        {
                            //DispStrXY(Msg[JSHDPSHL].str,0,2);//内存溢出
                            sAddr = ((ULONG)PCBuffer-SIZE_IAP) & 0xfffffffcL;
                        }
                        {
                            __disable_irq();    //禁止所有中断,以免将引起系统运行异常
                            __disable_fault_irq();

                            if ((ULONG)&UpdateBIOS & 1)
                            {
                                memcpy((char *)sAddr,((char *)UpdateBIOS)-1,SIZE_IAP);//(char *)&EndOfIAP-(char *)&UpdateBIOS+1);
                                sAddr +=1;
                            }
                            else
                            {
                                memcpy((char *)sAddr,((char *)UpdateBIOS),SIZE_IAP);//(char *)&EndOfIAP-(char *)&UpdateBIOS+1);
                            }
                            RefreshBIOS = (void*)sAddr;

                            //RefreshBIOS = UpdateBIOS;
                            (*RefreshBIOS)();//    			UpdateBIOS();
                            while (1){}
                        }
                    }
                    else
                    {
                        strcpy(SysBuf,UPCOMM[UCA]);
                        SendRecord(SysBuf,3);
                        return;
                    }
                    break;
                }
#endif
#if (1)//M3平台下,禁止下载图片,字库功能
#if (ENABLE16==1)
                if (type==('L'+('I'<<8)) && (PCBuffer[3]=='b' || PCBuffer[3]=='B') && (PCBuffer[4]=='b' || PCBuffer[4]=='B'))
                { //下载更新16点阵字库 //
                    if (ApplVar.CentralLock != SET)
                    {
                        strcpy(SysBuf,UPCOMM[UCA]);
                        SendRecord(SysBuf,3);
                        return;
                    }

                    strcpy(SysBuf,UPCOMM[UOK]);
                    SendRecord(SysBuf,3);


#if (SCRSTATDBL)
                    DispOnStatDBL((char *)Msg[ZZXZZKU].str,0);
#else
                    DispSt2C(Msg[ZZXZZKU].str);//RG-DISPLAY
#endif
                    ApplVar.DownLoad = 0x2a;
                    if (PCBuffer[3]=='B')//LIBB
                    {
#if (SCRSTATDBL)
                        DispOnStatDBL("CCLIB24",0);
#else
                        DispSt1C("CHN-LIB24");//RG-DISPLAY
#endif
                        DowmLoadBINCLIB(24);
                    }
                    else if (PCBuffer[4]=='B')//LIbB
                    {
#if (SCRSTATDBL)
                        DispOnStatDBL("CCLIB1624",0);
#else
                        DispSt1C("CHN-LIB16+24");//RG-DISPLAY
#endif

                        DowmLoadBINCLIB(16+24);
                    }
                    else//LIbb
                    {
#if (SCRSTATDBL)
                        DispOnStatDBL("CCLIB16",0);
#else
                        DispSt1C("CHN-LIB16");//RG-DISPLAY
#endif
                        DowmLoadBINCLIB(16);
                    }
                    break;
                }
#endif
                if (type==('G'+('R'<<8)) && PCBuffer[3]=='A' && PCBuffer[4]=='B')
                {
                    if (ApplVar.CentralLock != SET)
                    {
                        strcpy(SysBuf,UPCOMM[UCA]);
                        SendRecord(SysBuf,3);
                        return;
                    }

                    strcpy(SysBuf,UPCOMM[UOK]);
                    SendRecord(SysBuf,3);

#if (SCRSTATDBL)
                    DispOnStatDBL((char *)Msg[ZZXZTXING].str,0);
#else
                    DispSt1C(Msg[ZZXZTXING].str);//RG-DISPLAY;
#endif

                    ApplVar.DownLoad = 0x2A;

                    DowmLoadGRB();//ccr2014-03-25
                    break;
                }
#endif
                if (type > MERGEPLU-1 && type < REPPLU+1)
                {
                    CheckPluFile(type);
                    ApplVar.ErrorNumber=0;
                    break;
                }
            }
        case 'D':           /* command for dump program */
            {
                size = 0;
                record = 0;
                switch(type)  {
                    case PROMO:
                        size = sizeof(ApplVar.AP.Promotion);
                        record = (char *)&ApplVar.AP.Promotion;
                        break;
                    case SALPER:
                        size = sizeof(ApplVar.AP.SalPer);
                        record = (char *)&ApplVar.AP.SalPer;
                        break;
                    case OPTIONS:
                        size = sizeof(ApplVar.AP.Options);
                        record = (char *)&ApplVar.AP.Options;
                        break;
                    case PROGID :  /* program ID data */
                        size = sizeof(Release);
                        record = (char *)Release;
                        break;
                    case SYSFLG :  /* system Flags */
                        size = sizeof(ApplVar.AP.Flag);
                        record = (char *)ApplVar.AP.Flag;
                        break;
                    case START :  /* start addresses */
                        size = sizeof(ApplVar.AP.StartAddress) ;
                        record = (char *)ApplVar.AP.StartAddress;
                        break;
                    case RCONFIG : /* config */
                        size = sizeof(ApplVar.AP.Config);
                        record = (char *)&ApplVar.AP.Config;
                        break;
                    case RDAY : /* DAY table */
                        size = sizeof(ApplVar.AP.Day);
                        record = (char *)&ApplVar.AP.Day;
                        break;
                    case RMONTH : /* ApplVar.Month table */
                        size = sizeof(ApplVar.AP.Month);
                        record = (char *)&ApplVar.AP.Month;
                        break;
                    case RZONE : /* zone table */
                        size = sizeof(ApplVar.AP.Zone);
                        record = (char *)&ApplVar.AP.Zone;
                        break;
                    case RTOTAL : /* ApplVar.Total sales */
                        size = sizeof(ApplVar.AP.Sales);
                        record = (char *)&ApplVar.AP.Sales;
                        break;
                    case KEY0 :
                        size = 64;     /* 64 bytes is 32 keys */
                        record = (char *)ApplVar.AP.KeyTable;
                        break;
                    case KEY32 :
                        size = 64;     /* 64 bytes is 32 keys */
                        record = (char *)&ApplVar.AP.KeyTable[32];
                        break;
                        //		case KEY64 :
                        //		    size = 64;     /* 64 bytes is 32 keys */
                        //			record = (char *)&ApplVar.AP.KeyTable[64];
                        //		    break;
                        //		case KEY96 :
                        //		    size = 64;     /* 64 bytes is 32 keys */
                        //			record = (char *)&ApplVar.AP.KeyTable[96];
                        //		    break;
                    case RREP1 :        /* user report 1,2 & 3 */
                        size =  3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[0];
                        break;
                    case RREP2 :        /* user report 4,5 & 6 */
                        size =  3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[3];
                        break;
                    case RREP3 :        /* user report 7,8 & 9 */
                        size = 3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[6];
                        break;
#if (salNumber)
                    case RREP4 :        /* user report 7,8 & 9 */
                        size = 3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[9];
                        break;
                    case RREP5 :        /* user report 7,8 & 9 */
                        size = 1 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[12];
                        break;
#else
                    case RREP4 :        /* user report 7,8 & 9 */
                        size = 1 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[9];
                        break;
#endif
                    case REPCAP1 :        /* report types 0 - 6 */
                        size = 7 * REPWIDTH;
                        record = (char *)ApplVar.TXT.ReportType;
                        break;
                    case REPCAP2 :        /* report types 7 - 13 */
                        size = 7 * REPWIDTH;
                        record = (char *)ApplVar.TXT.ReportType[7];
                        break;
                    case REPCAP3 :        /* messages 14 - 15 */
                        size = 2 * REPWIDTH;
                        record = (char *)ApplVar.TXT.ReportType[14];
                        break;
                    case RHEAD1 :       /* receipt header 0 - 4 */
                        size = 5 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header;
                        break;
                    case RHEAD2 :       /* receipt header 5 - 7 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[5];
                        break;
                    case RTRAIL1 :       /* receipt trailer 0 - 4 */
                        size = 5 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer;
                        break;
                    case RTRAIL2 :       /* receipt trailer 5 - 5 */
                        size = 1 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[5];
                        break;
                    case RSLIPH1 :       /* slip header 0 - 2 */
                        size = 3 * SHEADWIDTH;
                        record = (char *)ApplVar.TXT.SlipHeader;
                        break;
                    case RSLIPH2 :       /* receipt header 3 - 5 */
                        size = 3 * SHEADWIDTH;
                        record = (char *)ApplVar.TXT.SlipHeader[3];
                        break;

                    case FIRM :
                        size = sizeof(ApplVar.AP.FirmKeys);
                        record = (char *)ApplVar.AP.FirmKeys;
                        break;
                    case MNGR :
                        size = sizeof(ApplVar.AP.Manager);
                        record = (char *)ApplVar.AP.Manager;
                        break;
                    case CORREC :
                        size = sizeof(ApplVar.AP.Correc);
                        record = (char *)&ApplVar.AP.Correc;
                        break;
                    case CURR :
                        size = sizeof(ApplVar.AP.Curr);
                        record = (char *)&ApplVar.AP.Curr;
                        break;
                    case DISC :
                        size = sizeof(ApplVar.AP.Disc);
                        record = (char *)&ApplVar.AP.Disc;
                        break;
                    case DRAW :
                        size = sizeof(ApplVar.AP.Draw);
                        record = (char *)&ApplVar.AP.Draw;
                        break;
                    case PBF :
                        size = sizeof(ApplVar.AP.Pb);
                        record = (char *)&ApplVar.AP.Pb;
                        break;
                    case PORA :
                        size = sizeof(ApplVar.AP.PoRa);
                        record = (char *)&ApplVar.AP.PoRa;
                        break;
                    case TEND :
                        size = sizeof(ApplVar.AP.Tend);
                        record = (char *)&ApplVar.AP.Tend;
                        break;
                    case MODI :
                        size = sizeof(ApplVar.AP.Modi);
                        record = (char *)&ApplVar.AP.Modi;
                        break;
                    case TAX :
                        size = sizeof(ApplVar.AP.Tax);
                        record = (char *)&ApplVar.AP.Tax;
                        break;
                    case CLERK :
                        size = sizeof(ApplVar.AP.Clerk);
                        record = (char *)&ApplVar.AP.Clerk;
                        break;
                    case GROUP :
                        size = sizeof(ApplVar.AP.Group);
                        record = (char *)&ApplVar.AP.Group;
                        break;
                    case DEPT :
                        size = sizeof(ApplVar.AP.Dept);
                        record = (char *)&ApplVar.AP.Dept;
                        break;
                    case PLU1 :
                        size = sizeof(ApplVar.AP.Plu);
                        record = (char *)&ApplVar.AP.Plu;
                        break;
                    case OFFER:
                        size = sizeof(ApplVar.AP.OFFPrice);
                        record = (char *)&ApplVar.AP.OFFPrice;
                        break;
                    case AGREE :
                        size = sizeof(ApplVar.AP.Agree);
                        record = (char *)&ApplVar.AP.Agree;
                        break;
                    case BLOCKIC:		//ccr chipcard
                        size = sizeof(ApplVar.AP.ICBlock);
                        record = (char *)&ApplVar.AP.ICBlock;
                        break;
                    case CHIPCARDSET:	//ccr chipcard
                        size = sizeof(ApplVar.ICCardSet);
                        record = (char *)&ApplVar.ICCardSet;
                        break;
                    case REMOTESETTIME :
                        if (TESTBIT(ApplVar.MyFlags,ZREPORT))   /* z report taken ? */
                        {
                            CheckError(13);
                            *((WORD *)PCBuffer) = 0xff;  /* id means end */
                            SendRecord(PCBuffer, 2);   /* send end record */
                        }
                        else{
                            ApplVar.ErrorNumber=0;
                            memcpy(&EntryBuffer[ENTRYSIZE - 9],&PCBuffer[3],8);
                            Appl_EntryCounter = 8;
                            sPC = TIME_DATE;
                            TIME_DATE = 2;
                            NewTimeDate(1);
                            TIME_DATE = sPC;
                            memcpy(&EntryBuffer[ENTRYSIZE - 7],&PCBuffer[11],6);
                            Appl_EntryCounter = 6;
                            NewTimeDate(2);
                            if(ApplVar.ErrorNumber!=0)
                            {
                                CheckError(13);
                                *((WORD *)PCBuffer) = 0xff;  /* id means end */
                                SendRecord(PCBuffer, 2);   /* send end record */
                            }
                            else
                            {
                                CheckError(0);
                                *((WORD *)PCBuffer) = 0;  /* id means end */
                                SendRecord(PCBuffer, 2);   /* send end record */
                            }
                        }
                        break;
                        //ccr2016-05-31>>>>>>>>>>>>>>>>>>>
#if defined(CASE_INDIA)
                    case UPDATEECRNO:     //更新收款机号
                        size = 2;
                        record = (char *)&ApplVar.AP.Flag[14];
                        break;
                    case UPDATERECNO:      //更新收据号
                        size = RECNUM;
                        record = (char *)&ApplVar.ReceiptNumber;
                        memset(PCBuffer+3+RECNUM,0,8);
                        BCDValueToULong(PCBuffer+3,&sAddr);
                        if (sAddr)  sAddr--;
                        ULongToBCDValue(PCBuffer+3,sAddr);
                        break;
#endif
                        //ccr2016-05-31<<<<<<<<<<<<<<<<<<<

                    default :
                        break;
                }//switch(type)
                if (size)
                {//更新收款机的配置数据
                    if (PCBuffer[0] == 'U')   /* update */
                    {
                        sEcrNo = REGISTER;
                        sLocate = LOCATION;
                        sPC = COMPUTER;
                        sBAR = BARCODE;
                        sBAL = BALANCE;
                        sEXTMem = EXTRAMN;
                        if (type == CHIPCARDSET)
                            sAddr = ApplVar.ICCardSet.PosCode;
                        memcpy(record, &PCBuffer[3], size);
                        if (type == CHIPCARDSET)
                            ApplVar.ICCardSet.PosCode = sAddr;
                        if (type == SYSFLG)
                        {
                            InitSysFlag();
                            COMPUTER = sPC;
                            BARCODE = sBAR;
                            BALANCE = sBAL;
                            REGISTER = sEcrNo;
                            LOCATION = sLocate;
                            EXTRAMN =  sEXTMem;
                        }
                        else if (type == START)
                        {//更新地址分配,收到的地址分配为相对地址!!!!
                            ClsXRam();
                            if	(ResetStartAddress(true)==false)
                            {
                                Bell(2);
                                DispSt1C(Msg[WJKJYJIE].str);
                                RJPrint(0,Msg[WJKJYJIE].str);


                                //重新计算内存中可容纳的单品数目
                                sAddr = ApplVar.AP.StartAddress[AddrEndP] - ApplVar.SIZE_EXTRAM;

                                sAddr /= pluRecordSize;
                                sAddr++;//为需要减少的单品数目
                                sMax = sAddr;
                                sAddr *= pluRecordSize;//所减少的单品所占用的空间
                                if (sMax<ApplVar.AP.Plu.Number)
                                {
                                    ApplVar.AP.Plu.Number -= sMax;
                                    if (ApplVar.AP.Plu.RNumber>ApplVar.AP.Plu.Number)
                                        ApplVar.AP.Plu.RNumber = ApplVar.AP.Plu.Number;
                                }
                                else
                                {
                                    RJPrint(0,(char*)cMessageE18);//"!!!! FAT(PLU) ERROR !!!!"
                                    ApplVar.AP.Plu.Number = ApplVar.AP.Plu.RNumber = 0;
                                }

                                for (sMax = AddrPLU+1;sMax<AddrMAX;sMax++)
                                    ApplVar.AP.StartAddress[sMax] -= sAddr;


                            }
                            ApplVar.MyFlags = 0;
                            //Save_ApplRam();
                        }
                        else if (type == KEY0 || type == KEY32)
                        {
                            DeptPluKeysCount();

                            ApplVar.AP.FirmKeys[ID_xDEPT] = 0xff;
                            ApplVar.AP.FirmKeys[ID_DATE] = 0xff;
                            ApplVar.AP.FirmKeys[ID_RJFEED] = 0xff;
                            ApplVar.AP.FirmKeys[ID_CLEAR] = 0xff;
                            ApplVar.AP.FirmKeys[ID_LOCK] = 0xff;

                            for (id=0;id<MAXKEYB;id++)
                            {
                                switch (ApplVar.AP.KeyTable[id]){
                                    case SHIFT1:
                                        ApplVar.AP.FirmKeys[ID_xDEPT] = id;
                                        break;
                                    case DATETIME:
                                        ApplVar.AP.FirmKeys[ID_DATE] = id;
                                        break;
                                    case JPF:
                                    case RPF:
                                        ApplVar.AP.FirmKeys[ID_RJFEED] = id;
                                        break;
                                    case CLEAR:
                                        ApplVar.AP.FirmKeys[ID_CLEAR] = id;
                                        break;
                                    case MODELOCK:
                                        ApplVar.AP.FirmKeys[ID_LOCK] = id;
                                        break;
                                }
                            }
                        }
                        Save_Config(true);//ccr2014-10-30
                        break;
                    }
                    //为'D'命令,把配置数据发送给主机
                    if (type == START)//转换为相对地址发送给主机
                        ResetStartAddress(false);
                    *((WORD *)PCBuffer) = type;  /* id */
                    memcpy(PCBuffer + 2, record, size);
                    SendRecord(PCBuffer, size + 2);   /* send record */
                    if (type == START)//恢复为绝对地址
                    {
                        ResetStartAddress(true);
                        Init_Flow();
                    }
                    Save_Config(true);//ccr2014-10-30
                }
                else if (type == PBTRAIL)    /* ApplVar.PB Extra Trailer */
                {
                    if (ApplVar.AP.Pb.NumberOfPb && TESTBIT(ApplVar.AP.Pb.Random, BIT7))
                    {
                        RamOffSet = (ApplVar.AP.Pb.Random & 0x0f) + ApplVar.AP.Pb.Text + ApplVar.AP.Pb.AmtSize + 6;
                        if (TESTBIT(PBINFO, BIT7))      /* Discount item stored ? */
                            RamOffSet += ((ApplVar.AP.Pb.AmtSize + 1) * 2);
                        RamOffSet *= ApplVar.AP.Pb.NumberOfPb;
                        RamOffSet += ApplVar.AP.StartAddress[AddrPBt] ;

                        start = *((WORD *) &PCBuffer[3]);
                        if (start < 50)
                        {
                            start *= (PRTLEN+1);
                            RamOffSet += start;
                            if (PCBuffer[0] == 'U')    /* Update ? */
                            {
                                WriteRam(&PCBuffer[5], 5 * (PRTLEN+1));     /* write Block */
                                break;
                            } else
                            {
                                *((WORD *)PCBuffer) = PBTRAIL;
                                start = 0;
                                for(;;)
                                {
                                    ReadRam(&PCBuffer[4], 5 * (PRTLEN+1));     /* read Block */
                                    *((WORD *)(PCBuffer + 2)) = start;
                                    /* send record */
                                    if (!SendRecord(PCBuffer, (5 * (PRTLEN+1)) + 4))
                                        break;
                                    start += 5;
                                    if (start > 45)
                                        break;
                                }
                            }
                        }
                    }
                } else if (type == CCHAR) /* defined(CASE_FORHANZI) characters ? */
                {
                    RamOffSet = 0 ;                                 /* always located at ram start */
                    if (PCBuffer[0] == 'U')    /* Update ? */
                    {
                        start = *((WORD *) &PCBuffer[3]);             /* character number */
                        if (start < ApplVar.AP.Options.ChinaChar) {
                            start *= 34;
                            RamOffSet += start;
                            WriteRam(&PCBuffer[5], 34);     /* write character */
                        }
                        break;
                    } else
                    {
                        *((WORD *)PCBuffer) = CCHAR;
                        for(start = 0; start < ApplVar.AP.Options.ChinaChar; start++)
                        {
                            ReadRam(&PCBuffer[4], 34);     /* read char */
                            *((WORD *)(PCBuffer + 2)) = start;
                            /* send record */
                            if (!SendRecord(PCBuffer, 34 + 4))
                                break;
                        }
                    }
                }
                else
                {
                    if (type == ADDPLU || type == DELPLU || type == UPDPLU)
                    {
                        if (PCBuffer[0] != 'U')
                            break;
                        if (ApplVar.AP.Plu.RandomSize && ApplVar.AP.Plu.Number)
                        {
                            ApplVar.Key.Code = 0;   /* suppress repeat */
                            memcpy(&ApplVar.Plu, &PCBuffer[3], sizeof(ApplVar.Plu));
                            ApplVar.PluNumber = GetPluNumber(1, ApplVar.Plu.Random);        /* fill ApplVar.PluNumber with number */
                            switch(type) {
                                case DELPLU:
                                    if (ApplVar.PluNumber)
                                    {
                                        SETBIT(ApplVar.MyFlags,CONFIGECR);
                                        CheckRandomPlu(1, 1);       /* delete plu */
                                    }
                                    break;
                                case ADDPLU:
                                    if (!ApplVar.PluNumber)  /* Not Found then move and Add */
                                    {
                                        CheckRandomPlu(0, 1);//ccr2014-12-10
                                        //ccr2014-12-10if (CheckRandomPlu(0, 1))
                                        //ccr2014-12-10	ApplVar.PluNumber = GetPluNumber(0, ApplVar.Plu.Random);        /* fill ApplVar.PluNumber with number */
                                        SETBIT(ApplVar.MyFlags,CONFIGECR);
                                        break;//ccr2014-12-10
                                    }
                                case UPDPLU:
                                    if (ApplVar.PluNumber)
                                    {
                                        ApplVar.PluNumber--;
                                        WritePlu();
                                        SETBIT(ApplVar.MyFlags,CONFIGECR);
                                    }
                                default:
                                    break;
                            }
                        }
                        if (type != UPDPLU)
                        {
                            *((WORD *)PCBuffer) = 0;  /* id means end */
                            SendRecord(PCBuffer, 2);   /* send end record */
                        }
                        break;
                    }
                    else if (type > PBTOT)
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            PbTotal(type - PBTOT, 0);
                            if (!(PBINFO & 0x04) && !(PBPRINT & 0x80))       /* open numbers ? */
                                memcpy(ApplVar.PB.Random, &PCBuffer[3], 7);
                            memcpy(ApplVar.PB.Text, &PCBuffer[10], sizeof(ApplVar.PB.Text));
                            PbTotal(type - PBTOT, 3);
                            break;
                        }
                        id = PBTOT + 1;
                        if ((ApplVar.AP.Pb.Random & 0x0f) || ApplVar.AP.Pb.Text)
                            sMax = ApplVar.AP.Pb.NumberOfPb;
                        else
                            sMax = 0;                /* Not active */
                        size = sizeof(ApplVar.PB.Random) + sizeof(ApplVar.PB.Text);
                        record = (char *)ApplVar.PB.Random;
                    }
                    else if (type > BLOCKIC)    /* Chipcard for block ? ccr chipcard */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.ICBlockNumber = type - BLOCKIC - 1;
                            if (ApplVar.ICBlockNumber < ApplVar.AP.ICBlock.Number)
                            {
                                memcpy(&ApplVar.ICBlock, &PCBuffer[3], sizeof(ApplVar.ICBlock));
                                WriteICBlock();
                            }
                            break;
                        }
                        id = BLOCKIC + 1;
                        sMax = ApplVar.AP.ICBlock.Number;
                        size = sizeof(ApplVar.ICBlock);
                        record = (char *)&ApplVar.ICBlock;

                    }
                    else if (type > PLU1)   /* plu */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.PluNumber = type - PLU1 - 1;
                            if (ApplVar.PluNumber < ApplVar.AP.Plu.Number)
                            {
                                memcpy(&ApplVar.Plu, &PCBuffer[3], sizeof(ApplVar.Plu));
                                WritePlu();
                                if (ApplVar.PluNumber==0)//ccr2015-02-26
                                    ApplVar.AP.Plu.RNumber=0;//ccr2015-02-26
                                if (ApplVar.AP.Plu.RandomSize &&    /* set PLU's used */
                                        (ApplVar.PluNumber >= ApplVar.AP.Plu.RNumber))
                                {
                                    ApplVar.AP.Plu.RNumber = (ApplVar.PluNumber + 1);
                                }
                                SETBIT(ApplVar.MyFlags,CONFIGECR);//ccr2016-01-15
                            }
                            break;
                        }
                        id = PLU1 + 1;
                        if (ApplVar.AP.Plu.RandomSize)    /* set PLU's used */
                            sMax = ApplVar.AP.Plu.RNumber;
                        else
                            sMax = ApplVar.AP.Plu.Number;
                        size = sizeof(ApplVar.Plu);
                        record = (char *)&ApplVar.Plu;
                    }
                    else if (type > DEPT)   /* dept */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.DeptNumber = type - DEPT - 1;
                            memcpy(&ApplVar.Dept, &PCBuffer[3], sizeof(ApplVar.Dept));
                            WriteDept();
                            break;
                        }
                        id = DEPT + 1;
                        sMax = ApplVar.AP.Dept.Number;
                        size = sizeof(ApplVar.Dept);
                        record = (char *)&ApplVar.Dept;
                    }
                    else if (type > GROUP)   /* group */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.GroupNumber = type - GROUP - 1;
                            memcpy(&ApplVar.Group, &PCBuffer[3], sizeof(ApplVar.Group));
                            WriteGroup();
                            break;
                        }
                        id = GROUP + 1;
                        sMax = ApplVar.AP.Group.Number;
                        size = sizeof(ApplVar.Group);
                        record = (char *)&ApplVar.Group;
                    }
                    else if (type > OFFER)   /* OFFER */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.OFFNumber = type - OFFER -1;
                            memcpy(&ApplVar.OFFPrice, &PCBuffer[3], sizeof(ApplVar.OFFPrice));
                            WriteOFFPrice();
                            break;
                        }
                        id = OFFER+1;
                        sMax = ApplVar.AP.OFFPrice.Number;
                        size = sizeof(ApplVar.OFFPrice);
                        record = (char *)&ApplVar.OFFPrice;
                    }
                    else if (type > CLERK)   /* clerk */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.ClerkNumber = type - CLERK;
                            memcpy(&ApplVar.Clerk, &PCBuffer[3], sizeof(ApplVar.Clerk));
                            WriteClerk();
                            break;
                        }
                        id = CLERK;
                        sMax = ApplVar.AP.Clerk.Number + 1;
                        size = sizeof(ApplVar.Clerk);
                        record = (char *)&ApplVar.Clerk;
                    }
                    else if (type > TAX)    /* modifier */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.TaxNumber = type - TAX - 1;
                            memcpy(&ApplVar.Tax, &PCBuffer[3], sizeof(ApplVar.Tax));
                            WriteTax();
                            break;
                        }
                        id = TAX + 1;
                        sMax = ApplVar.AP.Tax.Number;
                        size = sizeof(ApplVar.Tax);
                        record = (char *)&ApplVar.Tax;
                    }
                    else if (type > SALPER)    /* sales person */
                    {
#if (salNumber)
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.SalPerNumber = type - SALPER;
                            memcpy(&ApplVar.SalPer, &PCBuffer[3], sizeof(ApplVar.SalPer));
                            WriteSalPer();
                            break;
                        }
#endif
                        id = SALPER;
                        sMax = ApplVar.AP.SalPer.Number+1;
                        size = sizeof(ApplVar.SalPer);
                        record = (char *)&ApplVar.SalPer;
                    }
                    else if (type > MODI)    /* modifier */
                    {
                        if (PCBuffer[0] == 'U')   /* update */
                        {
                            ApplVar.ModiNumber = type - MODI - 1;
                            memcpy(&ApplVar.Modi, &PCBuffer[3], sizeof(ApplVar.Modi));
                            WriteModi();
                            break;
                        }
                        id = MODI + 1;
                        sMax = ApplVar.AP.Modi.Number;
                        size = sizeof(ApplVar.Modi);
                        record = (char *)&ApplVar.Modi;
                    }
                    else
                    {
                        id = (type / 100) * 100 + 1;
                        if (type > TEND)
                        {
                            if (PCBuffer[0] == 'U')   /* update */
                            {
                                ApplVar.TendNumber = type - TEND - 1;
                                memcpy(&ApplVar.Tend, &PCBuffer[3], sizeof(ApplVar.Tend));
                                WriteTender();
                                break;
                            }
                            sMax = ApplVar.AP.Tend.Number;
                            size = sizeof(ApplVar.Tend);
                            record = (char *)&ApplVar.Tend;
                        }
                        else if (type > PORA)
                        {
                            if (PCBuffer[0] == 'U')   /* update */
                            {
                                ApplVar.PoRaNumber = type - PORA - 1;
                                memcpy(&ApplVar.PoRa, &PCBuffer[3], sizeof(ApplVar.PoRa));
                                WritePoRa();
                                break;
                            }
                            sMax = ApplVar.AP.PoRa.Number;
                            size = sizeof(ApplVar.PoRa);
                            record = (char *)&ApplVar.PoRa;
                        }
                        else if (type > PBF)
                        {
                            if (PCBuffer[0] == 'U')   /* update */
                            {
                                ApplVar.PbFNumber = type - PBF - 1;
                                memcpy(&ApplVar.PbF, &PCBuffer[3], sizeof(ApplVar.PbF));
                                WritePbF();
                                break;
                            }
                            sMax = ApplVar.AP.Pb.Number;
                            size = sizeof(ApplVar.PbF);
                            record = (char *)&ApplVar.PbF;
                        }
                        else if (type > DRAW)
                        {
                            if (PCBuffer[0] == 'U')   /* update */
                            {
                                ApplVar.DrawNumber = type - DRAW - 1;
                                memcpy(&ApplVar.Draw, &PCBuffer[3], sizeof(ApplVar.Draw));
                                WriteDrawer();
                                break;
                            }
                            sMax = ApplVar.AP.Draw.Number;
                            size = sizeof(ApplVar.Draw);
                            record = (char *)&ApplVar.Draw;
                        }
                        else if (type > DISC)
                        {
                            if (PCBuffer[0] == 'U')   /* update */
                            {
                                ApplVar.DiscNumber = type - DISC - 1;
                                memcpy(&ApplVar.Disc, &PCBuffer[3], sizeof(ApplVar.Disc));
                                WriteDisc();
                                break;
                            }
                            sMax = ApplVar.AP.Disc.Number;
                            size = sizeof(ApplVar.Disc);
                            record = (char *)&ApplVar.Disc;
                        }
                        else if (type > CURR)
                        {
                            if (PCBuffer[0] == 'U')   /* update */
                            {
                                ApplVar.CurrNumber = type - CURR - 1;
                                memcpy(&ApplVar.Curr, &PCBuffer[3], sizeof(ApplVar.Curr));
                                WriteCurr();
                                break;
                            }
                            sMax = ApplVar.AP.Curr.Number;
                            size = sizeof(ApplVar.Curr);
                            record = (char *)&ApplVar.Curr;
                        }
                        else if (type > CORREC)
                        {
                            if (PCBuffer[0] == 'U')   /* update */
                            {
                                ApplVar.CorrecNumber = type - CORREC - 1;
                                memcpy(&ApplVar.Correc, &PCBuffer[3], sizeof(ApplVar.Correc));
                                WriteCorrec();
                                break;
                            }
                            sMax = ApplVar.AP.Correc.Number;
                            size = sizeof(ApplVar.Correc);
                            record = (char *)&ApplVar.Correc;
                        }
                        else
                            break;
                    }
                    start = type - id;
                    end = *((WORD *)(PCBuffer + 3)) - id + 1;
                    if (end > sMax)
                        end = sMax;
                    MemSet(record, size, 0);    /* clear memory */
                    while(start < end)
                    {//将指定文件的一条记录数据发送给主机
                        memset(SysBuf,' ',INPUTWIDTH);
                        if (type > PBTOT)
                        {
                            CopyFrStr(SysBuf,Msg[SETPBINF].str);
                            PbTotal(start + 1, 0);  /* read */
                        }
                        else if (type > BLOCKIC)//ccr chipcard
                        {
                            CopyFrStr(SysBuf,Msg[GUASHIIC].str);
                            ApplVar.ICBlockNumber = start;
                            ReadICBlock();
                        }
                        else if (type > PLU1)
                        {
                            CopyFrStr(SysBuf,Msg[SETPLU].str);
                            ApplVar.PluNumber= start;
                            ReadPlu();
                        }
                        else if (type > DEPT)
                        {
                            CopyFrStr(SysBuf,Msg[SETDEPT].str);
                            ApplVar.DeptNumber = start;
                            ReadDept();
                        }
                        else if (type > GROUP)
                        {
                            CopyFrStr(SysBuf,Msg[SETGROUP].str);
                            ApplVar.GroupNumber = start;
                            ReadGroup();
                        }
                        else if (type > OFFER)
                        {
                            CopyFrStr(SysBuf,Msg[SETOFF].str);
                            ApplVar.OFFNumber = start;
                            ReadOFFPrice();
                        }
                        else if (type > CLERK)
                        {
                            CopyFrStr(SysBuf,Msg[SETCLERK].str);
                            ApplVar.ClerkNumber = start;
                            ReadClerk();
                        }
                        else if (type > TAX)    /* tax */
                        {
#if !defined(CASE_FORHANZI)
                            CopyFrStr(SysBuf,Msg[SETTAX].str);
#else
                            CopyFrStr(SysBuf,"TAX");
#endif
                            ApplVar.TaxNumber = start;
                            ReadTax();
                        }
                        else if (type > SALPER)    /* salesperson */
                        {
                            CopyFrStr(SysBuf,Msg[SETSALER].str);
                            ApplVar.SalPerNumber = start;
                            ReadSalPer();
                        }
                        else if (type > MODI)    /* modifier */
                        {
                            CopyFrStr(SysBuf,Msg[SETMODIF].str);
                            ApplVar.ModiNumber = start;
                            ReadModi();
                        }
                        else if (type > TEND)    /* tender */
                        {
                            CopyFrStr(SysBuf,Msg[SETTEND].str);
                            ApplVar.TendNumber = start;
                            ReadTender();
                        }
                        else if (type > PORA)    /* pora */
                        {
                            CopyFrStr(SysBuf,Msg[SETPORA].str);
                            ApplVar.PoRaNumber = start;
                            ReadPoRa();
                        }
                        else if (type > PBF)    /* pb */
                        {
                            CopyFrStr(SysBuf,Msg[SETPBF].str);
                            ApplVar.PbFNumber = start;
                            ReadPbF();
                        }
                        else if (type > DRAW)    /* drawer */
                        {
                            CopyFrStr(SysBuf,Msg[SETDRAWER].str);
                            ApplVar.DrawNumber = start;
                            ReadDrawer();
                        }
                        else if (type > DISC)    /* discount */
                        {
                            CopyFrStr(SysBuf,Msg[SETDISC].str);
                            ApplVar.DiscNumber = start;
                            ReadDisc();
                        }
                        else if (type > CURR)    /* currency */
                        {
                            CopyFrStr(SysBuf,Msg[SETCURR].str);
                            ApplVar.CurrNumber = start;
                            ReadCurr();
                        }
                        else                  /* correction */
                        {
                            CopyFrStr(SysBuf,Msg[SETCORR].str);
                            ApplVar.CorrecNumber = start;
                            ReadCorrec();
                        }
#if (SCRSTATDBL)
                        WORDtoASC(SysBuf+SCRDBLWD-1,start);
                        DispOnStatDBL(SysBuf,0);
#else
                        WORDtoASC(SysBuf+INPUTWIDTH-1,start);
                        DispSt2(SysBuf);//RG-DISPLAY
#endif

                        *((WORD *)PCBuffer) = start + id;  /* id */
                        memcpy(PCBuffer + 2, record, size);
                        if (!SendRecord(PCBuffer, size + 2))   /* send record */
                            break;
                        start++;
                    }
                }
                *((WORD *)PCBuffer) = 0;  /* id means end */
                SendRecord(PCBuffer, 2);   /* send end record 发送结束标示 */
            }
            break;
        case 'C':           /* clear memory ? */
#if (MESSTOHOST==1)
            if (type == 0x4d4f) /* command "COM":Copy Of Mess */
            {
                for (id=0;id<HZMESSMAX;id++)
                {
                    memset(SysBuf,' ',sizeof(SysBuf));
                    WORDtoASC(SysBuf+4,id);
                    strcpy(SysBuf+6,Msg[id].str);
                    if (!SendRecord(SysBuf,strlen(SysBuf)+1))
                        return;
                }
                *((WORD *)PCBuffer) = 0;  /* id means end */
                SendRecord(PCBuffer, 2);   /* send end record */
                return;
            }
#endif
        case 'I':           /* init */
            {
                if (type == 0x544e) /* command "INT" */
                {
                    InitApplication(true);//强制进行初始化
                }
                else if (type == 0x524c) /* command "CLR" */
                {
                    id = ApplVar.CentralLock;
                    start = ApplVar.ClerkNumber;
                    end = ApplVar.ClerkLock;
                    sMax = ApplVar.SalPerNumber;
                    //				ClearApplMemory();
                    ClearAllReport();
                    ApplVar.CentralLock = id;
                    ApplVar.ClerkNumber = start;
                    ApplVar.ClerkLock = end;
                    ApplVar.SalPerNumber = sMax;
                }
                //		else
                //				break;
                *((WORD *)PCBuffer) = 0;  /* id means end */
                SendRecord(PCBuffer, 2);   /* send end record */
                ARROWS = 0x01;
            }
            return;
        case 'R':           /* command for getting user report */
        case 'S':           /* command for getting system report */
            {
                ApplVar.RepComputer = 0;
                ApplVar.FReport = 0;
                MemSet(SysBuf + 7, 24, ' ');
                *((WORD *)(SysBuf)) = 1;        /* indicate counters */
                SysBuf[2] = 1;      /* indicate text */
                SysBuf[3] = 0;
                SysBuf[4] = 0;
                *((WORD *)(SysBuf + 5)) = 0;
                GetReceiptNumber(SysBuf + 7);
                SysBuf[15] = 'L';      /* location number */
                SysBuf[16] = LOCATION;
                SysBuf[19] = 'R';      /* register number */
                SysBuf[20] = REGISTER;
                SysBuf[24] = 'Z';
                HEXtoASC(SysBuf + 25,(char *)&ApplVar.ZCount, 3);
                SendRecord(SysBuf, 31);      /* send counter record */
                SysBuf[0] = 2;              /* indicate time & date */
                memcpy(SysBuf + 7, DateAsci, 15);
                SysBuf[22] = ' ';
                memcpy(SysBuf + 23, TimeAsci, 8);
                SendRecord(SysBuf, 31);      /* send time-date record */

                SysBuf[0] = PCBuffer[0];// 'S' or 'R'
                SysBuf[1] = PCBuffer[1];// 'Z' or 'X' or 'C'

                if (ApplVar.FRegi)
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
                else if (!ApplVar.ErrorNumber)
                {
                    ApplVar.RepComputer = PCBuffer[1];        /* X or Z (only Z is checked) */
                    if (ApplVar.RepComputer == 'Z' || ApplVar.RepComputer == 'C')
                        ApplVar.FReport = Z;
                    else
                        ApplVar.FReport = X;
                    if (PCBuffer[0] == 'R')       /* user report ? */
                    {//PCBuffer[2]=ReportNumber
                        ApplVar.ReportNumber = PCBuffer[2];
                        GetReport(ApplVar.ReportNumber);
                    }
                    else
                    {
                        Appl_EntryCounter = 4;
                        memcpy(EntryBuffer + ENTRYSIZE - 5,PCBuffer + 2, 4);
                        GetSystemReport(0);
                        ClearEntry(false);
                    }
                }

                ApplVar.RepComputer = 0;
                ApplVar.FReport = 0;
                *((WORD *)(PCBuffer)) = 0;        /* indicate end */
                PCBuffer[2] = 1;      /* indicate text */
                PCBuffer[3] = 0;
                PCBuffer[4] = 0;
                *((WORD *)(PCBuffer + 5)) = 0;
                if (ApplVar.ErrorNumber)        /* error occurred */
                {
                    *((WORD *)(PCBuffer + 5)) = ApplVar.ErrorNumber;
                    strcpy(PCBuffer + 7, Msg[ApplVar.ErrorNumber - 1].str);
                    SendRecord(PCBuffer, 7 + strlen(Msg[ApplVar.ErrorNumber - 1].str));
                    ApplVar.ErrorNumber=0;
                }
                else
                    SendRecord(PCBuffer, 7);
            }
            break;
        case 'F'://[01][03][46][00][00][B7];log  [01][03][46][FF][FF][B9]
            {
                //ccr2016-06-12>>>>>>>>>>>>>>>
                if (type==0xffff)
                {
                    ApplVar.FlowHeader.ReadP=ApplVar.FlowHeader.HeadP;
                    type=0;
                }
                //ccr2016-06-12<<<<<<<<<<<<<<<
#if defined(CASE_INDONESIA)
                SendMultiLog();
#else
                if(type!=0 || ApplVar.FlowHeader.ReadP==ApplVar.FlowHeader.NextNP)
                {
                    *(WORD *)PCBuffer = 0;
                    SendRecord(PCBuffer, 2);
                    return;
                }
                *(PCBuffer+1) = ECRNOLOG;
                *(PCBuffer+2) = REGISTER;
                *(PCBuffer+3) = LOCATION;
                SendRecord(PCBuffer, 4);
                for(;;)
                {
                    size = Read_Flow(PCBuffer+1);	   //lyq2003
                    if(size <= 0)
                    {
                        *(WORD *)PCBuffer = 0;
                        SendRecord(PCBuffer, 2);
                        if(size==-1)
                        {
                            CheckError(ERROR_ID(CWXXI48));
                        }
                        return;
                    }
                    else
                    {
                        if(!(SendRecord(PCBuffer, size+1)))
                        {
                            SUB_SCANFP(size);
                            CheckError(ERROR_ID(CWXXI84));
                            return;
                        }
                    }
                }
#endif
            }
            return;
#if PCREMOTE==1
        case 'K'://comman for emulate keyboard;
            {
                if ((type & 0xff)==ApplVar.AP.FirmKeys[ID_CLEAR])
                {
                    ClearEntry(false);
                    ApplVar.ErrorNumber=0;
                }
                if (KeyFrHost!=0xff || (ApplVar.ErrorNumber && (type & 0xff) !=0))
                {
                    PCBuffer[0] = 0x8A;
                    PCBuffer[1] = ApplVar.ErrorNumber & 0x7f;
                    SendRecord(PCBuffer, 2);
                    return;
                }
                else
                {
                    if ((type & 0xff)==PRNONOFF)
                    {
                        if (TESTBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER))
                            RESETBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER);
                        else
                            SETBIT(ApplVar.MyFlags, PRNONPB+ENPRINTER);
                    }
                    else
                    {
                        KeyFrHost = type & 0xff;
                    }

                    PCBuffer[0] = 0x85;
                    PCBuffer[1] = Now.year&0xff;
                    PCBuffer[2] = Now.month;
                    PCBuffer[3] = Now.day;
                    PCBuffer[4] = Now.hour;
                    PCBuffer[5] = Now.min;
                    PCBuffer[6] = ((ApplVar.MyFlags & (PRNONPB+ENPRINTER))==0);
                    PCBuffer[7] = ApplVar.CentralLock;
                    SendRecord(PCBuffer, 8);
                }
            }
            return;
#endif
        case 'P':
            ApplVar.AP.Plu.RNumber = (CWORD(PCBuffer[1]));
            return;

#if((defined(CASE_EJSD) || defined(CASE_EJFLASH)))	//cc 20071026
        case 'E'://"EJ"+4字节的地址
            sMax = 0;
            if (PCBuffer[1]=='J')
            {// 无格式发送指定地址上的EJ数据 //
                sAddr =  CLONG(PCBuffer[2])+EJMEMORYFROM;
                if  (TESTBIT(ApplVar.EJVar.ContFlag,EJDOCHEAD))
                    sAddrEnd = ApplVar.EJHeader.AddrLastEJ;
                else
                    sAddrEnd = ApplVar.EJHeader.AddrNextEJ;

                if (sAddr<sAddrEnd)//ccr091021 ApplVar.MMCSIZEMAX)
                {
                    if (sAddrEnd-sAddr>250)
                        sMax = 250;
                    else
                        sMax = (sAddrEnd-sAddr);
                    memset(PCBuffer+2, 0xff,sMax);
                    if (sMax && ReadFromMMC(PCBuffer+2, sAddr,sMax))
                    {
#if defined(CASE_EJFLASH)
                        if (sAddr==EJMEMORYFROM)//逻辑不严谨,实际无错
                            memcpy(PCBuffer+2,&ApplVar.EJHeader,HEADSIZE);
#endif

                        if (SendRecord(PCBuffer, sMax+2))   /* send datas */
                            ApplVar.EJHeader.AddrEJRead = sAddr+sMax-EJMEMORYFROM;//为相对地址
                    }
                    else
                        sMax =0;
                }
            }
            else if (PCBuffer[1]=='H')
            {// 发送EJ头数据大小 //
                memcpy(PCBuffer+2,(char*)&ApplVar.EJHeader,HEADSIZE);
                SendRecord(PCBuffer,HEADSIZE+2);
                sMax = 1;
                break;
            }

            if (sMax==0)
            {
                CWORD(PCBuffer[0]) = 0;
                SendRecord(PCBuffer,2);
            }
            //			else
            //				DisplayHex((char*)&sAddr,4);
            break;
#endif
        default :
            return;
    }

    if (type > PLU1  && type<AGREE || (type >= UPDPLU && type <= REPPLU))
    {
        ApplVar.RepeatCount = 0 ;
        ApplVar.PluNumber = save;
        ReadPlu();
    } else if (type > DEPT  && type<PLU1)
    {
        ApplVar.DeptNumber = save;
        ReadDept();
    } else if (type > CLERK && type<OFFER)
    {
        ApplVar.ClerkNumber = save;

        if(save)
        {
            ReadClerk();
        }

    } else if (type > SALPER  && type<TAX)
    {
        ApplVar.SalPerNumber = save;
        ReadSalPer();
    }
    if(TRAINING)
    {
        ApplVar.FTrain = 1 ;
    }
    else
    {
        ApplVar.FTrain = 0 ;
    }
}
#else
//jdb2018-04-19
static signed char TcpAnswerToHost(char cmd,unsigned short data)
{
    char sbuf[6];

    sbuf[0]=cmd;
    sbuf[1]=2;
    *((WORD *)(sbuf+2)) = data;
    return  STH(sbuf, 4);/* send end record */

}

void TcpSaveGraphic(int sAddr,char *grphic,int length)//ccr2014-03-25
{
}

static signed char TcpRecordToHost(char cmd,unsigned short type,unsigned char *data,unsigned short size)
{
    PCBuffer[0]=cmd;
    PCBuffer[1]=size+2;
    *((WORD *)(PCBuffer+2)) = type;
    memcpy(PCBuffer+4,data,size);

    return  STH(PCBuffer,size+4);/* send end record */
}

#define POINTERMAX 2 //jdb2018-05-16

static int TcpReadZport(BYTE *buf)
{
    WORD skip, next, count, indx, indx1, indx2;
    unsigned long saveOffset;
	BYTE ret;

    skip = 0;
	indx1 = 0;
	indx2 = 0;
	#if 1
    if (TESTBIT(ApplVar.Size.Periods, BIT0))      /* calculate size of 1 pointer */
        skip += ApplVar.Size.Length;
	indx2 += ApplVar.Size.Length;
	if (TESTBIT(ApplVar.Size.Periods, BIT1))
        skip += ApplVar.Size.Length;
	#else
    skip += ApplVar.Size.Length;
	#endif
	next = skip;
	
    switch (ApplVar.PointerType)
    {
        case 0://统计总销售
            skip = 0;
            break;
        case 1:         /* Clerks */
            if (!ApplVar.AP.Clerk.Number)
                return;
            skip *= (ApplVar.ClerkNumber - 1);
			indx1 = skip;
			indx2 += indx1;
            break;
        case 2:         /* Hour */
            if (!ApplVar.AP.Zone.Number)
                return;
            skip *= ApplVar.Zone;
			indx1 = skip;
			indx2 += indx1;
            break;
        default:
            break;
    }
	next += skip;
    saveOffset = RamOffSet;
    //RamOffSet += skip;
	RamOffSet += indx1;
	
    count = 0;
    if (TESTBIT(ApplVar.Size.Periods, BIT0)) {
        ret = ReadTotal();
        indx = count;
	
		//if(!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))//jdb2018-05-22 数据为零不上传
		if(ret == 0x02 || ret == 0x01)
        {//BIT2=0,各项统计数据为0
            //RamOffSet = saveOffset + next;
			RamOffSet = saveOffset + indx2;
			goto nextpoint;
        }
	
        if(ApplVar.PointerType == 0)
        {
            buf[indx++] = 0x01;//grand daily report
            buf[indx++] = 0x0;
        }
        if(ApplVar.PointerType == 1)
        {
            buf[indx++] = 0x20;//clerk daily report
            buf[indx++] = ApplVar.ClerkNumber;
        }
        if(ApplVar.PointerType == 2)
        {
            buf[indx++] = 0x10;//time report
            buf[indx++] = ApplVar.Zone + 1;
        }
        //if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
        #if 0
		if(!CheckNotZero(&ApplVar.Total.Qty))
        {//BIT2=0,各项统计数据为0
            //ApplVar.Total.Qty = ZERO;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            count += 8;
            //return count;
            //RamOffSet = saveOffset + next;
            RamOffSet = saveOffset + next;
			goto nextpoint;
        }
		#endif

        count += 8;
        if (ApplVar.Size.Cc){
            buf[indx++] = ApplVar.Size.Cc;
            memcpy(buf + count, ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
            count += ApplVar.Size.Cc;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Qty){
            buf[indx++] = ApplVar.Size.Qty;
            memcpy(buf + count, ApplVar.Total.Qty.Value, ApplVar.Size.Qty);
            count += ApplVar.Size.Qty;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Amt)
        {
            buf[indx++] = ApplVar.Size.Amt;
            memcpy(buf + count, ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
            count += ApplVar.Size.Amt;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT6))	   /* qty negative ? */
                SETBIT(ApplVar.Total.Amt.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.RetQty)
        {
            buf[indx++] = ApplVar.Size.RetQty;
            memcpy(buf + count, ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
            count += ApplVar.Size.RetQty;
            ApplVar.Total.RetQty.Sign = ApplVar.Total.Qty.Sign & 0x03;	  /* same decimal */
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Disc)
        {
            buf[indx++] = ApplVar.Size.Disc;
            memcpy(buf + count, ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
            count += ApplVar.Size.Disc;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT4))	   /* disc negative ? */
                SETBIT(ApplVar.Total.Disc.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Cost)
        {
            buf[indx++] = ApplVar.Size.Cost;
            memcpy(buf + count, ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
            count += ApplVar.Size.Cost;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT5))	   /* cost	negative ? */
                SETBIT(ApplVar.Total.Cost.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

    }

	nextpoint:
	
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
    {
        ret = ReadTotal();
        indx = count;

		//if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
		if(ret == 0x02 || ret == 0x01)
        {//BIT2=0,各项统计数据为0
        	RamOffSet = saveOffset;
            return count;
        }
		
        if(ApplVar.PointerType == 0)
        {
            buf[indx++] = 0x02;//week report
            buf[indx++] = 0x0;
        }
        if(ApplVar.PointerType == 1)
        {
            buf[indx++] = 0x40;//clerk week report
            buf[indx++] = ApplVar.ClerkNumber;
        }

		#if 0
        if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
        {//BIT2=0,各项统计数据为0
            ApplVar.Total.Qty = ZERO;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            count += 8;
            return count;
        }
		#endif

        count += 8;
        if (ApplVar.Size.Cc){
            buf[indx++] = ApplVar.Size.Cc;
            memcpy(buf + count, ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
            count += ApplVar.Size.Cc;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Qty){
            buf[indx++] = ApplVar.Size.Qty;
            memcpy(buf + count, ApplVar.Total.Qty.Value, ApplVar.Size.Qty);
            count += ApplVar.Size.Qty;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Amt)
        {
            buf[indx++] = ApplVar.Size.Amt;
            memcpy(buf + count, ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
            count += ApplVar.Size.Amt;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT6))     /* qty negative ? */
                SETBIT(ApplVar.Total.Amt.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.RetQty)
        {
            buf[indx++] = ApplVar.Size.RetQty;
            memcpy(buf + count, ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
            count += ApplVar.Size.RetQty;
            ApplVar.Total.RetQty.Sign = ApplVar.Total.Qty.Sign & 0x03;    /* same decimal */
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Disc)
        {
            buf[indx++] = ApplVar.Size.Disc;
            memcpy(buf + count, ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
            count += ApplVar.Size.Disc;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT4))     /* disc negative ? */
                SETBIT(ApplVar.Total.Disc.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Cost)
        {
            buf[indx++] = ApplVar.Size.Cost;
            memcpy(buf + count, ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
            count += ApplVar.Size.Cost;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT5))     /* cost  negative ? */
                SETBIT(ApplVar.Total.Cost.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }
    }

    RamOffSet = saveOffset;

    return count;
}

static int TcpPluReadZport(BYTE *buf)
{
    WORD skip, next, count, indx;
    unsigned long saveOffset;
	BYTE ret;

    skip = 0;
    if (TESTBIT(ApplVar.Size.Periods, BIT0))      /* calculate size of 1 pointer */
        skip += ApplVar.Size.Length;
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
        skip += ApplVar.Size.Length;
	next = skip;
	
    switch (ApplVar.PointerType)
    {
        case 0://统计总销售
            skip = 0;
            break;
        default:
            break;
    }
	next += skip;
    saveOffset = RamOffSet;
    RamOffSet += skip;
	
    count = 0;
    if (TESTBIT(ApplVar.Size.Periods, BIT0)) {
        ret = ReadTotal();
        indx = count;
	
		//if(!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))//jdb2018-05-22 数据为零不上传
		if(ret == 0x02)
        {//BIT2=0,各项统计数据为0
            RamOffSet = saveOffset + next;
			goto nextpoint;
        }
	
        if(ApplVar.PointerType == 0)
        {
            buf[indx++] = 0x80;//plu
            buf[indx++] = 0x0;//
        }
        if(ApplVar.PointerType == 1)
        {
            buf[indx++] = 0x20;//clerk daily report
            buf[indx++] = ApplVar.ClerkNumber;
        }
        if(ApplVar.PointerType == 2)
        {
            buf[indx++] = 0x10;//time report
            buf[indx++] = ApplVar.Zone + 1;
        }
        //if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
        #if 0
		if(!CheckNotZero(&ApplVar.Total.Qty))
        {//BIT2=0,各项统计数据为0
            //ApplVar.Total.Qty = ZERO;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            count += 8;
            //return count;
            //RamOffSet = saveOffset + next;
            RamOffSet = saveOffset + next;
			goto nextpoint;
        }
		#endif

        count += 8;
        if (ApplVar.Size.Cc){
            buf[indx++] = ApplVar.Size.Cc;
            memcpy(buf + count, ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
            count += ApplVar.Size.Cc;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Qty){
            buf[indx++] = ApplVar.Size.Qty;
            memcpy(buf + count, ApplVar.Total.Qty.Value, ApplVar.Size.Qty);
            count += ApplVar.Size.Qty;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Amt)
        {
            buf[indx++] = ApplVar.Size.Amt;
            memcpy(buf + count, ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
            count += ApplVar.Size.Amt;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT6))	   /* qty negative ? */
                SETBIT(ApplVar.Total.Amt.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.RetQty)
        {
            buf[indx++] = ApplVar.Size.RetQty;
            memcpy(buf + count, ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
            count += ApplVar.Size.RetQty;
            ApplVar.Total.RetQty.Sign = ApplVar.Total.Qty.Sign & 0x03;	  /* same decimal */
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Disc)
        {
            buf[indx++] = ApplVar.Size.Disc;
            memcpy(buf + count, ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
            count += ApplVar.Size.Disc;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT4))	   /* disc negative ? */
                SETBIT(ApplVar.Total.Disc.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Cost)
        {
            buf[indx++] = ApplVar.Size.Cost;
            memcpy(buf + count, ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
            count += ApplVar.Size.Cost;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT5))	   /* cost	negative ? */
                SETBIT(ApplVar.Total.Cost.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

    }

	nextpoint:
	
    if (TESTBIT(ApplVar.Size.Periods, BIT1))
    {
        ret = ReadTotal();
        indx = count;

		//if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
		if(ret == 0x02)
        {//BIT2=0,各项统计数据为0
        	RamOffSet = saveOffset;
            return count;
        }
		
        if(ApplVar.PointerType == 0)
        {
            buf[indx++] = 0x02;//week report
            buf[indx++] = 0x80;//plu
        }
        if(ApplVar.PointerType == 1)
        {
            buf[indx++] = 0x40;//clerk week report
            buf[indx++] = ApplVar.ClerkNumber;
        }

		#if 0
        if (!TESTBIT(ApplVar.Total.Qty.Sign, BIT2))    /* already used then clear by not read */
        {//BIT2=0,各项统计数据为0
            ApplVar.Total.Qty = ZERO;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            buf[indx++] = 0xff;
            count += 8;
            return count;
        }
		#endif

        count += 8;
        if (ApplVar.Size.Cc){
            buf[indx++] = ApplVar.Size.Cc;
            memcpy(buf + count, ApplVar.Total.Cc.Value, ApplVar.Size.Cc);
            count += ApplVar.Size.Cc;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Qty){
            buf[indx++] = ApplVar.Size.Qty;
            memcpy(buf + count, ApplVar.Total.Qty.Value, ApplVar.Size.Qty);
            count += ApplVar.Size.Qty;
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Amt)
        {
            buf[indx++] = ApplVar.Size.Amt;
            memcpy(buf + count, ApplVar.Total.Amt.Value, ApplVar.Size.Amt);
            count += ApplVar.Size.Amt;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT6))     /* qty negative ? */
                SETBIT(ApplVar.Total.Amt.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.RetQty)
        {
            buf[indx++] = ApplVar.Size.RetQty;
            memcpy(buf + count, ApplVar.Total.RetQty.Value, ApplVar.Size.RetQty);
            count += ApplVar.Size.RetQty;
            ApplVar.Total.RetQty.Sign = ApplVar.Total.Qty.Sign & 0x03;    /* same decimal */
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Disc)
        {
            buf[indx++] = ApplVar.Size.Disc;
            memcpy(buf + count, ApplVar.Total.Disc.Value, ApplVar.Size.Disc);
            count += ApplVar.Size.Disc;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT4))     /* disc negative ? */
                SETBIT(ApplVar.Total.Disc.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }

        if (ApplVar.Size.Cost)
        {
            buf[indx++] = ApplVar.Size.Cost;
            memcpy(buf + count, ApplVar.Total.Cost.Value, ApplVar.Size.Cost);
            count += ApplVar.Size.Cost;
            if (TESTBIT(ApplVar.Total.Qty.Sign, BIT5))     /* cost  negative ? */
                SETBIT(ApplVar.Total.Cost.Sign, BIT7);
        }else{
            buf[indx++] = 0;
        }
    }

    RamOffSet = saveOffset;

    return count;
}

static signed char TcpProcessRecord(char cmd,unsigned char *hostString,unsigned char psize)
{
    static unsigned long logFirst=0;

    //char sBuffer[sizeof(union FLOWREC)+5];
	char sBuffer[1024]; //jdb2018-05-16

    WORD start, end, sMax, id, save, type, saveClerkNumber;
    short size, count;
    unsigned long sAddr;
    unsigned long nextOff; //jdb2018-05-15 下一个地址
    BYTE currentvalue, zidx; //jdb2018-06-04
    WORD plucount; //jdb2018-06-13
    WORD savelockstate; //jdb2018-07-27 保存当前ECR钥匙档
    short     flowid;//jdb2018-07-18 流水号
    union  FLOWREC	 *FlowBuff;//jdb2018-07-18 流水指针
    char *record;
    unsigned char sPC,sBAR,sBAL,sEcrNo,sLocate,sEXTMem;
    signed char err_state=0;
    char cmdReturn=0;//当其值不为0时,需要向主机进行应答.

    type = *((WORD *)(hostString + 1));   // type: get the DataType..

    id = 0;
    if (type > PLU1 || (type >= UPDPLU && type <= REPPLU))
        save = ApplVar.PluNumber;
    else if (type > DEPT)
        save = ApplVar.DeptNumber;
    else if (type > CLERK)
        save = ApplVar.ClerkNumber;
    else if (type > SALPER)
        save = ApplVar.SalPerNumber;

    switch (cmd)       /* determine record type */
    {
        case cmdGRAPHIC://type包含图片数据的存储地址
            {
                if (ApplVar.CentralLock != SET || psize<3)
                {
                    type = ('C'+('A'<<8));
                    return TcpAnswerToHost(cmdANSWER,type);
                }

                PutsO("Update GRAP");
                DispSt1C(Msg[ZZXZTXING].str);
                TcpSaveGraphic(type,hostString+3,psize-3);//ccr2014-03-25
                type = ('O'+('K'<<8));
                return TcpAnswerToHost(cmdANSWER,type);
            }
        case cmdUPDATE://Update the ECR file
            if (type > MERGEPLU-1 && type < REPPLU+1)
            {
                CheckPluFile(type);
                ApplVar.ErrorNumber=0;
                cmdReturn = cmdANSWER;
                break;
            }
        case cmdDOWNLOAD: //Send ECR file to host
            {
                size = 0;
                record = 0;
                switch (type)
                {
                    case PROMO:
                        size = sizeof(ApplVar.AP.Promotion);
                        record = (char *)&ApplVar.AP.Promotion;
                        break;
                    case SALPER:
                        size = sizeof(ApplVar.AP.SalPer);
                        record = (char *)&ApplVar.AP.SalPer;
                        break;
                    case OPTIONS:
                        size = sizeof(ApplVar.AP.Options);
                        record = (char *)&ApplVar.AP.Options;
                        break;
                    case PROGID :  /* program ID data */
                        size = sizeof(Release);
                        record = (char *)Release;
                        break;
                    case SYSFLG :  /* system Flags */
                        size = sizeof(ApplVar.AP.Flag);
                        record = (char *)ApplVar.AP.Flag;
                        break;
                    case START :  /* start addresses */
                        size = sizeof(ApplVar.AP.StartAddress) ;
                        record = (char *)ApplVar.AP.StartAddress;
                        break;
                    case RCONFIG : /* config */
                        size = sizeof(ApplVar.AP.Config);
                        record = (char *)&ApplVar.AP.Config;
                        break;
                    case RDAY : /* DAY table */
                        size = sizeof(ApplVar.AP.Day);
                        record = (char *)&ApplVar.AP.Day;
                        break;
                    case RMONTH : /* ApplVar.Month table */
                        size = sizeof(ApplVar.AP.Month);
                        record = (char *)&ApplVar.AP.Month;
                        break;
                    case RZONE : /* zone table */
                        size = sizeof(ApplVar.AP.Zone);
                        record = (char *)&ApplVar.AP.Zone;
                        break;
                    case RTOTAL : /* ApplVar.Total sales */
                        size = sizeof(ApplVar.AP.Sales);
                        record = (char *)&ApplVar.AP.Sales;
                        break;
                    case KEY0 :
                        size = 64;	   /* 64 bytes is 32 keys */
                        record = (char *)ApplVar.AP.KeyTable;
                        break;
                    case KEY32 :
                        size = 64;	   /* 64 bytes is 32 keys */
                        record = (char *)&ApplVar.AP.KeyTable[32];
                        break;
                    case RREP1 :		/* user report 1,2 & 3 */
                        size =	3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[0];
                        break;
                    case RREP2 :		/* user report 4,5 & 6 */
                        size =	3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[3];
                        break;
                    case RREP3 :		/* user report 7,8 & 9 */
                        size = 3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[6];
                        break;
                    case RREP4 :		/* user report 7,8 & 9 */
                        size = 3 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[9];
                        break;
                    case RREP5 :		/* user report 7,8 & 9 */
                        size = 1 * sizeof(struct FIXEDREPORT);
                        record = (char *)&ApplVar.AP.ReportList[12];
                        break;

                    case REPCAP1 :		  /* report types 0 - 6 */
                        size = 7 * REPWIDTH;
                        record = (char *)ApplVar.TXT.ReportType;
                        break;
                    case REPCAP2 :		  /* report types 7 - 13 */
                        size = 7 * REPWIDTH;
                        record = (char *)ApplVar.TXT.ReportType[7];
                        break;
                    case REPCAP3 :		  /* messages 14 - 15 */
                        size = 2 * REPWIDTH;
                        record = (char *)ApplVar.TXT.ReportType[14];
                        break;
                    case RHEAD1 :		/* receipt header 0 - 2 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header;
                        break;
                    case RHEAD2 :		/* receipt header 3 - 5 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[3];
                        break;
                    case RHEAD3 :		/* receipt header 6 - 7 */
                        //size = 2 * (PRTLEN+1);
						size = 3 * (PRTLEN+1);/* receipt header 6 - 8 */
                        record = (char *)ApplVar.TXT.Header[6];
                        break;
					//jdb2018-06-11 start 9~40行
					case RHEAD4 :		/* receipt header 9 - 11 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[9];
                        break;
					case RHEAD5 :		/* receipt header 12 - 14 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[12];
                        break;
					case RHEAD6 :		/* receipt header 15 - 17 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[15];
                        break;
					case RHEAD7 :		/* receipt header 18 - 20 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[18];
                        break;
					case RHEAD8 :		/* receipt header 21 - 23 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[21];
                        break;
					case RHEAD9 :		/* receipt header 24 - 26 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[24];
                        break;
					case RHEAD10 :		/* receipt header 27 - 29 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[27];
                        break;
					case RHEAD11 :		/* receipt header 30 - 32 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[30];
                        break;
					case RHEAD12 :		/* receipt header 33 - 35 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[33];
                        break;
					case RHEAD13 :		/* receipt header 36 - 38 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[36];
                        break;
					case RHEAD14 :		/* receipt header 39*/
                        size = 1 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Header[39];
                        break;
					//jdb2018-06-11 end
                    case RTRAIL1 :		 /* receipt trailer 0 - 2 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer;
                        break;
                    case RTRAIL2 :		 /* receipt trailer 3 - 5 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[3];
                        break;
					//jdb2018-06-11 start 7~40行
					case RTRAIL3 :		 /* receipt trailer 6 - 8 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[6];
                        break;
					case RTRAIL4 :		/* receipt header 9 - 11 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[9];
                        break;
					case RTRAIL5 :		/* receipt header 12 - 14 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[12];
                        break;
					case RTRAIL6 :		/* receipt header 15 - 17 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[15];
                        break;
					case RTRAIL7 :		/* receipt header 18 - 20 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[18];
                        break;
					case RTRAIL8 :		/* receipt header 21 - 23 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[21];
                        break;
					case RTRAIL9 :		/* receipt header 24 - 26 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[24];
                        break;
					case RTRAIL10 :		/* receipt header 27 - 29 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[27];
                        break;
					case RTRAIL11 :		/* receipt header 30 - 32 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[30];
                        break;
					case RTRAIL12 :		/* receipt header 33 - 35 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[33];
                        break;
					case RTRAIL13 :		/* receipt header 36 - 38 */
                        size = 3 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[36];
                        break;
					case RTRAIL14 :		/* receipt header 39*/
                        size = 1 * (PRTLEN+1);
                        record = (char *)ApplVar.TXT.Trailer[39];
                        break;
					//jdb2018-06-11 end
                    case RSLIPH1 :		 /* slip header 0 - 2 */
                        size = 3 * SHEADWIDTH;
                        record = (char *)ApplVar.TXT.SlipHeader;
                        break;
                    case RSLIPH2 :		 /* receipt header 3 - 5 */
                        size = 3 * SHEADWIDTH;
                        record = (char *)ApplVar.TXT.SlipHeader[3];
                        break;
                    case FIRM :
                        size = sizeof(ApplVar.AP.FirmKeys);
                        record = (char *)ApplVar.AP.FirmKeys;
                        break;
                    case MNGR :
                        size = sizeof(ApplVar.AP.Manager);
                        record = (char *)ApplVar.AP.Manager;
                        break;
                    case CORREC :
                        size = sizeof(ApplVar.AP.Correc);
                        record = (char *)&ApplVar.AP.Correc;
                        break;
                    case CURR :
                        size = sizeof(ApplVar.AP.Curr);
                        record = (char *)&ApplVar.AP.Curr;
                        break;
                    case DISC :
                        size = sizeof(ApplVar.AP.Disc);
                        record = (char *)&ApplVar.AP.Disc;
                        break;
                    case DRAW :
                        size = sizeof(ApplVar.AP.Draw);
                        record = (char *)&ApplVar.AP.Draw;
                        break;
                    case PBF :
                        size = sizeof(ApplVar.AP.Pb);
                        record = (char *)&ApplVar.AP.Pb;
                        break;
                    case PORA :
                        size = sizeof(ApplVar.AP.PoRa);
                        record = (char *)&ApplVar.AP.PoRa;
                        break;
                    case TEND :
                        size = sizeof(ApplVar.AP.Tend);
                        record = (char *)&ApplVar.AP.Tend;
                        break;
                    case MODI :
                        size = sizeof(ApplVar.AP.Modi);
                        record = (char *)&ApplVar.AP.Modi;
                        break;
                    case TAX :
                        size = sizeof(ApplVar.AP.Tax);
                        record = (char *)&ApplVar.AP.Tax;
                        break;
                    case CLERK :
                        size = sizeof(ApplVar.AP.Clerk);
                        record = (char *)&ApplVar.AP.Clerk;
                        break;
                    case GROUP :
                        size = sizeof(ApplVar.AP.Group);
                        record = (char *)&ApplVar.AP.Group;
                        break;
                    case DEPT :
                        size = sizeof(ApplVar.AP.Dept);
                        record = (char *)&ApplVar.AP.Dept;
                        break;
                    case PLU1 :
                        size = sizeof(ApplVar.AP.Plu);
                        record = (char *)&ApplVar.AP.Plu;
                        break;
                    case OFFER:
                        size = sizeof(ApplVar.AP.OFFPrice);
                        record = (char *)&ApplVar.AP.OFFPrice;
                        break;
                    case AGREE :
                        size = sizeof(ApplVar.AP.Agree);
                        record = (char *)&ApplVar.AP.Agree;
                        break;
                    case BLOCKIC:		//ccr chipcard
                        size = sizeof(ApplVar.AP.ICBlock);
                        record = (char *)&ApplVar.AP.ICBlock;
                        break;
                    case CHIPCARDSET:	//ccr chipcard
                        size = sizeof(ApplVar.ICCardSet);
                        record = (char *)&ApplVar.ICCardSet;
                        break;
                    case REMOTESETTIME :
                        if (TESTBIT(ApplVar.MyFlags,ZREPORT))	/* z report taken ? */
                        {
                            CheckError(ERROR_ID(CWXXI13));
                            type = 0xffff;
                        } else
                        {
                            ApplVar.ErrorNumber=0;
                            memcpy(&EntryBuffer[ENTRYSIZE - 9],&hostString[3],8);
                            Appl_EntryCounter = 8;
                            NewTimeDate(1);
                            memcpy(&EntryBuffer[ENTRYSIZE - 7],&hostString[11],6);
                            Appl_EntryCounter = 6;
                            NewTimeDate(2);
                            if (ApplVar.ErrorNumber)
                            {
                                CheckError(0);
                                type = 0xffff;
                            }
                            ClearEntry(true);
                            InActive |= ACTIVE+1;	  /* Inactive longer then allowed */
                            CheckTime(false);
                        }
                        return TcpAnswerToHost(cmdANSWER,type);
                    default :
                        break;
                }
                if (size)
                {//更新收款机的配置数据
                    if (cmd == cmdUPDATE)   /* update */
                    {
                        sEcrNo = REGISTER;
                        sLocate = LOCATION;
                        sPC = COMPUTER;
                        sBAR = BARCODE;
                        sBAL = BALANCE;
                        sEXTMem = EXTRAMN;
                        if (type == CHIPCARDSET)
                            sAddr = ApplVar.ICCardSet.PosCode;
                        memcpy(record, &hostString[3], size);//将数据存入收款机配置文件中
                        if (type == CHIPCARDSET)
                            ApplVar.ICCardSet.PosCode = sAddr;
                        if (type == SYSFLG)
                        {
                            InitSysFlag();
                            COMPUTER = sPC;
                            BARCODE = sBAR;
                            BALANCE = sBAL;
                            REGISTER = sEcrNo;
                            LOCATION = sLocate;
                            EXTRAMN =  sEXTMem;
                        } else if (type == START)
                        {//更新地址分配,收到的地址分配为相对地址!!!!
                            ClsXRam();
                            if (ResetStartAddress(true)==false)
                            {
                                Bell(2);
                                DispSt1C(Msg[WJKJYJIE].str);
                                RJPrint(0,Msg[WJKJYJIE].str);


                                //重新计算内存中可容纳的单品数目
                                sAddr = ApplVar.AP.StartAddress[AddrEndP] - ApplVar.SIZE_EXTRAM;

                                sAddr /= pluRecordSize;
                                sAddr++;//为需要减少的单品数目
                                sMax = sAddr;
                                sAddr *= pluRecordSize;//所减少的单品所占用的空间
                                if (sMax<ApplVar.AP.Plu.Number)
                                {
                                    ApplVar.AP.Plu.Number -= sMax;
                                    if (ApplVar.AP.Plu.RNumber>ApplVar.AP.Plu.Number)
                                        ApplVar.AP.Plu.RNumber = ApplVar.AP.Plu.Number;
                                } else
                                {
                                    RJPrint(0,(char*)cMessageE18);//"!!!! FAT(PLU) ERROR !!!!"
                                    ApplVar.AP.Plu.Number = ApplVar.AP.Plu.RNumber = 0;
                                }

                                for (sMax = AddrPLU+1;sMax<AddrMAX;sMax++)
                                    ApplVar.AP.StartAddress[sMax] -= sAddr;
                            }
                            ApplVar.MyFlags = 0;
                            //Save_ApplRam();
                        } else if (type == KEY0 || type == KEY32)
                        {
                            DeptPluKeysCount();

                            ApplVar.AP.FirmKeys[ID_xDEPT] = 0xff;
                            ApplVar.AP.FirmKeys[ID_DATE] = 0xff;
                            ApplVar.AP.FirmKeys[ID_RJFEED] = 0xff;
                            ApplVar.AP.FirmKeys[ID_CLEAR] = 0xff;
                            ApplVar.AP.FirmKeys[ID_LOCK] = 0xff;

                            for (id=0;id<MAXKEYB;id++)
                            {
                                switch (ApplVar.AP.KeyTable[id])
                                {
                                    case SHIFT1:
                                        ApplVar.AP.FirmKeys[ID_xDEPT] = id;
                                        break;
                                    case DATETIME:
                                        ApplVar.AP.FirmKeys[ID_DATE] = id;
                                        break;
                                    case JPF:
                                    case RPF:
                                        ApplVar.AP.FirmKeys[ID_RJFEED] = id;
                                        break;
                                    case CLEAR:
                                        ApplVar.AP.FirmKeys[ID_CLEAR] = id;
                                        break;
                                    case MODELOCK:
                                        ApplVar.AP.FirmKeys[ID_LOCK] = id;
                                        break;
                                }
                            }
                        }
                        return TcpAnswerToHost(cmdANSWER,type);//应答host,处理完毕
                    }
                    //为'D'命令,把配置数据发送给主机
                    if (type == START)//转换为相对地址发送给主机
                        ResetStartAddress(false);
                    err_state = TcpRecordToHost(cmdUPDATE,type,record,size);/* send record */
                    if (type == START)//恢复为绝对地址
                    {
                        ResetStartAddress(true);
                        Init_Flow();
                    }
                    Save_Config(true);//ccr2014-10-30
                }
                else
                {
                    if (type == ADDPLU || type == DELPLU || type == UPDPLU)
                    {
                        if (cmd != cmdUPDATE)
                        {
                            cmdReturn = cmdANSWER;
                            break;
                        }
                        if (ApplVar.AP.Plu.RandomSize && ApplVar.AP.Plu.Number)
                        {
                            ApplVar.Key.Code = 0;	/* suppress repeat */
                            memcpy(&ApplVar.Plu, &hostString[3], sizeof(ApplVar.Plu));
                            ApplVar.PluNumber = GetPluNumber(1, ApplVar.Plu.Random);		/* fill ApplVar.PluNumber with number */
                            switch (type)
                            {
                                case DELPLU:
                                    if (ApplVar.PluNumber)
                                    {
                                        CheckRandomPlu(1, 1);		/* delete plu */
                                        SETBIT(ApplVar.MyFlags,CONFIGECR);
                                    }
                                    break;
                                case ADDPLU:
                                    if (!ApplVar.PluNumber)  /* Not Found then move and Add */
                                    {
                                        CheckRandomPlu(0, 1);//ccr2014-12-10
                                        //ccr2014-12-10if (CheckRandomPlu(0, 1))
                                        //ccr2014-12-10 ApplVar.PluNumber = GetPluNumber(0, ApplVar.Plu.Random);		/* fill ApplVar.PluNumber with number */
                                        SETBIT(ApplVar.MyFlags,CONFIGECR);
                                        break;//ccr2014-12-10
                                    }
                                case UPDPLU:
                                    if (ApplVar.PluNumber)
                                    {
                                        ApplVar.PluNumber--;
                                        WritePlu();
                                        SETBIT(ApplVar.MyFlags,CONFIGECR);
                                    }
                                default:
                                    break;
                            }
                        }
                        err_state = TcpAnswerToHost(cmdANSWER,type);//应答主机,下载数据处理完毕,请求下载下一条
                        break;
                    }
                    else if (type > PBTOT)
                    {
                        if (cmd == cmdUPDATE)	/* update */
                        {
                            PbTotal(type - PBTOT, 0);
                            if (!(PBINFO & 0x04) && !(PBPRINT & 0x80))		 /* open numbers ? */
                                memcpy(ApplVar.PB.Random, &hostString[3], 7);
                            memcpy(ApplVar.PB.Text, &hostString[10], sizeof(ApplVar.PB.Text));
                            PbTotal(type - PBTOT, 3);
                            err_state = TcpAnswerToHost(cmdDOWNLOAD,type+1);//应答主机,下载数据处理完毕,请求下载下一条
                            break;
                        }
                        id = PBTOT + 1;
                        if ((ApplVar.AP.Pb.Random & 0x0f) || ApplVar.AP.Pb.Text)
                            sMax = ApplVar.AP.Pb.NumberOfPb;
                        else
                            sMax = 0;				 /* Not active */
                        size = sizeof(ApplVar.PB.Random) + sizeof(ApplVar.PB.Text);
                        record = (char *)ApplVar.PB.Random;
                    }
                    else if (type > BLOCKIC)	/* Chipcard for block ? ccr chipcard */
                    {
                        if (cmd == cmdUPDATE)	/* update */
                        {
                            ApplVar.ICBlockNumber = type - BLOCKIC - 1;
                            if (ApplVar.ICBlockNumber < ApplVar.AP.ICBlock.Number)
                            {
                                memcpy(&ApplVar.ICBlock, &hostString[3], sizeof(ApplVar.ICBlock));
                                WriteICBlock();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = BLOCKIC + 1;
                        sMax = ApplVar.AP.ICBlock.Number;
                        size = sizeof(ApplVar.ICBlock);
                        record = (char *)&ApplVar.ICBlock;

                    } 
                    else if (type > PLU1)	/* plu */
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.PluNumber = type - PLU1 - 1;
                            if (ApplVar.PluNumber < ApplVar.AP.Plu.Number)
                            {
                                memcpy(&ApplVar.Plu, &hostString[3], sizeof(ApplVar.Plu));
                                WritePlu();
                                if (ApplVar.PluNumber==0)//ccr2015-02-26
                                    ApplVar.AP.Plu.RNumber=0;//ccr2015-02-26
                                if (ApplVar.AP.Plu.RandomSize &&    /* set PLU's used */
                                        (ApplVar.PluNumber >= ApplVar.AP.Plu.RNumber))
                                {
                                    ApplVar.AP.Plu.RNumber = (ApplVar.PluNumber + 1);
                                }
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = PLU1 + 1;
                        if (ApplVar.AP.Plu.RandomSize)	 /* set PLU's used */
                            sMax = ApplVar.AP.Plu.RNumber;
                        else
                            sMax = ApplVar.AP.Plu.Number;
                        size = sizeof(ApplVar.Plu);
                        record = (char *)&ApplVar.Plu;
                    }
                    else if (type > DEPT)	/* dept */
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.DeptNumber = type - DEPT - 1;
                            if (ApplVar.DeptNumber<ApplVar.AP.Dept.Number)
                            {
                                memcpy(&ApplVar.Dept, &hostString[3], sizeof(ApplVar.Dept));
                                WriteDept();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = DEPT + 1;
                        sMax = ApplVar.AP.Dept.Number;
                        size = sizeof(ApplVar.Dept);
                        record = (char *)&ApplVar.Dept;
                    }
                    else if (type > GROUP)   /* group */
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.GroupNumber = type - GROUP - 1;
                            if (ApplVar.GroupNumber<ApplVar.AP.Group.Number)
                            {
                                memcpy(&ApplVar.Group, &hostString[3], sizeof(ApplVar.Group));
                                WriteGroup();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = GROUP + 1;
                        sMax = ApplVar.AP.Group.Number;
                        size = sizeof(ApplVar.Group);
                        record = (char *)&ApplVar.Group;
                    } 
                    else if (type > OFFER)   /* OFFER */
                    {
                        if (cmd == cmdUPDATE)   /* update */
                        {
                            ApplVar.OFFNumber = type - OFFER -1;
                            if (ApplVar.OFFNumber<ApplVar.AP.OFFPrice.Number)
                            {
                                memcpy(&ApplVar.OFFPrice, &hostString[3], sizeof(ApplVar.OFFPrice));
                                WriteOFFPrice();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = OFFER+1;
                        sMax = ApplVar.AP.OFFPrice.Number;
                        size = sizeof(ApplVar.OFFPrice);
                        record = (char *)&ApplVar.OFFPrice;
                    }
                    else if (type > CLERK)	 /* clerk */
                    {
                        if (cmd == cmdUPDATE)	/* update */
                        {
                            if (ApplVar.ClerkNumber<=ApplVar.AP.Clerk.Number)
                            {
                                ApplVar.ClerkNumber = type - CLERK;
                                memcpy(&ApplVar.Clerk, &hostString[3], sizeof(ApplVar.Clerk));
                                WriteClerk();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = CLERK;
                        sMax = ApplVar.AP.Clerk.Number + 1;
                        size = sizeof(ApplVar.Clerk);
                        record = (char *)&ApplVar.Clerk;
                    } 
                    else if (type > TAX)	  /* modifier */
                    {
                        if (cmd == cmdUPDATE)	/* update */
                        {
                            ApplVar.TaxNumber = type - TAX - 1;
                            if (ApplVar.TaxNumber<ApplVar.AP.Tax.Number)
                            {
                                memcpy(&ApplVar.Tax, &hostString[3], sizeof(ApplVar.Tax));
                                WriteTax();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = TAX + 1;
                        sMax = ApplVar.AP.Tax.Number;
                        size = sizeof(ApplVar.Tax);
                        record = (char *)&ApplVar.Tax;
                    } 
                    else if (type > SALPER)	 /* sales person */
                    {
                        if (cmd == cmdUPDATE)	/* update */
                        {
                            ApplVar.SalPerNumber = type - SALPER;
                            if (ApplVar.SalPerNumber<=ApplVar.AP.SalPer.Number)
                            {
                                memcpy(&ApplVar.SalPer, &hostString[3], sizeof(ApplVar.SalPer));
                                WriteSalPer();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = SALPER;
                        sMax = ApplVar.AP.SalPer.Number+1;
                        size = sizeof(ApplVar.SalPer);
                        record = (char *)&ApplVar.SalPer;
                    } 
                    else if (type > MODI)    /* modifier */
                    {
                        if (cmd == cmdUPDATE)	/* update */
                        {
                            ApplVar.ModiNumber = type - MODI - 1;
                            if (ApplVar.ModiNumber<ApplVar.AP.Modi.Number)
                            {
                                memcpy(&ApplVar.Modi, &hostString[3], sizeof(ApplVar.Modi));
                                WriteModi();
                                cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                            } else
                                cmdReturn = cmdANSWER;
                            break;
                        }
                        id = MODI + 1;
                        sMax = ApplVar.AP.Modi.Number;
                        size = sizeof(ApplVar.Modi);
                        record = (char *)&ApplVar.Modi;
                    }
                    else
                    {
                        id = (type / 100) * 100 + 1;
                        if (type > TEND)
                        {
                            if (cmd == cmdUPDATE)	/* update */
                            {
                                ApplVar.TendNumber = type - TEND - 1;
                                if (ApplVar.TendNumber<ApplVar.AP.Tend.Number)
                                {
                                    memcpy(&ApplVar.Tend, &hostString[3], sizeof(ApplVar.Tend));
                                    WriteTender();
                                    cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                                } else
                                    cmdReturn = cmdANSWER;
                                break;
                            }
                            sMax = ApplVar.AP.Tend.Number;
                            size = sizeof(ApplVar.Tend);
                            record = (char *)&ApplVar.Tend;
                        } else if (type > PORA)
                        {
                            if (cmd == cmdUPDATE)	/* update */
                            {
                                ApplVar.PoRaNumber = type - PORA - 1;
                                if (ApplVar.PoRaNumber<ApplVar.AP.PoRa.Number)
                                {
                                    memcpy(&ApplVar.PoRa, &hostString[3], sizeof(ApplVar.PoRa));
                                    WritePoRa();
                                    cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                                } else
                                    cmdReturn = cmdANSWER;
                                break;
                            }
                            sMax = ApplVar.AP.PoRa.Number;
                            size = sizeof(ApplVar.PoRa);
                            record = (char *)&ApplVar.PoRa;
                        } else if (type > PBF)
                        {
                            if (cmd == cmdUPDATE)	/* update */
                            {
                                ApplVar.PbFNumber = type - PBF - 1;
                                if (ApplVar.PbFNumber<ApplVar.AP.Pb.Number)
                                {
                                    memcpy(&ApplVar.PbF, &hostString[3], sizeof(ApplVar.PbF));
                                    WritePbF();
                                    cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                                } else
                                    cmdReturn = cmdANSWER;
                                break;
                            }
                            sMax = ApplVar.AP.Pb.Number;
                            size = sizeof(ApplVar.PbF);
                            record = (char *)&ApplVar.PbF;
                        } else if (type > DRAW)
                        {
                            if (cmd == cmdUPDATE)	/* update */
                            {
                                ApplVar.DrawNumber = type - DRAW - 1;
                                if (ApplVar.DrawNumber<ApplVar.AP.Draw.Number)
                                {
                                    memcpy(&ApplVar.Draw, &hostString[3], sizeof(ApplVar.Draw));
                                    WriteDrawer();
                                    cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                                } else
                                    cmdReturn = cmdANSWER;
                                break;
                            }
                            sMax = ApplVar.AP.Draw.Number;
                            size = sizeof(ApplVar.Draw);
                            record = (char *)&ApplVar.Draw;
                        } else if (type > DISC)
                        {
                            if (cmd == cmdUPDATE)	/* update */
                            {
                                ApplVar.DiscNumber = type - DISC - 1;
                                if (ApplVar.DiscNumber<ApplVar.AP.Disc.Number)
                                {
                                    memcpy(&ApplVar.Disc, &hostString[3], sizeof(ApplVar.Disc));
                                    WriteDisc();
                                    cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                                } else
                                    cmdReturn = cmdANSWER;
                                break;
                            }
                            sMax = ApplVar.AP.Disc.Number;
                            size = sizeof(ApplVar.Disc);
                            record = (char *)&ApplVar.Disc;
                        } else if (type > CURR)
                        {
                            if (cmd == cmdUPDATE)	/* update */
                            {
                                ApplVar.CurrNumber = type - CURR - 1;
                                if (ApplVar.CurrNumber<ApplVar.AP.Curr.Number)
                                {
                                    memcpy(&ApplVar.Curr, &hostString[3], sizeof(ApplVar.Curr));
                                    WriteCurr();
                                    cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                                } else
                                    cmdReturn = cmdANSWER;
                                break;
                            }
                            sMax = ApplVar.AP.Curr.Number;
                            size = sizeof(ApplVar.Curr);
                            record = (char *)&ApplVar.Curr;
                        } else if (type > CORREC)
                        {
                            if (cmd == cmdUPDATE)	/* update */
                            {
                                ApplVar.CorrecNumber = type - CORREC - 1;
                                if (ApplVar.CorrecNumber<ApplVar.AP.Correc.Number)
                                {
                                    memcpy(&ApplVar.Correc, &hostString[3], sizeof(ApplVar.Correc));
                                    WriteCorrec();
                                    cmdReturn = cmdDOWNLOAD; //应答主机,下载数据处理完毕,请求下载下一条
                                } else
                                    cmdReturn = cmdANSWER;
                                break;
                            }
                            sMax = ApplVar.AP.Correc.Number;
                            size = sizeof(ApplVar.Correc);
                            record = (char *)&ApplVar.Correc;
                        } else
                        {
                            cmdReturn = cmdANSWER;
                            break;
                        }
                    }
                    start = type - id;
                    if (start < sMax)
                    {//将指定文件的一条记录数据发送给主机
                        MemSet(record, size, 0);    /* clear memory */
                        memset(sBuffer,' ',INPUTWIDTH);
                        if (type > PBTOT)
                        {
                            CopyFrStr(sBuffer,Msg[SETPBINF].str);
                            PbTotal(start + 1, 0);  /* read */
                        } else if (type > BLOCKIC)//ccr chipcard
                        {
                            CopyFrStr(sBuffer,Msg[GUASHIIC].str);
                            ApplVar.ICBlockNumber = start;
                            ReadICBlock();
                        } else if (type > PLU1)
                        {
                            CopyFrStr(sBuffer,Msg[SETPLU].str);
                            ApplVar.PluNumber= start;
                            ReadPlu();
                        } else if (type > DEPT)
                        {
                            CopyFrStr(sBuffer,Msg[SETDEPT].str);
                            ApplVar.DeptNumber = start;
                            ReadDept();
                        } else if (type > GROUP)
                        {
                            CopyFrStr(sBuffer,Msg[SETGROUP].str);
                            ApplVar.GroupNumber = start;
                            ReadGroup();
                        } else if (type > OFFER)
                        {
                            CopyFrStr(sBuffer,Msg[SETOFF].str);
                            ApplVar.OFFNumber = start;
                            ReadOFFPrice();
                        } else if (type > CLERK)
                        {
                            CopyFrStr(sBuffer,Msg[SETCLERK].str);
                            ApplVar.ClerkNumber = start;
                            ReadClerk();
                        } else if (type > TAX)    /* tax */
                        {
                            CopyFrStr(sBuffer,Msg[SETTAX].str);
                            ApplVar.TaxNumber = start;
                            ReadTax();
                        } else if (type > SALPER)    /* salesperson */
                        {
                            CopyFrStr(sBuffer,Msg[SETSALER].str);
                            ApplVar.SalPerNumber = start;
                            ReadSalPer();
                        } else if (type > MODI)    /* modifier */
                        {
                            CopyFrStr(sBuffer,Msg[SETMODIF].str);
                            ApplVar.ModiNumber = start;
                            ReadModi();
                        } else if (type > TEND)    /* tender */
                        {
                            CopyFrStr(sBuffer,Msg[SETTEND].str);
                            ApplVar.TendNumber = start;
                            ReadTender();
                        } else if (type > PORA)    /* pora */
                        {
                            CopyFrStr(sBuffer,Msg[SETPORA].str);
                            ApplVar.PoRaNumber = start;
                            ReadPoRa();
                        } else if (type > PBF)    /* pb */
                        {
                            CopyFrStr(sBuffer,Msg[SETPBF].str);
                            ApplVar.PbFNumber = start;
                            ReadPbF();
                        } else if (type > DRAW)    /* drawer */
                        {
                            CopyFrStr(sBuffer,Msg[SETDRAWER].str);
                            ApplVar.DrawNumber = start;
                            ReadDrawer();
                        } else if (type > DISC)    /* discount */
                        {
                            CopyFrStr(sBuffer,Msg[SETDISC].str);
                            ApplVar.DiscNumber = start;
                            ReadDisc();
                        } else if (type > CURR)    /* currency */
                        {
                            CopyFrStr(sBuffer,Msg[SETCURR].str);
                            ApplVar.CurrNumber = start;
                            ReadCurr();
                        } else                  /* correction */
                        {
                            CopyFrStr(sBuffer,Msg[SETCORR].str);
                            ApplVar.CorrecNumber = start;
                            ReadCorrec();
                        }
                        WORDtoASC(sBuffer+INPUTWIDTH-1,start);
                        DispSt2(sBuffer);

                        err_state = TcpRecordToHost(cmdUPDATE,type,record, size);//发送记录数据
                    }
                    else
                        cmdReturn = cmdANSWER;//无所需要的数据
                }
            }
            break;
        case cmdINIT:			/* init */
            if (type == 0x544e) /* command "INT" */
            {
                InitApplication(true);//强制进行初始化
            }
            cmdReturn = cmdANSWER;//应答主机,数据处理完毕
            ARROWS = 0x01;
            break;
        case cmdCLEAR:			 /* clear memory ? */
            if (type == 0x524c) /* command "C  LR" */
            {
                id = ApplVar.CentralLock;
                start = ApplVar.ClerkNumber;
                end = ApplVar.ClerkLock;
                sMax = ApplVar.SalPerNumber;
                //				ClearApplMemory();
                ClearAllReport();
                ApplVar.CentralLock = id;
                ApplVar.ClerkNumber = start;
                ApplVar.ClerkLock = end;
                ApplVar.SalPerNumber = sMax;
            }
            cmdReturn = cmdANSWER;
            ARROWS = 0x01;
            break;
        case cmdFLOWDATA:
            if(ApplVar.FRegi || ApplVar.FlowHeader.ReadP==ApplVar.FlowHeader.NextNP)
            {//在销售状态下,禁止提取流水数据
                cmdReturn = cmdANSWER;
            }
            else
            {//发送收款机销售流水,数据发送格式:'F'+[size]+[type1]+[type2]+[data]; [type1]+[type2]为流水顺序号
                if (logFirst!=0)
                    ApplVar.FlowHeader.ReadP = logFirst;

                if (type==0)
                {
                    logFirst = ApplVar.FlowHeader.ReadP;
                    sBuffer[0] = ECRNOLOG;
                    sBuffer[1] = REGISTER;
                    sBuffer[2] = LOCATION;
                    return TcpRecordToHost(cmdFLOWDATA,type,sBuffer, 3);//发送记录数据
                }
                sAddr = ApplVar.FlowHeader.ReadP;
                size = Read_Flow(sBuffer);
                logFirst = ApplVar.FlowHeader.ReadP;
                if (size <= 0)
                {
                    if (size==-1)
                    {
                        ApplVar.ErrorNumber = (ERROR_ID(CWXXI48));
                    }

                    cmdReturn = cmdANSWER;
                } else
                {
                    ApplVar.FlowHeader.ReadP = sAddr;//必须在收到host的应答后,确认host收到了数据,才修正FlowHeader.ReadP

					//jdb2018-07-18 流水加入信息
					flowid = sBuffer[0] & 0x1f;
					FlowBuff = (union  FLOWREC	 *)sBuffer;
					if(flowid == DEPTLOG)
					{
						ApplVar.DeptNumber = FlowBuff->CDC_dpt.DeptN-1;
						ReadDept();
						memcpy(sBuffer + size, ApplVar.Dept.Name, sizeof(ApplVar.Dept.Name));
						size += sizeof(ApplVar.Dept.Name);
					}
					else if(flowid == PLULOG)
					{
						ApplVar.PluNumber = GetPluNumber(1, FlowBuff->CDC_art.ArtN);
        				if (ApplVar.PluNumber)
            				ApplVar.PluNumber--;
						ReadPlu();
						memcpy(sBuffer + size, ApplVar.Plu.Name, sizeof(ApplVar.Plu.Name));
						size += sizeof(ApplVar.Plu.Name);
					}
					else if(flowid == REGISLOG)
					{
						ApplVar.ClerkNumber = FlowBuff->CDC_regis.OperNo;
						ReadClerk();
						memcpy(sBuffer + size, ApplVar.Clerk.Name, sizeof(ApplVar.Clerk.Name));
						size += sizeof(ApplVar.Clerk.Name);
					}
					else if(flowid == TAXLOG)
					{
						ApplVar.TaxNumber = FlowBuff->CDC_Tax.TaxID-1;
    					ReadTax();
						memcpy(sBuffer + size, ApplVar.Tax.Name, sizeof(ApplVar.Tax.Name));
						size += sizeof(ApplVar.Tax.Name);
					}
					
                    err_state = TcpRecordToHost(cmdFLOWDATA,type,sBuffer, size);//发送记录数据
                    if (err_state!=ERR_OK)
                    {//发送失败,恢复流水指针
                        ApplVar.ErrorNumber = (ERROR_ID(CWXXI84));
                        cmdReturn = cmdANSWER;
                    } else
                        return err_state;
                }
            }
            break;
        case cmdPASSWORD://ccr2015-02-04 Catesir for PWD>>>>>>>>>>>>>>>>
            switch (type)
            {
                case 0:
                    memcpy(&ApplVar.AP.ModePwd.PwdX,&hostString[3],MAXPWD);//MAXPWD-1
                    break;
                case 1:
                    memcpy(&ApplVar.AP.ModePwd.PwdZ,&hostString[3],MAXPWD);
                    break;
                case 2:
                    memcpy(&ApplVar.AP.ModePwd.PwdSET,&hostString[3],MAXPWD);
                    break;
                case 3:
                    memcpy(&ApplVar.AP.ModePwd.PwdMG,&hostString[3],MAXPWD);
                    break;
            }
            cmdReturn = cmdANSWER;
            break;
        case cmdSERVER://ccr2015-02-04 Catesir for Set IP>>>>>>>>>>>>>>>>
            ApplVar.IP.ServerIP[0]= hostString[1];
            ApplVar.IP.ServerIP[1]= hostString[2];
            ApplVar.IP.ServerIP[2]= hostString[3];
            ApplVar.IP.ServerIP[3]= hostString[4];
            cmdReturn = cmdANSWER;
            break;
		case 'P': //jdb2018-05-29 
			size = 0;
            memset(sBuffer, 0, sizeof(sBuffer));
			if(type == 100) //获取shop id , ecr id
			{
				sBuffer[0] = ApplVar.AP.Flag[0x0e];
				sBuffer[1] = ApplVar.AP.Flag[0x0f];
				size = 2;
			}
			else if(type == DEPT)
			{
				*((WORD *)sBuffer) = ApplVar.AP.Dept.Number;
				size = 2;
			}
			else if(type == PLU1)//获取有效PLU总数
			{
			if (ApplVar.AP.Plu.RandomSize)	 /* set PLU's used */
            	sMax = ApplVar.AP.Plu.RNumber;
            else
                sMax = ApplVar.AP.Plu.Number;
			*((WORD *)sBuffer) = sMax;
			size = 2;
			}
			TcpRecordToHost('P',type,sBuffer, size);
			break;
		case 'R': //jdb2018-06-04
			size = 0;
      		memset(sBuffer, 0, sizeof(sBuffer));
			if(*(hostString + 1) <= 0x09) //jdb2018-06-04 Z-print
			{
				zidx = *(hostString + 1);

				sBuffer[0] = 0x4f;
				sBuffer[1] = 0x4b;
				size = 2;

				TcpRecordToHost('R',type,sBuffer, size); //jdb2018-06-20 打印前发送命令
				
				currentvalue = ApplVar.FReport;
				ApplVar.FReport = Z;
				savelockstate = ApplVar.CentralLock;//jdb2018-07-27 保存钥匙档
				ApplVar.CentralLock = ZZ;//jdb2018-07-27 打印票据区间
				ApplVar.ReportNumber = zidx;//jdb2018-07-27
				GetReport(zidx);
				ApplVar.CentralLock = savelockstate;
				ApplVar.FReport = currentvalue;
			}
			else
				TcpRecordToHost('R',type,sBuffer, size);
			break;
            //jdb2018-05-08 获取Z报表
        case 'Z':           /* command for getting system report */
            {
#if 0
                ApplVar.RepComputer = 0;
                ApplVar.FReport = 0;
                MemSet(SysBuf + 7, 24, ' ');
                *((WORD *)(SysBuf)) = 1;        /* indicate counters */
                SysBuf[2] = 1;      /* indicate text */
                SysBuf[3] = 0;
                SysBuf[4] = 0;
                *((WORD *)(SysBuf + 5)) = 0;
                GetReceiptNumber(SysBuf + 7);
                SysBuf[15] = 'L';      /* location number */
                SysBuf[16] = LOCATION;
                SysBuf[19] = 'R';      /* register number */
                SysBuf[20] = REGISTER;
                SysBuf[24] = 'Z';
                HEXtoASC(SysBuf + 25,(char *)&ApplVar.ZCount, 3);
                TcpRecordToHost('Z',type,SysBuf, 31);
                //SendRecord(SysBuf, 31);      /* send counter record */
                SysBuf[0] = 2;              /* indicate time & date */
                memcpy(SysBuf + 7, DateAsci, 15);
                SysBuf[22] = ' ';
                memcpy(SysBuf + 23, TimeAsci, 8);
                TcpRecordToHost('Z',type,SysBuf, 31);
                //SendRecord(SysBuf, 31);      /* send time-date record */

                SysBuf[0] = PCBuffer[0];// 'S' or 'R'
                SysBuf[1] = PCBuffer[1];// 'Z' or 'X' or 'C'

                if (ApplVar.FRegi)
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
                else if (!ApplVar.ErrorNumber)
                {
                    ApplVar.RepComputer = PCBuffer[1];        /* X or Z (only Z is checked) */
                    if (ApplVar.RepComputer == 'Z' || ApplVar.RepComputer == 'C')
                        ApplVar.FReport = Z;
                    else
                        ApplVar.FReport = X;
                    if (PCBuffer[0] == 'R')       /* user report ? */
                    {//PCBuffer[2]=ReportNumber
                        ApplVar.ReportNumber = PCBuffer[2];
                        GetReport(ApplVar.ReportNumber);
                    }
                    else
                    {
                        Appl_EntryCounter = 4;
                        memcpy(EntryBuffer + ENTRYSIZE - 5,PCBuffer + 2, 4);
                        GetSystemReport(0);
                        ClearEntry(false);
                    }
                }

                ApplVar.RepComputer = 0;
                ApplVar.FReport = 0;
                *((WORD *)(PCBuffer)) = 0;        /* indicate end */
                PCBuffer[2] = 1;      /* indicate text */
                PCBuffer[3] = 0;
                PCBuffer[4] = 0;
                *((WORD *)(PCBuffer + 5)) = 0;
                if (ApplVar.ErrorNumber)        /* error occurred */
                {
                    *((WORD *)(PCBuffer + 5)) = ApplVar.ErrorNumber;
                    strcpy(PCBuffer + 7, Msg[ApplVar.ErrorNumber - 1].str);
                    TcpRecordToHost('Z',type,PCBuffer, 7 + strlen(Msg[ApplVar.ErrorNumber - 1].str));
                    //SendRecord(PCBuffer, 7 + strlen(Msg[ApplVar.ErrorNumber - 1].str));
                    ApplVar.ErrorNumber=0;
                }
                else
                    TcpRecordToHost('Z',type,PCBuffer, 7);
                //SendRecord(PCBuffer, 7);
#else
                ApplVar.BufCG = 0;

                size = 0;
				count = 0;
                memset(sBuffer, 0, sizeof(sBuffer));

				if(*(hostString + 2) == 0x80) //jdb2018-05-22 PLU
				{
					plucount = 0;
					for(ApplVar.PluNumber = ApplVar.SPluNumber; plucount < 5 && ApplVar.PluNumber < ApplVar.AP.Plu.RNumber; ApplVar.PluNumber++)
					{
						ApplVar.PointerType = 0;

						GetPluOffSet();
                        RamOffSet += ApplVar.AP.Plu.TotalOffSet;

						ApplVar.Size = ApplVar.AP.Plu.Size[ApplVar.PointerType];
                        count = TcpPluReadZport(sBuffer + size);
						if(count > 0)
						{
							size += count;

							GetPluOffSet();
                        	ReadRam(ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
                        	ReadRam(ApplVar.Plu.Name, ApplVar.AP.Plu.CapSize);

                        	sBuffer[size++] = ApplVar.AP.Plu.CapSize;

                        	memcpy(sBuffer + size, ApplVar.Plu.Name, ApplVar.AP.Plu.CapSize);
                        	size += ApplVar.AP.Plu.CapSize;

							//jdb2018-07-01 报表增加barcode项
							sBuffer[size++] = ApplVar.AP.Plu.RandomSize;
							memcpy(sBuffer + size, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
                        	size += ApplVar.AP.Plu.RandomSize;

							plucount++;
						}
					}

					if(plucount > 0) //jdb2018-06-20 有PLU数据
					{
						ApplVar.SPluNumber = ApplVar.PluNumber; //保存PLU序号
					}
					else //报表数据读取完
					{
						ApplVar.SPluNumber = 0;
						
						sBuffer[size++] = 'O';
						sBuffer[size++] = 'K';
					}
				}
                else if(*(hostString + 2) == 0x01) //jdb2018-05-11 GRANDTOTAL
                {
                    sBuffer[size++] = 0;//没有名字
                    for(ApplVar.PointerType = 0; ApplVar.PointerType < 3; ApplVar.PointerType++)
                    {
                        if(ApplVar.PointerType == 0)
                        {
                            RamOffSet = ApplVar.AP.StartAddress[AddrTotl];
                            nextOff = RamOffSet;
                        }
                        else
                        {
                            RamOffSet = nextOff;
                        }

                        ApplVar.Size = ApplVar.AP.Sales.Size[ApplVar.PointerType];

                        if(ApplVar.PointerType == 2)//time zone
                        {

                            for(ApplVar.Zone = 0; ApplVar.Zone < ApplVar.AP.Zone.Number; ApplVar.Zone++)
                            {
                                size += TcpReadZport(sBuffer + size);
                            }
                        }
						#if 1
						else if(ApplVar.PointerType == 1)//clerk
						{
							saveClerkNumber = ApplVar.ClerkNumber;
							for(ApplVar.ClerkNumber = 1; ApplVar.ClerkNumber <= ApplVar.AP.Clerk.Number; ApplVar.ClerkNumber++)
                            {
                                size += TcpReadZport(sBuffer + size);
                            }
							ApplVar.ClerkNumber = saveClerkNumber;
						}
						#endif
                        else
                        {
                            size += TcpReadZport(sBuffer + size);
                        }

                        //jdb2018-05-15 跳向下一个地址
                        if(ApplVar.PointerType == 0)
                        {
                            nextOff += PeriodSkip(&(ApplVar.Size), 1);
                        }

                        if(ApplVar.PointerType == 1)
                        {
                            nextOff += PeriodSkip(&(ApplVar.Size), ApplVar.AP.Clerk.Number);
                        }
                    }
                }
                else if(*(hostString + 2) == 0x02) //jdb2018-05-11 DEPART
                {
                    ApplVar.DeptNumber = *((char *)(hostString + 3)) - 1;
                    if(ApplVar.DeptNumber < ApplVar.AP.Dept.Number){
                        GetDeptOffSet();
                        //jdb2018-05-14 读取部类名字
                        ReadRam(ApplVar.Dept.Random, ApplVar.AP.Dept.RandomSize);
                        ReadRam(ApplVar.Dept.Name, ApplVar.AP.Dept.CapSize);

                        sBuffer[size++] = ApplVar.AP.Dept.CapSize;

                        memcpy(sBuffer + size, ApplVar.Dept.Name, ApplVar.AP.Dept.CapSize);
                        size += ApplVar.AP.Dept.CapSize;
                        for(ApplVar.PointerType = 0; ApplVar.PointerType < POINTERMAX; ApplVar.PointerType++)
                        {
                            if(ApplVar.PointerType == 0){

                                GetDeptOffSet();
                                RamOffSet += ApplVar.AP.Dept.TotalOffSet;
                                nextOff = RamOffSet;
                            }
                            else
                            {
                                RamOffSet = nextOff;
                            }

                            ApplVar.Size = ApplVar.AP.Dept.Size[ApplVar.PointerType];
                            size += TcpReadZport(sBuffer + size);

                            //jdb2018-05-15 跳向下一个地址
                            if(ApplVar.PointerType == 0)
                            {
                                nextOff += PeriodSkip(&(ApplVar.Size), 1);
                            }
                        }
                    }
                }
                else if(*(hostString + 2) == 0x04) //jdb2018-05-11 TENDER
                {
                    ApplVar.TendNumber = *((char *)(hostString + 3)) - 1;
                    if(ApplVar.TendNumber < ApplVar.AP.Tend.Number){

                        GetTenderOffSet();
                        ReadRam((BYTE*)&ApplVar.Tend, ApplVar.AP.Tend.TotalOffSet);	/* write function options */
                        ApplVar.Tend.Name[ApplVar.AP.Tend.CapSize] = 0 ;

                        sBuffer[size++] = ApplVar.AP.Tend.CapSize;

                        memcpy(sBuffer + size, ApplVar.Tend.Name, ApplVar.AP.Tend.CapSize);
                        size += ApplVar.AP.Tend.CapSize;

                        for(ApplVar.PointerType = 0; ApplVar.PointerType < POINTERMAX; ApplVar.PointerType++)
                        {
                            if(ApplVar.PointerType == 0){

                                GetTenderOffSet();
                                RamOffSet += ApplVar.AP.Tend.TotalOffSet;
                                nextOff = RamOffSet;
                            }
                            else
                            {
                                RamOffSet = nextOff;
                            }

                            ApplVar.Size = ApplVar.AP.Tend.Size[ApplVar.PointerType];

							if(ApplVar.PointerType == 1)//clerk
						{
							saveClerkNumber = ApplVar.ClerkNumber;
							for(ApplVar.ClerkNumber = 1; ApplVar.ClerkNumber <= ApplVar.AP.Clerk.Number; ApplVar.ClerkNumber++)
                            {
                                size += TcpReadZport(sBuffer + size);
                            }
							ApplVar.ClerkNumber = saveClerkNumber;
						}
							else
								{
                            size += TcpReadZport(sBuffer + size);
								}

                            //jdb2018-05-15 跳向下一个地址
                            if(ApplVar.PointerType == 0)
                            {
                                nextOff += PeriodSkip(&(ApplVar.Size), 1);
                            }
                        }
                    }
                }
                else if(*(hostString + 2) == 0x08) //jdb2018-05-11 DRAWER
                {
                    ApplVar.DrawNumber = *((char *)(hostString + 3)) - 1;
                    if(ApplVar.DrawNumber < ApplVar.AP.Draw.Number){

                        GetDrawerOffSet();
                        ReadRam((BYTE*)&ApplVar.Draw, ApplVar.AP.Draw.TotalOffSet);	/* write function options */
                        ApplVar.Draw.Name[ApplVar.AP.Draw.CapSize] = 0 ;

                        sBuffer[size++] = ApplVar.AP.Draw.CapSize;

                        memcpy(sBuffer + size, ApplVar.Draw.Name, ApplVar.AP.Draw.CapSize);
                        size += ApplVar.AP.Draw.CapSize;

                        for(ApplVar.PointerType = 0; ApplVar.PointerType < POINTERMAX; ApplVar.PointerType++)
                        {
                            if(ApplVar.PointerType == 0){

                                GetDrawerOffSet();
                                RamOffSet += ApplVar.AP.Draw.TotalOffSet;
                                nextOff = RamOffSet;
                            }
                            else
                            {
                                RamOffSet = nextOff;
                            }

                            ApplVar.Size = ApplVar.AP.Draw.Size[ApplVar.PointerType];

							if(ApplVar.PointerType == 1)//clerk
						{
							saveClerkNumber = ApplVar.ClerkNumber;
							for(ApplVar.ClerkNumber = 1; ApplVar.ClerkNumber <= ApplVar.AP.Clerk.Number; ApplVar.ClerkNumber++)
                            {
                                size += TcpReadZport(sBuffer + size);
                            }
							ApplVar.ClerkNumber = saveClerkNumber;
						}
							else
								{
                            size += TcpReadZport(sBuffer + size);
								}

                            //jdb2018-05-15 跳向下一个地址
                            if(ApplVar.PointerType == 0)
                            {
                                nextOff += PeriodSkip(&(ApplVar.Size), 1);
                            }
                        }
                    }
                }
                else if(*(hostString + 2) == 0x10) //jdb2018-05-11 CORRECT
                {
                    ApplVar.CorrecNumber = *((char *)(hostString + 3)) - 1;
                    if(ApplVar.CorrecNumber < ApplVar.AP.Correc.Number){

                        GetCorrecOffSet();
                        ReadRam((BYTE*)&ApplVar.Correc, ApplVar.AP.Correc.TotalOffSet);	/* write function options */
                        ApplVar.Correc.Name[ApplVar.AP.Correc.CapSize] = 0 ;

                        sBuffer[size++] = ApplVar.AP.Correc.CapSize;

                        memcpy(sBuffer + size, ApplVar.Correc.Name, ApplVar.AP.Correc.CapSize);
                        size += ApplVar.AP.Correc.CapSize;

                        for(ApplVar.PointerType = 0; ApplVar.PointerType < POINTERMAX; ApplVar.PointerType++)
                        {
                            if(ApplVar.PointerType == 0){

                                GetCorrecOffSet();
                                RamOffSet += ApplVar.AP.Correc.TotalOffSet;
                                nextOff = RamOffSet;
                            }
                            else
                            {
                                RamOffSet = nextOff;
                            }

                            ApplVar.Size = ApplVar.AP.Correc.Size[ApplVar.PointerType];

							if(ApplVar.PointerType == 1)//clerk
						{
							saveClerkNumber = ApplVar.ClerkNumber;
							for(ApplVar.ClerkNumber = 1; ApplVar.ClerkNumber <= ApplVar.AP.Clerk.Number; ApplVar.ClerkNumber++)
                            {
                                size += TcpReadZport(sBuffer + size);
                            }
							ApplVar.ClerkNumber = saveClerkNumber;
						}
							else
								{
                            size += TcpReadZport(sBuffer + size);
								}

                            //jdb2018-05-15 跳向下一个地址
                            if(ApplVar.PointerType == 0)
                            {
                                nextOff += PeriodSkip(&(ApplVar.Size), 1);
                            }
                        }
                    }
                }
                else if(*(hostString + 2) == 0x20) //jdb2018-05-11 DISC
                {
                    ApplVar.DiscNumber = *((char *)(hostString + 3)) - 1;
                    if(ApplVar.DiscNumber < ApplVar.AP.Disc.Number){

                        GetDiscOffSet();
                        ReadRam((BYTE*)&ApplVar.Disc, ApplVar.AP.Disc.TotalOffSet);	/* write function options */
                        ApplVar.Disc.Name[ApplVar.AP.Disc.CapSize] = 0 ;

                        sBuffer[size++] = ApplVar.AP.Disc.CapSize;

                        memcpy(sBuffer + size, ApplVar.Disc.Name, ApplVar.AP.Disc.CapSize);
                        size += ApplVar.AP.Disc.CapSize;

                        for(ApplVar.PointerType = 0; ApplVar.PointerType < POINTERMAX; ApplVar.PointerType++)
                        {
                            if(ApplVar.PointerType == 0){

                                GetDiscOffSet();
                                RamOffSet += ApplVar.AP.Disc.TotalOffSet;
                                nextOff = RamOffSet;
                            }
                            else
                            {
                                RamOffSet = nextOff;
                            }

                            ApplVar.Size = ApplVar.AP.Disc.Size[ApplVar.PointerType];

							if(ApplVar.PointerType == 1)//clerk
						{
							saveClerkNumber = ApplVar.ClerkNumber;
							for(ApplVar.ClerkNumber = 1; ApplVar.ClerkNumber <= ApplVar.AP.Clerk.Number; ApplVar.ClerkNumber++)
                            {
                                size += TcpReadZport(sBuffer + size);
                            }
							ApplVar.ClerkNumber = saveClerkNumber;
						}
							else
								{
                            size += TcpReadZport(sBuffer + size);
								}

                            //jdb2018-05-15 跳向下一个地址
                            if(ApplVar.PointerType == 0)
                            {
                                nextOff += PeriodSkip(&(ApplVar.Size), 1);
                            }
                        }
                    }
                }
                else if(*(hostString + 2) == 0x40) //jdb2018-05-11 TAX
                {
                    ApplVar.TaxNumber = *((char *)(hostString + 3)) - 1;
                    if(ApplVar.TaxNumber < ApplVar.AP.Tax.Number){

                        GetTaxOffSet();
                        ReadRam((BYTE*)&ApplVar.Tax, ApplVar.AP.Tax.TotalOffSet);	/* write function options */
                        ApplVar.Tax.Name[ApplVar.AP.Tax.CapSize] = 0 ;

                        sBuffer[size++] = ApplVar.AP.Tax.CapSize;

                        memcpy(sBuffer + size, ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize);
                        size += ApplVar.AP.Tax.CapSize;

                        for(ApplVar.PointerType = 0; ApplVar.PointerType < POINTERMAX; ApplVar.PointerType++)
                        {
                            if(ApplVar.PointerType == 0){

                                GetTaxOffSet();
                                RamOffSet += ApplVar.AP.Tax.TotalOffSet;
                                nextOff = RamOffSet;
                            }
                            else
                            {
                                RamOffSet = nextOff;
                            }

                            ApplVar.Size = ApplVar.AP.Tax.Size[ApplVar.PointerType];
                            size += TcpReadZport(sBuffer + size);

                            //jdb2018-05-15 跳向下一个地址
                            if(ApplVar.PointerType == 0)
                            {
                                nextOff += PeriodSkip(&(ApplVar.Size), 1);
                            }
                        }
                    }
                }
                else
                {
                }
                TcpRecordToHost('Z',type,sBuffer, size);
#endif
            }
            break;
        default :
            cmdReturn = cmdANSWER;
            break;
    }


    if (cmdReturn)
    {//必须对host进行应答
        if (cmdReturn==cmdDOWNLOAD)
            err_state = TcpAnswerToHost(cmdDOWNLOAD,type+1);//应答主机,下载数据处理完毕,请求下载下一条
        else
            err_state = TcpAnswerToHost(cmdReturn,0);//应答主机,数据处理完毕
    }

    if (type > PLU1  && type<AGREE || (type >= UPDPLU && type <= REPPLU))
    {
        ApplVar.RepeatCount = 0 ;
        ApplVar.PluNumber = save;
        ReadPlu();
    } else if (type > DEPT  && type<PLU1)
    {
        ApplVar.DeptNumber = save;
        ReadDept();
    } else if (type > CLERK && type<OFFER)
    {
        ApplVar.ClerkNumber = save;

        if (save)
        {
            ReadClerk();
        }

    } else if (type > SALPER  && type<TAX)
    {
        ApplVar.SalPerNumber = save;
        ReadSalPer();
    }
    if (!TESTBIT(ApplVar.Clerk.Options, BIT7))
    {
        ApplVar.FTrain = 1 ;
        SETBIT(ARROWS, BIT2) ;
    } else
    {
        ApplVar.FTrain = 0 ;
        RESETBIT(ARROWS, BIT2) ;
    }
    return err_state;
}
#endif

//read a package from host,return the length of data in package
//return 0 if error
short  ReadRecord()
{
    BYTE length, bcc;
    short i;

#if defined(STM32F10X_HD_USBCOMM)
    if (FromUSB)
    {
        return ReadUSBRecord();
    }
#endif

    for (i=10;(i && (ReadComm(COMPUTER_1) != SOH));i--);//read SOH send by host
    if (!i)
    {
        SendComm(COMPUTER_1,NAK);//Error from host
        return 0;
    }
    EmptyComm(COMPUTER_1);
    SendComm(COMPUTER_1,ACK);//Send ACK to the host

    for (bcc=10;(bcc && (i = ReadComm(COMPUTER_1)) == -1);bcc--);//Read the length of the record
    if (!bcc)
    {
        SendComm(COMPUTER_1,NAK);//Error from host
        return 0;
    }
    length = i;
#if (PCBUFMAX<256)
    if (!length || length >= PCBUFMAX-2)
    {
        SendComm(COMPUTER_1,NAK);//Error from host
        return 0;
    }
#endif
    bcc = length;
    i = 0;
    for (i=0;i < length+1;i++)
    {//Read data from host an check-sum
        PCBuffer[i] = ReadComm(COMPUTER_1);
        bcc += PCBuffer[i];
    }
    if (bcc)//the verify error
    {
        SendComm(COMPUTER_1,NAK);//Error from host
        return 0;
    }
    else
    {
        SendComm(COMPUTER_1,ACK);//Ok from host
        return length;
    }
}

//#if (DD_SIODRVRAM==1)
//#pragma SECTION program bios_code_sh
//#endif
//communicat with Host computer throw Rs232 port
// 0 1 2 3 4 5 6
//:02000002F0000C
// |||  ||||  |\/______Check value
// |||  |||\__/________Datas (2 bytes)
// |||  |\/____________Flag, 00h=Data;01h=End of a segment;02h=New segment address;
// ||\__/______________Offset address in Segment
// \/__________________Length of Datas (02h=2 bytes
// 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0
//:10000000434C454152000000000000000000000089
//:100010004E554D31000000000000000000000000BF
//:04101000B3A32E0058
//:00000001FF		;End of the file


// download cclib or binary data from host. format of file is binary.

#if (ENABLE16)
// download cclib16 or binary data from host. format of file is binary.
void DowmLoadBINCLIB(BYTE libType)
{
#if (!defined(DEBUGBYPC) && defined(CASE_FORHANZI))
    BYTE length, bcc;
    BYTE sBuf;
    WORD sSta,BCCLib=0;
    int i,j;
    ULONG sAddr,sAddrEnd,sAddrFrom;

    length = ReadRecord();//读取字库窜方地址信息
    if (!length)
    {
        ApplVar.DownLoad = 0;
        return;
    }

    j = 0;
    for (i=1;i<length;i++)//skip the first ':'
    {
        if (PCBuffer[i]>'9')
            sBuf =  PCBuffer[i]-'A'+10;
        else
            sBuf =  PCBuffer[i]-'0';
        if (i & 1)
            bcc = sBuf;
        else
            PCBuffer[j++]= (bcc<<4) | sBuf;
    }
    bcc = 0;
    for (i=0;i<j;i++)
        bcc += PCBuffer[i];//check the data recived
    if (bcc)
    {
        ApplVar.DownLoad = 0;
        return;
    }

    if (libType==16)
    {
        sAddrFrom = sAddr = FLASH_HZK16_ADDR;//(char *)0+(((UnLong)PCBuffer[4])<<12) + (((UnLong)PCBuffer[1])<<8) +PCBuffer[2];
        sAddrEnd = FLASH_HZK16_ADDR+FLASH_HZK16_SIZE;
        if (sAddr==FLASH_HZK16_ADDR)
        {
            j=bFlashMBlockErase(FLASH_HZK16_BLOCKFr,FLASH_HZK16_BLOCKS);
        }
    }
    else if (libType==24)
    {
        sAddrFrom = sAddr = FLASH_HZK24_ADDR;//(char *)0+(((UnLong)PCBuffer[4])<<12) + (((UnLong)PCBuffer[1])<<8) +PCBuffer[2];
        sAddrEnd = FLASH_HZK24_ADDR+FLASH_HZK24_SIZE;
        if (sAddr==FLASH_HZK24_ADDR)
        {
            j=bFlashMBlockErase(FLASH_HZK24_BLOCKFr,FLASH_HZK24_BLOCKS);
        }
    }
    else if (libType==(16+24))
    {
        sAddrFrom = sAddr = FLASH_HZK16_ADDR;//(char *)0+(((UnLong)PCBuffer[4])<<12) + (((UnLong)PCBuffer[1])<<8) +PCBuffer[2];
        sAddrEnd = FLASH_HZK24_ADDR+FLASH_HZK24_SIZE;
        if (sAddr==FLASH_HZK16_ADDR)
        {
            j=bFlashMBlockErase(FLASH_HZK16_BLOCKFr,FLASH_HZK16_BLOCKS+FLASH_HZK24_BLOCKS);
        }
    }
    else
        return;
    if (j!=FLASH_SUCCESS)
    {
        memset(PCBuffer,' ',INPUTWIDTH);
        CopyFrStr(PCBuffer,Msg[CHUCUO].str);
        WORDtoASC(PCBuffer+INPUTWIDTH-1,abs(j));
        DispSt2C(PCBuffer);
        ApplVar.DownLoad = 0;
        return;
    }


    while (TRUE)
    {
        length = ReadRecord();

        if (!length ||(length == 3 && PCBuffer[0]=='U' && PCBuffer[1]=='C' && PCBuffer[2]=='A'))
        {
            sAddrFrom+=NOR_FLASH_ADDR;
            sAddr+=NOR_FLASH_ADDR;
            for (sAddrEnd=sAddrFrom;sAddrEnd<sAddr;sAddrEnd++)
            {//统计下载数据的累加和
                BCCLib += FlashReadByte(sAddrEnd);
            }
            DisplayHex((char*)&BCCLib,2);
            DispSt2C(Msg[WANCHENG].str);
            break;
        }
        if (sAddr>=sAddrFrom && sAddr<=sAddrEnd-length)
        {
            if ((j = bFlashProgram(sAddr, length, PCBuffer)) != FLASH_SUCCESS)
            {
                memset(PCBuffer,' ',INPUTWIDTH);
                CopyFrStr(PCBuffer,Msg[CHUCUO].str);
                WORDtoASC(PCBuffer+INPUTWIDTH-1,abs(j));
                DispSt2C(PCBuffer);
            }
        }
        sAddr += length;
    }
    ApplVar.DownLoad = 0;
#endif
}
#endif
// download cclib or binary data from host. format of file is INTEL HEX.
/*
   void DowmLoadHEXCLIB()
   {
   BYTE length, bcc;
   BYTE sByte;
   WORD SegAddr;
   short  i,j,k;

   CONSTCHAR *OffAddr;

   SegAddr = 0;
   while (TRUE)
   {
   length = ReadRecord();

   if (!length ||(length == 3 && PCBuffer[0]=='U' && PCBuffer[1]=='C' && PCBuffer[2]=='A'))
   {
   if ((ApplVar.DownLoad & 0x0f)==0x0c)
   FlashReadReset();
   DispSt2C(Msg[WANCHENG].str);
   break;
   }

   j = 0;
   for (i=1;i<length;i++)//skip the first ':'
   {
   if (PCBuffer[i]>'9')
   sByte =  PCBuffer[i]-'A'+10;
   else
   sByte =  PCBuffer[i]-'0';
   if (i & 1)
   bcc = sByte;
   else
   PCBuffer[j++]= (bcc<<4) | sByte;
   }
   bcc = 0;
   for (i=0;i<j;i++)
   bcc += PCBuffer[i];//check the data recived

   if (PCBuffer[3] == 0x02)//new address for data
   {
   SegAddr = PCBuffer[4];
   strcpy(PCBuffer,"下载地址      ");
//Ccr 			strcpy(PCBuffer,"下载地址      ");
HEXtoASC(PCBuffer+INPUTWIDTH-1,(char *)&SegAddr,2);
DispSt2C(PCBuffer);
}
else if (PCBuffer[3] == 0x01)// at the end of HEX file
{
DispSt2C(Msg[WANCHENG].str);
break;
}
else
{
OffAddr = (char*)0x0L +((UnLong)SegAddr<<12) +(((UnLong)PCBuffer[1])<<8) +PCBuffer[2];
length = PCBuffer[0];

if ((ApplVar.DownLoad & 0x0f)==0x0c)
{
if (OffAddr>=(char*)0x40000L && OffAddr<=(char*)0xbffffL)
if (FlashProgram((UnLong)(OffAddr-(char*)0x40000L), length, PCBuffer+4) != FLASH_SUCCESS)
{
strcpy(PCBuffer,"写错         ");
//Ccr 		    			strcpy(PCBuffer,"写错         ");
WORDtoASC(PCBuffer+INPUTWIDTH-1,abs(j));
DispSt2C(PCBuffer);
}
}
else
memcpy(OffAddr,PCBuffer+4,length);
}
}
ApplVar.DownLoad = 0;
}
*/
/**
 * 下载用户自定义图片数据,主要下载的图片中应该包含文字
 *  图片数据最多为4096字节
 * @author EutronSoftware (2014-03-25)
 */
void DowmLoadGRB()
{
#if !defined(CASE_INNERDOTPRN)

#if (!defined(DEBUGBYPC) && DOWN_GR_SIZE)
    BYTE length, bcc,sBuf;
    int i,j;
#if defined(STM32F10X_HD)
    char *GRBAddrFrom=(char *)DOWN_GR_ADDR;
#else
    char *GRBAddrFrom=(char *)BKPSRAM_BASE;
#endif
    ULONG sAddr;

    length = ReadRecord();//读取地址信息
    if (!length)
    {
        ApplVar.DownLoad = 0;
        return;
    }

    j=0;
    for (i=1;i<length;i++)//skip the first ':'
    {
        if (PCBuffer[i]>'9')
            sBuf =  PCBuffer[i]-'A'+10;
        else
            sBuf =  PCBuffer[i]-'0';
        if (i & 1)
            bcc = sBuf;
        else
            PCBuffer[j++]= (bcc<<4) | sBuf;
    }
    bcc = 0;
    for (i=0;i<j;i++)
        bcc += PCBuffer[i];//check the data recived
    if (bcc)
    {
        ApplVar.DownLoad = 0;
        return;
    }

    sAddr=0;
    while (TRUE)
    {
        length = ReadRecord();

        if (!length ||(length == 3 && PCBuffer[0]=='U' && PCBuffer[1]=='C' && PCBuffer[2]=='A'))
        {

            InfoSelf_Graph.cGrapCols = 0;
            InfoSelf_Graph.cGrapSize = 0;
            //memset(&InfoSelf_Graph,0,sizeof(InfoSelf_Graph));
            if (sAddr>20)//下载的图片数据为符合要求的数据
            {
                ChangeGRBToMyself();
            }
            else
            {
                GRB_Define.cGraSize=0;
                GRB_Define.cTextFlag=0;
            }

            DispSt2C(Msg[WANCHENG].str);
            break;
        }
        if (sAddr==0 && PCBuffer[0]!=0x80)
        {//无字符区,强制增加字符区,为GRB_Define.cGraSize保留空间
            PCBuffer[0]=0x80;
            for (i=0;i<5;i++)
                GRBAddrFrom[sAddr++]=PCBuffer[i];
            for (i=0;i<5;i++)
                GRBAddrFrom[sAddr++]=0;
            for (i=5;i<length;i++)
                GRBAddrFrom[sAddr++]=PCBuffer[i];
        }
        else
        {
            for (i=0;i<length && sAddr<DOWN_GR_SIZE;i++) //最多4K数据
                GRBAddrFrom[sAddr++]=PCBuffer[i];
        }
        memset(PCBuffer,' ',INPUTWIDTH);
        //CopyFrStr(PCBuffer,Msg[ZZXZTXING].str);
        GRB_Define.cGraSize = sAddr;
        WORDtoASC(PCBuffer+5,GRB_Define.cGraSize);
        PCBuffer[6]=0;
        DispStrXY(PCBuffer+1,INPUTDISPX,STATELINE);
    }
    ApplVar.DownLoad = 0;
#endif
#endif
}


#if defined(STM32F10X_HD)
//read a package from host,return the length of data in package
//return 0 if error
short  ReadUSBRecord()
{
    BYTE length, bcc;
    ULONG i,j;

    for (i=0xffffffff;i;i--)//read SOH send by host
    {
        if (CheckUSB() || USB_COMM_Ready())
            if (ReadUSB(&bcc) && bcc==SOH)
                break;
    }
    if (!i)
    {
        SendUSB(NAK);//Error from host
        return 0;
    }
    //EmptyUSB();
    SendUSB(ACK);//Send ACK to the host

    for (i=0xffffffff;i;i--)//Read the length of the record
    {
        if (CheckUSB() || USB_COMM_Ready())
            if (ReadUSB(&length))
                break;
    }

    if (!i)
    {
        SendUSB(NAK);//Error from host
        return 0;
    }

#if (PCBUFMAX<256)
    if (!length || length >= PCBUFMAX-2)
    {
        SendUSB(NAK);//Error from host
        return 0;
    }
#endif
    bcc = length;
    i = 0;
    for (i=0;i < length+1;i++)
    {//Read data from host an check-sum
        while (!CheckUSB())
        {
            USB_COMM_Ready();
        }
        ReadUSB(PCBuffer+i);
        bcc += PCBuffer[i];
    }
    if (bcc)//the verify error
    {
        SendUSB(NAK);//Error from host
        return 0;
    }
    else
    {
        SendUSB(ACK);//Ok from host
        return length;
    }
}

/*
   When CheckComm return SOH, system can read data from rs232 use readcomm.
   the first byte is the length of data-L, then are the Byte data-B;
   the last is verify byte-V;
   V = ~(L + B)+1
   */
BYTE SendUSBRecord(BYTE *p ,BYTE length )
{
    ULONG retry;
    BYTE bcc,i,cpT;
    BYTE *record;
    BYTE sACK;

    if (!length)
        return FALSE;
    EmptyUSB();	/* empty port */
    //ccr2016-04-18    for (retry=0;retry<5;retry++)
    {
        SendUSB(SOH); // "发送SOH 建立通信；"
#if defined(CASE_INDONESIA)//印度版本:无须服务器应答SOH
        if (p[0]!='F')
#endif
        {
            for (retry=0xffffffff;retry;retry--)
            {
                if (CheckUSB() || USB_COMM_Ready())
                    if (ReadUSB(&sACK) && (sACK==ACK || sACK==NAK))
                        break;
            }
            if (!retry || sACK==NAK)
                return FALSE;	// " failure"
        }
        record = p;
        bcc = length;

        SendUSB(length); //send the packet length;
        for (i = 0;i<length;i++)
        {
            bcc += *record;	// get the jiaoyanhe..BCC
            record += 1;
        }
        cpT = *record;
        *record = (~bcc+1);
        SendUSBString(p,length+1); // Send the data..

        *record = cpT;


        for (retry=0xffffffff;retry;retry--)
        {
            if (CheckUSB() || USB_COMM_Ready())
                if (ReadUSB(&sACK) && (sACK==ACK || sACK==NAK))
                    break;
        }
        if (!retry || sACK==NAK)
            return FALSE;	// " failure"

        if (sACK==NAK)
            return false;		// Send data failure..
        if (retry)
            return TRUE;		// success.
    }
    return FALSE;
}
#endif
