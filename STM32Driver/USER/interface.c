
#include "king.h"
#include "exthead.h"
#include "interface.h"
#include "stm32f2xx_it.h"
#include "pwr_ctrl.h"

#if defined(CASE_ASCIIDISPLAY)
#include "lcd_160x.h"
#include "monitor.h"
#else
extern void LED_Init(void);
#endif

extern const char MsgSRAMNOTFOUND[];

#define YEARFrDEC 2000      //RTC中年开始年份
#define YEARFrHEX 0x2000    //RTC中年开始年份


DATE_BCD rtc_date;          //    buffer per get/set data corrente
TIME_BCD rtc_time;          //    buffer per get/set orario corrente

void KeyBoardProc(void);
void PFOInt_Init(void);

extern void Beep(uint8_t blen);
void msDelay(__IO uint32_t nTime);

volatile BYTE DrawerOpen_time = 0;

#if defined(FOR_DEBUG)
#define FORJLINK	1	//使用JLINK进行下载和调试时,将其置1
#else
#define FORJLINK	0	//使用JLINK进行下载和调试时,将其置1
#endif

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */


/* Get RTC time */
void rtc_gettime (RTCTime *rtc)
{
    RTC_TimeTypeDef  RTC_TimeStruct;
    RTC_DateTypeDef  RTC_DateStruct;
    RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

    rtc->year = RTC_DateStruct.RTC_Year + YEARFrDEC;
    rtc->mon = RTC_DateStruct.RTC_Month;
    rtc->mday = RTC_DateStruct.RTC_Date;
    rtc->wday = RTC_DateStruct.RTC_WeekDay;

    rtc->hour = RTC_TimeStruct.RTC_Hours;
    rtc->min = RTC_TimeStruct.RTC_Minutes;
    rtc->sec = RTC_TimeStruct.RTC_Seconds;
}

/* Set RTC time */
void rtc_settime (const RTCTime *rtc)
{
    RTC_TimeTypeDef  RTC_TimeStruct;
    RTC_DateTypeDef  RTC_DateStruct;

    RTC_DateStruct.RTC_Year        = rtc->year;
    RTC_DateStruct.RTC_Month       = rtc->mon ;
    RTC_DateStruct.RTC_Date        = rtc->mday;
    RTC_DateStruct.RTC_WeekDay     = rtc->wday;

    RTC_TimeStruct.RTC_Hours       = rtc->hour;
    RTC_TimeStruct.RTC_Minutes     = rtc->min ;
    RTC_TimeStruct.RTC_Seconds     = rtc->sec ;

    RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct) ;

}
/****************************************************************************
注意:
    收款机的所有业务数据的存/取都是通过ReadRam和WriteRam来进行,
ReadRam和WriteRam负责存储空间的管理,其管理的空间地址对应用程序是透明的.
即:应用程序只向ReadRam或WriteRam提供有效存储空间连续地址来访问数据
   至于存储空间是否连续,存储介质是FLASH芯片还是SRAM或是其它介质,
   由 ReadRam或WriteRam内部处理.

   应用程序运行所需变量空间不在ReadRam和WriteRam管理之列.
****************************************************************************/
/**
 * 根据RamOffSet所指定的地址,从文件数据存储区中读取len字节的数据
 * 读取完成之后,RamOffSet自动+len
 * @author EutronSoftware (2014-08-26)
 *
 * @param buf :读取的数据存放区
 * @param len :读取数据字节数
 */
void ReadRam( BYTE *buf,WORD len)
{

#if FLASH_AppRAM
    WORD temp_len;
    DWORD temp_addr;

    if (RamOffSet<FLASH1SIZE)
    {
        FlashOpt = 0;
        if ((RamOffSet + len)<=FLASH1SIZE)
            SSTF080B_RD(RamOffSet, len, buf);
        else
        {
            temp_len = FLASH1SIZE-RamOffSet;
            SSTF080B_RD(RamOffSet, temp_len, buf);
            FlashOpt = 1;
            SSTF080B_RD(0,  len-temp_len,  &buf[temp_len]);//second flash
        }
    }
    else if (RamOffSet<FLASHTOTAL)                           //second flash
    {
        FlashOpt = 1;
        temp_addr = RamOffSet - FLASH1SIZE;
        if ((RamOffSet + len)>FLASHTOTAL)
        {
            len = FLASHTOTAL-RamOffSet;
        }
        SSTF080B_RD(temp_addr, len, buf);
    }
#else
    //if ((RamOffSet + len)<=ApplVar.SIZE_EXTRAM)
        SRAM_ReadBuffer(buf,RamOffSet, len);
#endif
    RamOffSet += len;
}
/**
 * 根据RamOffSet所指定的地址,向文件数据存储区中写入len字节的数据
 * 写入完成之后,RamOffSet自动+len
 * @author EutronSoftware (2014-08-26)
 *
 * @param buf :要写入的数据
 * @param len :数据字节数
 */
void WriteRam( BYTE *buf, WORD len)   /* (void *from, WORD bytes) write bytes to ram banks */
{
#if FLASH_AppRAM
    WORD temp_len;
    DWORD temp_addr;

    if (RamOffSet<FLASH1SIZE)
    {
        FlashOpt = 0;
        if ((RamOffSet + len)<=FLASH1SIZE)
            WriteBlock(RamOffSet, buf, len);
        else
        {
            temp_len = FLASH1SIZE-RamOffSet;
            WriteBlock(RamOffSet, buf, temp_len);
            FlashOpt = 1;
            WriteBlock(0, &buf[temp_len], len-temp_len);   //second flash
        }
    }
    else if (RamOffSet<FLASHTOTAL)                           //second flash
    {
        FlashOpt = 1;
        temp_addr = RamOffSet - FLASH1SIZE;
        if ((RamOffSet + len)>FLASHTOTAL)
        {
            len = FLASHTOTAL-RamOffSet;
        }
        WriteBlock(temp_addr, buf, len);
    }
#else
//    if ((RamOffSet + len)<=ApplVar.SIZE_EXTRAME)
        SRAM_WriteBuffer(buf, RamOffSet, len);
#endif
    RamOffSet += len;
}
/****************************************************************************/



void DrawerOpen(void)//打开钱箱
{
    DRAWER_OPEN();
    DrawerOpen_time = 5;
}


//Bios所提供的功能列举如下
//有可能全部列举应用程序所使用到的功能.如果应用程序使用了在此未列出的功能,
//那么对应的Bios模块将会打印Command XX提示,此时应该为XX命令开发对应的程序.
WORD Bios(WORD cmd, void  *ptr, WORD par1, WORD par2)
{
    BYTE i, BCount;
    DWORD sect;
    BYTE aBuf[3];
    switch (cmd)
    {
    case BiosCmd_CC_Compare://
        //(BOOL)Result = Bios(BiosCmd_CC_Compare, (char*)SourceBuff, (WORD)ChipAdrress, (WORD)NumBytes )
        //比较SourceBuff中的数据是否与IC卡地址ChipAdrress中长度为NumBytes的数据是否一致,
        break;
    case BiosCmd_CC_Read:
        //(BOOL)Result = Bios(BiosCmd_CC_Read, (char*)DestBuff, (WORD)ChipAdrress, (WORD)NumBytes )
        //从IC卡地址ChipAdrress中读取长度为NumBytes的数据存入DestBuff
        return 0;
    case BiosCmd_CC_Write:
        //(BOOL)Result = Bios(BiosCmd_CC_Write, (far char*)SourceBuff, (WORD)ChipAdrress, (WORD)NumBytes )
        //把SourceBuff中的数据写入IC卡地址ChipAdrress中
        return 0;
#if (defined(DD_MMC) || defined(CASE_EJSD))
    case BiosCmd_SD_Erase:
        //Bios(BiosCmd_SD_Erase, (far char*)SourceBuff, (WORD)GROUP_L, (WORD)GROUP_H )
        //擦除MMC卡中由GROUP_H*65536+GROUP_L所指定的组的数据, MMC中组的划分需要参见相关手册
        if(SD_Detect()!= SD_PRESENT)
            return (SD_Erase(((uint32_t)par1<<16)+par2,((uint32_t)par1<<16)+par2+SDBLOCKSIZE)==SD_OK);
        else
            return false;
    case BiosCmd_SD_ReadInfo:
        //Bios(BiosCmd_SD_ReadInfo, (char*)sBuf, MMC_SEND_CID,16)
        //Bios(BiosCmd_SD_ReadInfo, (char*)sBuf, MMC_SEND_CSD,16)
        //读取MMC卡的CID或CSD,存入sBuf中
        if((SD_Detect()!= SD_PRESENT) || SD_GetCardInfo(&SDCardInfo) != SD_OK)
             return false;
        if (ptr)
        {
            if(par1 == MMC_SEND_CID)
            {
                memcpy(ptr,(char*)&SDCardInfo.SD_cid,16);
            }
            else if(par1 == MMC_SEND_CSD)
            {
                memcpy(ptr,(char*)&SDCardInfo.SD_csd,16);
            }
        }

        return true;

    case BiosCmd_SD_UnProtect:
        //Bios(BiosCmd_SD_UnProtect, (far char*)SourceBuff, (WORD)GROUP_L, (WORD)GROUP_H )
        //取消对MMC卡中由GROUP_H*65536+GROUP_L所指定的组, MMC中组的划分需要参见相关手册
        break;
    case BiosCmd_SD_Init:
        //Bios(BiosCmd_SD_Init, (far char*)SourceBuff, 0, 0 )
        //初始化MMC卡, MMC的初始化见相关手册

        NVIC_SDConfiguration();

        if (SD_Detect()== SD_PRESENT)
        {
            //SD_PWR_ON();
            return (SD_Init () == SD_OK);
        }
        else
            return false;
    case BiosCmd_SD_ReadBlock:
        //Bios(BiosCmd_SD_ReadBlock, buf,addr_L,  addr_H)
        //从MMC卡中的addr_h*65536+addr_L地址中读取一块数据,存入buf中,通常一块为512字节
//        return  SRAM23K256_RD(((par2 << 16) | par1)+0x4000, 512,(BYTE *)ptr);//testonly
        sect = ((par2 << 16) | par1);
        return SD_ReadMultiBlocks((BYTE *)ptr,sect,  SDBLOCKSIZE,2)==SD_OK;
    case BiosCmd_SD_WriteBlock:
        //Bios(BiosCmd_SD_WriteBlock, buf,addr_L,  addr_H)
        //把buf中的一块数据写入从MMC卡中的addr_h*65536+addr_L地址中,通常一块为512字节
//        return SRAM23K256_WR(((par2 << 16) | par1)+0x4000, (BYTE *)ptr, 512);//testonly
        sect = ((par2 << 16) | par1);
        return (SD_WriteMultiBlocks((BYTE *)ptr, sect,  SDBLOCKSIZE,2)==SD_OK);
    case BiosCmd_SD_CheckPresence:
        //Bios(BiosCmd_SD_CheckPresence, 0, 0, 0)
        //检查MMC卡是否存在
        return (SD_Detect()==SD_PRESENT);

#endif
    case BiosCmd_PrintGraph:
        //Bios(BiosCmd_PrintGraph, (char*)idx, option, position);
        //打印由idx所指定的图片
        //option:   bit0  =1,将GrafTexts中的文字叠加到图片上
        //          bit1  =1,使用图片自带的文字叠加到图片上
        //position:指定图片打印的字符位置

#if !defined(CASE_INNERDOTPRN)
        if (!TESTBIT(ApplVar.MyFlags, CLOSEPRINT))
        {
            aBuf[0] = (BYTE)ptr;//get the graphic index
            BCount = 2;
            if (par1 & 0x01)
            {
                if (ApplVar.GrafTexts[0][0] == 0)
                    aBuf[1] = '0';//没有需要叠加的文字
                else
                {
                    if (par2 && par2<=PRTLEN)
                    {//指定打印位置
                        aBuf[2] = par2;
                        BCount=3;
                    }
                    aBuf[1] = '1';//把文字叠加在图片上
                    RSPrint(aBuf, BCount, CMDP_PRGRA);
                    for (i=0; i<TEXTSOFGRAPH && ApplVar.GrafTexts[i][0]; i++)
                        RSPrint(ApplVar.GrafTexts[i], PRTLEN-1, CMDP_R);
                    aBuf[0] = 0;
                    RSPrint(aBuf, 1, CMDP_R);
                    return true;
                }

            }
            else if (par1 & 0x02)
            {//使用图片自带的文字叠加到图片上
                aBuf[1] = '2';
            }
            if (par2 && par2<=PRTLEN)
            {//指定打印位置
                aBuf[2] = par2;
                BCount=3;
            }
            RSPrint(aBuf, BCount, CMDP_PRGRA);
        }
#endif
        return true;
    case BiosCmd_PrintRestart:
        //重新启动打印,M3系统下没有使用
        break;
    case BiosCmd_PrintX:
#if !defined(CASE_INNERDOTPRN)
        //Bios(BiosCmd_PrintX, char *Line, WORD Cmd , WORD DotLinesBlank);
        //向打印机发送打印数据,Line为要打印的数据;Cmd为打印命令;DotLinesBlank为打印完成后的走纸行数
        //Cmd将根据新开发的程序来进行命名
        if (ptr)
        {
            BCount = strlen(ptr);
            if (BCount > PRTLEN)
                BCount = PRTLEN;
        }
        else
            BCount = 0;

        if (!TESTBIT(ApplVar.MyFlags, CLOSEPRINT))
            RSPrint((BYTE *)ptr, BCount, par1);//对于要打印的数据需要处理，
#endif
        return true;

    case BiosCmd_PrintDirect://目前的打印机程序尚无此功能
        //Bios(BiosCmd_PrintDirect, char *Line, WORD Cmd , WORD DotLinesBlank);
        //向打印机发送打印数据,Line为要打印的数据;Cmd为打印命令;DotLinesBlank为打印完成后的走纸行数
        //Cmd将根据新开发的程序来进行命名
        //使用BiosCmd_PrintDirect来打印时,打印数据不加入打印任务池,而是直接打印
        break;
    default://用来跟踪其它命令,当发现有尚未开发的命令时,显示命令代码,然后再开发对应程序
        USART_printf("Command %d\n",cmd);
        break;
    }
    return 0;
}


WORD  Bios_1(WORD cmd)
{
    RTC_DateTypeDef  RTC_DateStruct;
    RTC_TimeTypeDef  RTC_TimeStruct;

    switch (cmd)
    {
    case BiosCmd_PrintCheck:
        //Bios_1(BiosCmd_PrintCheck);
        //读取打印机的状态:缺纸/压杆抬起/过热等
		Get_Printer_status();
        return CWORD(Printer_Status);

    case BiosCmd_ResetCutter:
        //Bios_1(BiosCmd_ResetCutter)
        //复位打印机切刀
#if !defined(CASE_INNERDOTPRN)
        if (!TESTBIT(ApplVar.MyFlags, CLOSEPRINT))
            RSPrint(0, 0, PRN_CUTRESET_Mode);
#endif
        break;
    case BiosCmd_RestDisp0:
        //Bios_1(BiosCmd_RestDisp0);
        //重新显示显示屏幕的内容,通常用于关机后再开机时显示关机前保存的屏幕内容
#if !defined(CASE_ASCIIDISPLAY)
        LED_On();
        PutsO_Saved();
#endif
        break;
    case BiosCmd_SaveDisp0:
        //保存屏幕上的显示内容.
        //如:当收款机显示了正常的信息,突然遇到出错时,先将当前内容保护,然后再显示出错信息
        break;
    case BiosCmd_ClrFip0:
#if !defined(CASE_ASCIIDISPLAY)
        //清除LED显示
        LED_Off();
#endif
        return 0;

    case BiosCmd_AutoClickOff:
        //关闭按键自动蜂鸣功能
        EnableBeep(false);
        break;
    case BiosCmd_AutoClickOn:
        //打开按键自动蜂鸣功能
        EnableBeep(true);
        break;
    case BiosCmd_CC_Close:
        //关闭IC卡,即关闭IC卡的驱动电源
        return 0;
    case BiosCmd_BuzzL:
        Beep(3);
        break;
    case BiosCmd_BuzzM:
        Beep(2);
        break;
    case BiosCmd_BuzzS:
        //分别为长/中/短时间蜂鸣
        Beep(1);
        break;
    case BiosCmd_GetDate:
        //读取日期到rtc_date中,日期格式为
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

        rtc_date.dd = DECtoBCD(RTC_DateStruct.RTC_Date);
        rtc_date.mm = DECtoBCD(RTC_DateStruct.RTC_Month);
        rtc_date.yy = DECtoBCD(RTC_DateStruct.RTC_Year);
        rtc_date.w = DECtoBCD(RTC_DateStruct.RTC_WeekDay);
        return 1;

    case BiosCmd_GetTime:
        //读取时间到rtc_time中,时间格式为:
        RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
        rtc_time.hh = DECtoBCD(RTC_TimeStruct.RTC_Hours);
        rtc_time.mm = DECtoBCD(RTC_TimeStruct.RTC_Minutes);
        rtc_time.ss = DECtoBCD(RTC_TimeStruct.RTC_Seconds);
        return 0;

    case BiosCmd_SetDate:
        //按照rtc_date中的日期设置日期
        RTC_DateStruct.RTC_Year        = BCDtoDEC(rtc_date.yy);
        RTC_DateStruct.RTC_Month       = BCDtoDEC(rtc_date.mm) ;
        RTC_DateStruct.RTC_Date        = BCDtoDEC(rtc_date.dd);
        RTC_DateStruct.RTC_WeekDay     = BCDtoDEC(rtc_date.w);

        RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
        return 0;
    case BiosCmd_SetTime:
        //按照rtc_time中的时间设置时间
        RTC_TimeStruct.RTC_Hours       = BCDtoDEC(rtc_time.hh);
        RTC_TimeStruct.RTC_Minutes     = BCDtoDEC(rtc_time.mm) ;
        RTC_TimeStruct.RTC_Seconds     = BCDtoDEC(rtc_time.ss) ;
        RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct) ;
        return 0;
    case BiosCmd_CheckKeyborad:

        //判断是否有按键按下,但是不读取键值。1－有键按下，0－ 无键按下
        return CheckKeyboard();

    default://用来跟踪其它命令,当发现有尚未开发的命令时,显示命令代码,然后再开发对应程序
        USART_printf("Command_1 %d\n",cmd);
        break;
    }
    return 0;
}

WORD Bios_2(WORD cmd, void  *ptr)
{

    switch (cmd)
    {
    case BiosCmd_CC_Open:
        //打开IC卡,即接通IC卡的电源,同时读取IC卡的4字节属性,返回成功与否
        break;
    case BiosCmd_CC_VerifyPsw:
        //校验IC卡密码.返回成功与否
        break;
    case BiosCmd_CC_WritePsw:
        //修改IC卡密码,返回成功与否
        break;
    default://用来跟踪其它命令,当发现有尚未开发的命令时,显示命令代码,然后再开发对应程序
        USART_printf("Command_2 %d\n",cmd);
        break;
    }
    return 0;
}

WORD Bios_SetKeyboard(BYTE Code, WORD NumKeys, CONSTBYTE *TabPnt, BYTE TabRecLen)
{//重新定义键盘
    return 0;
}

WORD Bios_SetCutter(BYTE Stato)
{//打开切刀,Stato=0:关闭切刀;Stato=1:打开切刀
    return 0;
}



void SetRTS(BYTE port)
{//设置串口port的RTS
 //0-串口1,1-串口2.....
    if (port == 0)
        UART0_RTS_SET();
    else if (port == 1)
        UART1_RTS_SET();
#if USART_EXTEND_EN
	else if (port == 2)
        UART2_RTS_SET();
#endif
}

//set RTS to 'L' for Serial 1,2
void ClrRTS(BYTE port)
{//清除串口的RTS
 //1-串口1,2-串口2.....
    if (port == 1)
        UART0_RTS_CLR();
    else if (port == 2)
        UART1_RTS_CLR();
#if USART_EXTEND_EN
	else if (port == 3)
        UART2_RTS_CLR();
#endif
}

// 读取串口CTS状态,status=0表示外设处于正常状态
BYTE GetCTS(BYTE port)
{
//1-串口1,2-串口2.....
    int i;
    BYTE status[2];

    status[0]=status[1]=0;

    do
    {
        for (i=0;i<2;i++)
        {
            if (i)
                for (status[1]=0;status[1]<10;status[1]++);
            if (port == 1)
                status[i] = UART0_CTS_GET();
            else if (port == 2)
                status[i] = UART1_CTS_GET();
        #if USART_EXTEND_EN
                else if (port == 3)
                status[i] = UART2_CTS_GET();
        #endif
        }

    } while (status[0]!=status[1]);
    return(status[0]);
}



WORD Bios_SetTic(WORD Interval)
{//略
    return 0;
}

WORD Bios_SetPortBusy(BYTE NumCom, BYTE Satato, BYTE ChrRx, BYTE ChrTx)
{//略
    return 0;
}

WORD Bios_TestMAC(void)
{//测试MAC跳线器状态

    int sMACCount=0;
    ULONG t1,t2;

    do
    {
        t1 = (MAC_PORT->IDR & MAC_PIN);
        usDelay(1000);//间隔1ms检测一次
        t2 = (MAC_PORT->IDR & MAC_PIN);
        if (t1==t2)
		{
            sMACCount++;
			if (t1 && sMACCount>2)
				break;//如果检测为未短接MAC,只检测两次,加快开机速度
		}
        else
            sMACCount = 0;

    } while (sMACCount<50);//共检测50次

    if (!t1)
        return true;  // MAC on
    else
        return false;  // MAC OFF
}


WORD Bios_FM_BlankCheck(UnLong SrcAdr, WORD Num)
{//检查税控存储器地址SrcAdr中的Num字节是否为空
#if 0
    BYTE temp_checkBuf[Num];
    BYTE i;
    M25P80_RD(SrcAdr, Num, temp_checkBuf);
    for (i=0; i<Num; i++)
    {
        if (temp_checkBuf[i] != 0xFF)
            return 0;
    }
#endif
    return 1;
}
//////

BYTE Bios_FM_CheckPresence(void)
{//检查税控存储器是否存在
    return FMDetect();
}


#define FIRMWARE_ADDR  ((uint32_t)0x08000000L)    //程序空间地址
#define FIRMWARE_SIZE  ((uint32_t)0x80000L)      //程序空间大小,实际大小为0x80000,代码大小不超过0x60000

WORD CheckSum(void)
{//检测程序代码校验和

    WORD checksum=0;
    uint32_t TempAddr;

    for (TempAddr = 0; TempAddr<FIRMWARE_SIZE; TempAddr+=2) /* while there is data to write */
    {
        /* Transfer data to the memory */
        checksum += *(WORD *) (FIRMWARE_ADDR + TempAddr);
    }

    return checksum;
}

long TestRam(void)
{//检测RAM

    return 0x20000l;//sizeof the RAM in MCU
}
//按字节操作方式测试扩展RAM的读写正确性
long TestLMM(WORD lmmPage)
{

    BYTE temp_save,temp_check;
    long i,SizeOfSRAM;
    BYTE *pSRAM;

    SizeOfSRAM=ApplVar.SIZE_EXTRAM;
    if (SizeOfSRAM)
    {//装配了SRAM,每块内存256K
        pSRAM = (BYTE *)SRAM_ADDR;
        for (i=0;i<SizeOfSRAM;i++)
        {
            temp_check = 0x55;
            temp_save = *pSRAM;
            *pSRAM = temp_check;
            temp_check = *pSRAM;

            if (temp_check != 0x55)
            {
                *pSRAM = temp_save;
                return(-(i+SRAM_ADDR));
            }
            temp_check = 0xAA;
            *pSRAM = temp_check;
            temp_check = *pSRAM;
            if (temp_check != 0xAA)
            {
                *pSRAM = temp_save;
                return(-(i+SRAM_ADDR));
            }
            *pSRAM++ = temp_save;
        }
        return SizeOfSRAM;
    }
   return 0;

}
//清除应用数据存储区,同时在EXTRAMN中标示SRAM块数(256K/块),
//在ApplVar.SIZE_EXTRAM中记录SRAM大小
void ClsXRam(void)
{
#if FLASH_AppRAM
    FlashOpt = 0;
    SSTF080B_EraseChip();
    FlashOpt = 1;
//testonly	SSTF080B_EraseChip();
    FlashErase = 0;
    ApplVar.SIZE_EXTRAM=0x100000L;    //size of flash
    EXTRAMN |= (ApplVar.SIZE_EXTRAM>>18);   //标示第一块SRAM
#else

//    WORD *pSRAM=(WORD*)SRAM_ADDR;

    EXTRAMN &= 0xf0;
    //检测第一块SRAM是否存在

//    *pSRAM = 0x55AA;

//    if (*pSRAM == 0x55AA)//检测是否有扩展SRAM
    {
        ApplVar.SIZE_EXTRAM=SRAMSIZE;//testonly SRAM_Detect();
        if (ApplVar.SIZE_EXTRAM)
        {
            EXTRAMN |= (ApplVar.SIZE_EXTRAM>>18);   //标示SRAM块数(256K/每块)

            //清除扩展RAM中的数据
#if ApplVarInSRAM
            memset((void*)(SRAM_ADDR+sizeof(WORD)+sizeof(ULONG)+sizeof(struct APPTEXT)+sizeof(struct APPLICATION)),0,
                   ApplVar.SIZE_EXTRAM-(sizeof(WORD)+sizeof(ULONG)+sizeof(struct APPTEXT)+sizeof(struct APPLICATION)));
#else
            memset((void*)SRAM_ADDR,0,ApplVar.SIZE_EXTRAM);
#endif
        }
        else
        {//无扩展内存,系统无法运行
            while (KbHit()) Getch();//清除已按键
            while (!KbHit())
            {
                Beep(3);
#if defined(CASE_ASCIIDISPLAY)
                PutsO(MsgSRAMNOTFOUND);
#else
                mClearScreen();
                Delay(500);
                DispStrXY(MsgSRAMNOTFOUND,0,0);
                Delay(500);
#endif
            }
        }
    }

#endif
}



short Bell(short len)
{//根据len分别响铃长/中/短时长
    Beep(len);
    return 0;
}


WORD CC_Insert(void)
{//检测IC卡是否插入
//  return 1;//插入
    return 0;//没有插卡
}


//------------------------------------------------------------------



void CutRPaper(short lines)
{//打印机切纸
#if DD_CUTTER
    if (CUTTER & 0x03)
    {
        RFeed(lines);
#if !defined(CASE_INNERDOTPRN)
        if (!TESTBIT(ApplVar.MyFlags, CLOSEPRINT))
            RSPrint(0, 0, PRN_CUTH_Mode);
#endif
    }
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////

#define TIMER_INTERVAL 	500

/**********************************************************************************************/

/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;


#if !defined(DEBUGBYPC)

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  */
void msDelay(__IO uint32_t nTime)
{
    ULONG   oldSysTime=GetSystemTimer();
    while ((GetSystemTimer()-oldSysTime)<nTime)
    {
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 us.
  * @retval None
  */
void usDelay(__IO uint32_t nTime)
{
    if (nTime)
    {
        TIM5->SR = (uint16_t)~TIM_FLAG_Update;
        TIM5->ARR = uSECOND * nTime ;
        TIM_Cmd(TIM5, ENABLE);
        while (!(TIM5->SR & TIM_FLAG_Update))
        {
        };
        TIM_Cmd(TIM5, DISABLE);

    }
}

/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/
/* This is a real time clock service to be called from     */
/* FatFs module. Any valid time must be returned even if   */
/* the system does not support a real time clock.          */
/* This is not required in read-only configuration.        */
DWORD get_fattime ()
{
    RTCTime rtc;

    /* Get local time */
    rtc_gettime(&rtc);

    /* Pack date and time into a DWORD variable */
    return((DWORD)(rtc.year - 1980) << 25)
    | ((DWORD)rtc.mon << 21)
    | ((DWORD)rtc.mday << 16)
    | ((DWORD)rtc.hour << 11)
    | ((DWORD)rtc.min << 5)
    | ((DWORD)rtc.sec >> 1);

}
/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
/* The RTC Clock may varies due to LSI frequency dispersion. */
    /* Enable the LSI OSC */
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;

#else
#error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Enable The TimeStamp */
	//commented by ouhs for testonly
    RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);
}
extern void USB_Init(void);
extern void USB_TEST(void);

void STM32_Initial(void)
{
    RTC_InitTypeDef RTC_InitStructure;
    RCC_ClocksTypeDef RCC_Clocks;

    /******** 启动文件中已调用SystemInit()设置主频为120MHz*********/


    //RCC_HCLKConfig(RCC_SYSCLK_Div8);

    //RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */

    // 滴答定时器SysTick每1ms中断一次：120000000/1000*(1/120)us=1ms
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);//1ms for SysTick


    UARTInit(1,115200,8,0,1); 	//对应外部串口RS232-I
    UARTInit(2,115200,8,0,1);	//对应外部串口RS232-II

#if USART_EXTEND_EN
#if defined(CASE_INNERDOTPRN)
    UARTInit(3,9600,8,0,1);	//对应内部串口(内置针打)
#else
	UARTInit(3,115200,8,0,1);	//对应内部串口(MFRICard)
#endif
#endif

	HW_GPIOInit();
    pwr_Init();

    FSMC_Init();
    BKPSRAM_Init();//初始化内部RAM ccr2014-03-25
#if !defined(CASE_ASCIIDISPLAY)
    mInitialLCD();
#endif

    /* 初始化RTC */
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
    {
        /* RTC configuration  */
        RTC_Config();

        /* Configure the RTC data register and RTC prescaler */
        RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
        RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
        RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;

        /* Check on RTC init */
        if (RTC_Init(&RTC_InitStructure) == ERROR)
        {
            xprintf("RTC Config Err.\n");
        }

        /* Configure the time register */
        //RTC_TimeRegulate();

		/* Indicator for the RTC configuration */
    	//RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
    }
    else
    {
        /* Check if the Power On Reset flag is set */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
            xprintf("Reset occurred.\n");
        }
        /* Check if the Pin Reset flag is set */
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
            xprintf("ExReset occurred\n");
        }

        //xprintf("No need to config RTC.\n");//No need to config

        /* Enable the PWR clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

        /* Allow access to RTC */
        PWR_BackupAccessCmd(ENABLE);

        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

        /* Clear the RTC Alarm Flag */
        RTC_ClearFlag(RTC_FLAG_ALRAF);

        /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
        EXTI_ClearITPendingBit(EXTI_Line17);


    }
    TIM5_Init();
    KeyBoardInit();

#if defined(FISCAL)
    FM_Init();
#endif

#if defined(CASE_ASCIIDISPLAY)
    LCDInit();
#else
	TIM6_Init();
	LED_Init(); 	// 注意： ER-220主板LED客显接口与JTAG(SW)调试接口复用,使用LED客显时将不能通JTAG(SW)下载程序或调试
#endif
	SD_LowLevel_Init();
	/**   NVIC PreemptionPriority优先级分配：SD=0, 打印机定时器TIM2和TIM3=1,
	                                        串口=2, 键盘定时器中断TIM7=3, 键盘按键中断=3,
											掉电中断=0, LED客显定时器TIM6=3,
	**/
    NVIC_SDConfiguration();// For SD

	Beep(1);
	USB_Start();

#if defined(CASE_GPRS)
	GPRS_PWR_HIGH();//GPRS_OnOff();
#endif
}

void Beep(uint8_t blen)
{
    BEEP_ON();
    if (blen == 1)
        beep_len = 10;
    else if (blen == 2)
        beep_len = 40;
    else if (blen == 3)
        beep_len = 80;
    else
        beep_len = 10;
}

//***************************************************//
void HW_GPIOInit(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

	// 开启GPIO外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
						   RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD |
						   RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

	// 配置GPIO：推挽输出、上拉、速度100MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	// 蜂鸣器
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	// UART0_RTS,UART1_RTS,UART2_RTS
	GPIO_InitStructure.GPIO_Pin = UART0_RTS;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#if (FORJLINK == 0)
	GPIO_InitStructure.GPIO_Pin = UART1_RTS;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#ifndef CASE_GPRS
	GPIO_InitStructure.GPIO_Pin = UART2_RTS;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#else	//GPRS模块开关机控制和复位
	GPIO_InitStructure.GPIO_Pin = GPRS_PWR | GPRS_RST;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPRS_PWR_LOW();
	GPRS_RST_LOW();
#endif


	// 钱箱
	GPIO_InitStructure.GPIO_Pin = DRAWER_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(DRAWER_PORT, &GPIO_InitStructure);
    DRAWER_OFF();

	// UART0_CTS, UART1_CTS, UART2_CTS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = UART0_CTS;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#if (FORJLINK == 0)
	GPIO_InitStructure.GPIO_Pin = UART1_CTS;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

#ifndef CASE_GPRS
	GPIO_InitStructure.GPIO_Pin = UART2_CTS;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	//连接射频刷卡器时，有卡存在时会检测到高电平
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif


	// MAC (Init)
	GPIO_InitStructure.GPIO_Pin = MAC_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MAC_PORT, &GPIO_InitStructure);

	BEEP_OFF();

}

#endif


/*********************************************************************************************************
** Function name:      TIM7_Init
** Descriptions:      定时器7初始化程序,10ms中断，用于蜂鸣器、钱箱、键盘
*********************************************************************************************************/
void TIM7_Init(void)
{
    uint16_t PrescalerValue = 0;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    /* Compute the prescaler value */
    // 定时器时钟为:60MHz/PrescalerValue  //1MHz
    PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1;

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 9999;    //10ms中断一次
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    /* Prescaler configuration */
    TIM_PrescalerConfig(TIM7, PrescalerValue, TIM_PSCReloadMode_Immediate);

    /* Enable the TIM gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //ouhs 20140814 =3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* TIM Interrupts enable */
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

    /* Enable counter */
    TIM_Cmd(TIM7, ENABLE);
}


/*********************************************************************************************************
** Function name:      TIM5_Init,用于延时等待
** Descriptions:      定时器5初始化程序,
*********************************************************************************************************/
void TIM5_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* Enable TIM5 clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    /* TIM5 configuration */

    TIM_TimeBaseStructure.TIM_Period = uSECOND * 5000*1000; //1ms
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIM5,DISABLE);// 不使用影子寄存器 //
    TIM_OCStructInit(&TIM_OCInitStructure);

    /* Output Compare Timing Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    TIM_OCInitStructure.TIM_Pulse = 0x0;
    TIM_OC1Init(TIM5, &TIM_OCInitStructure);

    /* One Pulse Mode selection */
    TIM_SelectOnePulseMode(TIM4, TIM_OPMode_Single);
    /* Clear TIM5 update pending flags */
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);

    /* TIM5 enable counters */
//  TIM_Cmd(TIM5, ENABLE);

}

/*********************************************************************************************************
** Function name:      TIM6_Init
** Descriptions:      定时器6初始化程序，1ms中断，用于LED客显，LCD光标
*********************************************************************************************************/
void TIM6_Init(void)
{
    uint16_t PrescalerValue = 0;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	/* Compute the prescaler value */
	// 定时器时钟为:60MHz/PrescalerValue  //1MHz
    PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1;
	/* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 499;//500us    = 999; //1ms中断一次

    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	/* Prescaler configuration */
    TIM_PrescalerConfig(TIM6, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Enable the TIM gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //ouhs 20140814 =2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* TIM Interrupts enable */
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	/* Enable counter */
	TIM_Cmd(TIM6, ENABLE);
}

#if (defined(DD_MMC) || defined(CASE_EJSD))
/*****************************************************************
  当SD卡的块大小为1024时，每次读写SD卡必须是1024字节
  但是，可以以512字节为块的边界
*****************************************************************/
//从SD卡上读取一块数据 ，块的的大小为SDCardInfo.CardBlockSize
BYTE MMC_ReadBlock(char *buf,DWORD addr)
{
	return (SD_ReadBlock(buf,addr, SDBLOCKSIZE)==SD_OK);
}
//向SD卡中写入一块数据，块的的大小为SDCardInfo.CardBlockSize
BYTE MMC_WriteBlock(char *buf,DWORD addr)
{
	return (SD_WriteBlock((BYTE *)buf, addr, SDBLOCKSIZE)==SD_OK);
}

//ccr2014-08-13>>>>>>>>>>>>>>>>
/*****************************************************************
  当SD卡的块大小为1024时，每次读写SD卡必须是1024字节
  但是，可以以512字节为块的边界
*****************************************************************/
//从SD卡上读取一块数据 ，块的的大小为SDCardInfo.CardBlockSize
BYTE MMC_Read2Blocks(char *buf,DWORD addr)
{
	return (SD_ReadMultiBlocks(buf,addr, SDBLOCKSIZE,2)==SD_OK);
}
//向SD卡中写入一块数据，块的的大小为SDCardInfo.CardBlockSize
BYTE MMC_Write2Blocks(char *buf,DWORD addr)
{
    return (SD_WriteMultiBlocks((BYTE *)buf, addr, SDBLOCKSIZE,2)==SD_OK);
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif

/**
 * =================================================================
 * 对内部BackupSRAM访问需要增加以下代码
 * MCU内部RAM用来存储下载的用户自定义要所图片
 */
#define BKPSRAM_SIZE 0x1000

/**Backup_SRAM访问初始化**/
/**
 *
 *
 * @author EutronSoftware (2014-03-25)
 */
void BKPSRAM_Init(void)
{
  //使能电源模块时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  //使能对备份域的访问
  PWR_BackupAccessCmd(ENABLE);
  //使能备份域SRAM的时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);

  //使能内部BackupSRAM电源
  PWR_BackupRegulatorCmd(ENABLE);

  //等待电源稳定
  while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
  {
  }
}
/**
 * 写BackupSRAM
 *
 * @author EutronSoftware (2014-03-25)
 *
 * @param pBuffer
 * @param WriteAddr:0~4095
 * @param NumHalfwordToWrite
 */
void BKPSRAM_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
    if (NumHalfwordToWrite)
        memcpy((uint8_t *)(WriteAddr+BKPSRAM_BASE),pBuffer,NumHalfwordToWrite);
}

/**
 * 读BackupSRAM
 *
 * @author EutronSoftware (2014-03-25)
 *
 * @param pBuffer
 * @param ReadAddr:0~4095
 * @param NumHalfwordToRead
 */
void BKPSRAM_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
    if (NumHalfwordToRead)
        memcpy(pBuffer,(uint8_t *)(ReadAddr+BKPSRAM_BASE), NumHalfwordToRead);
}
