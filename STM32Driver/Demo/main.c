/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main program body
  *****************************************************************************/

#include <stdint.h>

/* Includes ------------------------------------------------------------------*/
#include "king.h"                               /* Kingtron library and I/O routines */
#include "appl.h"
#include "include.h"
#include "spi_m25p80.h"
#include "monitor.h"
#include "test.h"
#include "spi_enc28j60.h"

extern APPLICATION_SAVE  ApplVar;

extern BYTE TESTPAPER(void);
extern void Test_SD(char mode);
extern void Test_LCD12864(void);

/* buffer size (in byte) for R/W operations */
unsigned char UserBuffer[MAX_BUFFER_SIZE] __attribute__ ((aligned (4)));    /*ccr20110829 Buffer used by application                             */

char ConsoleLine[128];				/* Console input buffer */


BYTE    KeyFrHost=0;       //ApplVar.Key code from host computer.
BYTE Bios_Key0_Key;			//    key number: 00 = No key


CONST FSTRING Msg[]={
    " ",                            // space
#if (DD_ZIP||DD_ZIP_21||DD_LCD_1601)
    "MessageM1",                         //    CWXXI01//    Ccr "��Ч����!",
    "MessageM2",                             //    CWXXI02//    Ccr "��Ч����!",
    "MessageM3",                             //    CWXXI03//    Ccr "��Чʱ��!",
    "MessageM4",                         //    CWXXI04//    Ccr "��ֹ����!",
    "MessageM5",                             //    CWXXI05//    Ccr "PLU����Ϊ��!",
    "MessageM6",                         //    CWXXI06//    Ccr "PLU�ļ����!",
    "MessageM7",                         //    CWXXI07//    Ccr "��̨��ռ��!",
    "MessageM8",                         //    CWXXI08//    Ccr "��̨�Ѵ�!",
    "MessageM9",                         //    CWXXI09//    Ccr "��̨�Ų���!",
    "MessageM10",                        //    CWXXI10//    Ccr "��������̨��",
    "MessageM11",                    //    CWXXI11//    Ccr "��̨û�п�̨",
    "MessageM12",                        //    CWXXI12//    Ccr "��̨�������",
    "MessageM13",                    //    CWXXI13//    Ccr "��ֹ�޸�ʱ��",
    "MessageM14",                        //    CWXXI14//    Ccr "����������!",
    "MessageM15",                    //    CWXXI15//    Ccr "���ۻ�������",
    "MessageM16",                    //    CWXXI16//    Ccr "��Ʒû���۳�",
    "MessageM17",                            //    CWXXI17//    Ccr "���ڽ�����!",
    "MessageM18",                        //    CWXXI18//    Ccr "�������ݳ���",
    "MessageM19",                        //    CWXXI19//    Ccr "���ڸ���!",
    "MessageM20",                            //    CWXXI20//    Ccr "�������!",
    "MessageM21",                            //    CWXXI21//    Ccr "û��ȷ�Ͻ���",
    "MessageM22",                        //    CWXXI22//    Ccr "��ֹȡ������",
    "MessageM23",                    //    CWXXI23//    Ccr "�޲���!",
    "MessageM24",                        //    CWXXI24//    Ccr "�޴��տ�Ա!",
    "MessageM25",                //    CWXXI25//    Ccr "�տ�Ա�ı�",
    "MessageM26",                        //    CWXXI26//    Ccr "�޴��౨��!",
    "MessageM27",                        //    CWXXI27//    Ccr "�����ӡ�ж�",
    "MessageM28",                        //    CWXXI28//    Ccr "�����ھ���",
    "MessageM29",                    //    CWXXI29//    Ccr "���ܷ���ʱ��",
    "MessageM30",                        //    CWXXI30//    Ccr "�����������",
    "MessageM31",                        //    CWXXI31//    Ccr "ת�뵽",
    "MessageM32",                        //    CWXXI32//    Ccr "δ��Ȩ!",
    "MessageM33",                        //    CWXXI33//    Ccr "��ָ���տ�Ա",
    "MessageM34",                        //    CWXXI34//    Ccr "��ָ��ӪҵԱ",
    "MessageM35",                        //    CWXXI35//    Ccr "��ֹPLU��ۣ�",
    "MessageM36",                        //    CWXXI36//    Ccr "���벻��!",
    "MessageM37",                        //    CWXXI37//    Ccr "������ӡ����",
    "MessageM38",                        //    CWXXI38//    Ccr "Ʊ�ݴ�ӡ����",
    "MessageM39",                        //    CWXXI39//    Ccr "��ӡѹ��̧��",
    "MessageM40",                        //    CWXXI40//    Ccr "��ӡֽ����!",
    "MessageM41",                            //    CWXXI41//    Ccr "��ӡ�¶�̫��",
    "MessageM42",                        //    CWXXI42//    Ccr "����δ����!",
    "MessageM43",                        //    CWXXI43//    Ccr "����������",
    "MessageM44",                            //    CWXXI44//    Ccr "��ֹ���ָ���",
    "MessageM45",                            //    CWXXI45//    Ccr "�����޴˹���",
    "MessageM46",                            //    CWXXI46//    Ccr "δ��С�Ƽ�!",
    "MessageM47",                            //    CWXXI47//    Ccr "���ڹ�����",
    "MessageM48",                        //    CWXXI48//    Ccr "������ˮ��",
    "MessageM49",                            //    CWXXI49//    Ccr "MODEMͨѶ��!",
    "MessageM50",                        //    CWXXI50//    Ccr "����������!",
    "MessageM51",                            //    CWXXI51//    Ccr "POS�����!",
    "MessageM52",                            //    CWXXI52//    Ccr "�����ݴ�!",
    "MessageM53",                            //    CWXXI53//    Ccr "Ϊ���ڿ�!",
    "MessageM54",                            //    CWXXI54//    Ccr "Ϊ��ʧ��!",
    "MessageM55",                            //    CWXXI55//    Ccr "�ͻ�����!",
    "MessageM56",                                    //    CWXXI56//    Ccr "Ϊ�¿�!",
    "MessageM57",                            //    CWXXI57//    Ccr "���ǹ��￨!",
    "MessageM58",                            //    CWXXI58//    Ccr "д������!",
    "MessageM59",                            //    CWXXI59//    Ccr "���Ų���!",
    "MessageM60",                        //    CWXXI60//    Ccr "�����ۿۿ�!",
    "MessageM61",                        //    CWXXI61//    Ccr "�����ֽ�!",
    "MessageM62",                        //    CWXXI62//    Ccr "�������ʿ�!",
    "MessageM63",                            //    CWXXI63//    Ccr "����IC��!",
    "MessageM64",                    //    CWXXI64//    Ccr "�忨����!",
    "MessageM65",                            //    CWXXI65//    Ccr "�������!",
    "MessageM66",                    //    CWXXI66//    Ccr "IC��ֵ����!",
    "MessageM67",                    //    CWXXI67//    Ccr "IC��ʼ������",
    "MessageM68",                        //    CWXXI68//    Ccr "��ֹ��ʼ��!",
    "MessageM69",                    //    CWXXI69//    Ccr "IC����!",
    "MessageM70",                    //    CWXXI70//    Ccr "IC�������!",
    "MessageM71",                            //    CWXXT71//    Hf "IP��ַ��",
    "MessageM72",                //    CWXXI72//     ccr "��Ƶ������!",
    "MessageM73",                //    CWXXI73//    Cc "����ʧ��!",
    "MessageM74",                //    CWXXI74>>>>>for fiscal
    "MessageM75",                //    CWXXI75
    "MessageM76",                //    CWXXI76
    "MessageM77",                    //    CWXXI77 FM���� //
    "MessageM78",                    //    CWXXI78
    "MessageM79",                    //    CWXXI79
    "MessageM80",                //    CWXXI80
    "MessageM81",                //    CWXXI81
    "MessageM82",                //    CWXXI82 EJ�е������д� //
    "MessageM83",                        //    CWXXI83
    "MessageM84",                //    CWXXI84 EJ ��д���� //
    "MessageM85",                //    CWXXI85 EJ������ //
    "MessageM86",                //    CWXXI86
    "MessageM87",                //    CWXXI87 EJ���� //
    "MessageM88",                //    CWXXI88
    "MessageM89",                    //    CWXXI89
    "MessageM90",                        //    CWXXI90
    "MessageM91",                    //    CWXXI91 ���200�μӵ��ʼ�� //
    "MessageM92",                    //    CWXXI92
    "MessageM93",                        //    CWXXI93 FM ���տ����ƥ�� //
    "MessageM94",                            //    CWXXI94
    "MessageM95",                    //    CWXXI95
    "MessageM96",                    //    CWXXI96
    "MessageM97",                    //    CWXXI97
    "MessageM98",                        //    CWXXI98 EJ���տ����ƥ�� //
    "MessageM99",                        //    CWXXI99
    "MessageM100",                       //    CWXXI100
#endif	//(DD_ZIP||DD_ZIP_21)
    "MessageN1",             //    KAHAO//    Ccr 	"����:",				//    KAHAO
    "MessageN2",             //    KLXING//    Ccr 	"������:",				//    KLXING
    "MessageN3",         //    KNJE//    Ccr 	"���ڽ��:",			//    KNJE
    "MessageN4",             //    KYJIN//    Ccr 	"��Ѻ��:",				//    KYJIN
    "MessageN5",         //    XFZE//    Ccr 	"�����ܶ�:",			//    XFZE
    "MessageN6",         //    CHZHZE//    Ccr 	"��ֵ�ܶ�:",			//    CHZHZE
    "MessageN7",         //    SHYCSHU//    Ccr 	"ʹ�ô���:",			//    SHYCSHU
    "MessageN8",         //    JGLBIE//    Ccr 	"�۸񼶱�:",			//    JGLBIE
    "MessageN9",             //    PINMA//    Ccr 	"PIN��:",				//    PINMA
    "MessageN10",            //    BHJBIE//    Ccr 	"��������:",			//    BHJBIE
    "MessageN11",            //    ZDJZHANG//    Ccr 	"�Զ�����:",			//    ZDJZHANG
    "MessageN12",            //    ZHKRQI//    Ccr 	"�ƿ�����:",			//    ZHKRQI
    "MessageN13",            //    KYXQI//    Ccr 	"����Ч��:",			//    KYXQI
    "MessageN14",            //    KHMCHEN//    Ccr 	"�ͻ�����:",			//    KHMCHEN
    "MessageN15",                //    CHSHHICKA//    ccr	"��ʼ��IC",								//    CHSHHICKA
    "MessageN16",                        //    ICKCHZHI//    ccr	"IC����ֵ",									//    ICKCHZHI
    "MessageN17",                        //    QCHICKA//    ccr	"���IC��",									//    QCHICKA
    "MessageN18",  //    GUASHIIC//    ccr "��ʧ����",     //    GUASHIIC      //    ICCard Part ccr chipcard 2004-06-28
    "MessageN19", //    ZHEKOUCA//    ccr "�ۿۿ�:",            //    ZHEKOUCA
    "MessageN20",        //    XIANJINKA//    ccr "�ֽ�:",           //    XIANJINKA
    "MessageN21",  //    SHEZHANGKA//    ccr "���ʿ�:",           //    SHEZHANGKA
    "MessageN22",//    XFJIDIAN//    ccr "���ѼƵ�:",           //    XFJIDIAN
    "MessageN23",        //    ZHKLVF//    Ccr "�ۿ���(%):",        //    ZHKLVF
    "MessageN24",            //    ICKTKUAN//    Ccr "IC���˿�",			//    ICKTKUAN
    "MessageN25",            //    ICKDJDIANG//    Ccr "IC���ҽ�����",			//    ICKDJDIANG
    "MessageN26",            //    ICKSDIAN//    Ccr "IC���͵���",			//    ICKSDIAN
    "MessageN27",                 //    ZHUOTAI//    Ccr "��̨",                       //    ZHUOTAI
    "MessageN28",            //    Cc	"��ʼ�վݺ�",	//    RECNUMFR
    "MessageN29",            //    Cc	"�����վݺ�",	//    RECNUMTO
    "MessageN30",        //    hf		"EJ��ʼ��"		//    EJCSHUA
    "MessageN31",                        //    hf 	"EJ���"			//    EJBHAO
    "MessageN32",        //    hf		"EJ ����"			//    EJBBIAO
    "MessageN33",        //    hf 	"~��~��~��",		//    HSJXSHOU
#if (FISCAL ==1 || DD_FISPRINTER==1)
    "MessageN35",        //Cc	"˰��A˰��",		//VATA
    "MessageN36",        //Cc	"˰��B˰��",		//VATB
    "MessageN37",        //Cc	"˰��C˰��",		//VATC
    "MessageN38",        //Cc	"˰��D˰��",		//VATD
    "MessageN39",        //Cc	"˰��E˰��",		//VATE
    "MessageN40",        //Cc	"˰��F˰��",		//VATF
    "MessageN41",        //Cc	"˰��G˰��",		//VATG
    "MessageN42",        //Cc	"˰��H˰��",		//VATH
    "MessageN43",        //Cc	"˰���ܼ�",		//TAXTOTAL
    "MessageN44",    //Cc	"������",			//NETSALE
    "MessageN45",    //Cc	"���ױ���",		//RECNUMBER
    "MessageN46",        //FMPULLOUT  ccr071212
    "MessageN47",    //Cc	"����Ʊͷ",		//CHANGEHEAD
    "MessageN48",    //Cc	"����˰��A",		//TAXACHG
    "MessageN49",    //Cc	"����˰��B",		//TAXBCHG
    "MessageN50",    //Cc	"����˰��C",		//TAXCCHG
    "MessageN51",    //Cc	"����˰��D",		//TAXDCHG
    "MessageN52",    //Cc	"����˰��E",		//TAXECHG
    "MessageN53",    //Cc	"����˰��F",		//TAXFCHG
    "MessageN54",    //Cc	"����˰��G",		//TAXGCHG
    "MessageN55",    //Cc	"����˰��H",		//TAXHCHG
    "MessageN56",            //Cc	"��ʼ����",		//TAXFROM
    "MessageN57",                //Cc	"��ֹ����",		//TAXTO
    "MessageN58",    //Cc"˰�ش洢������",//PRNFISCAL
    "MessageN59",    //Cc	"�����ܼ�",		//PRNTOTAL
    "MessageN60",        //Cc	"�����ܼ�",		//CLEARTOTAL
    "MessageN61",        //Cc	"��λ�ܼ�",		//MACRESET
    "MessageN62",        //Cc"˰�ʸ����ܼ�",	//CHANGETAX
    "MessageN63",    //hf 	"˰���ձ���",	//SHKZBB
    "MessageN64",        //hf 	"˰����",			//TAXCODE
    "MessageN65",        //hf 	"RIF����",			//RIFCODE
    "MessageN67",    //hf		"��˰����",		//MIANSHUI
    "MessageN68",            //hf 	"˰Ŀ A",			//SHUIMUA
    "MessageN69",        //hf		"�ͻ�����",		//KHMINGCH		//20070308
    "MessageN70",            //hf		"������",			//JIQIHAO		//20070308
    "MessageN71",                        //hf		"����",			//RIQI			//20070308
    "MessageN72",                        //hf		"ʱ��",			//SHIJIAN		//20070308
    "MessageN73",                //hf		"�����ܼ�",		//XSHZHJI
    "MessageN74",        //hf		"�˻��ܼ�",		//THUOZHJI
    "MessageN75",        //hf		"�ۿ��ܼ�",		//ZHKOUZHJI
    "MessageN76",                    //20070313	" SALE    ",!!!!�µ����Ƶĳ��Ȳ�Ҫ����TAXABLE//hf 	"��ͨ����",		//XIAOSHOUA
    "MessageN77",                //hf		" �˻�����",		//THXIAOSHOU
    "MessageN78",                //20070313	" DISC   ",//hf		"�ۿ�����",		//ZHKOUXSH
    "MessageN79",        //20070313 " REFUND TAX SUM",//hf		"�˻�˰��"		//THZJSHUI
    "MessageN80",        //20070313 " DISC TAX SUM",//hf		"�ۿ�˰��"		//ZHKZJSHUI
    "MessageN81",        //    hf 	"EJ��Ϣ",			//    EJXINXI
    "MessageN82",            //    hf "EJ��ʼ��ʱ��",		//    EJSHJIAN
    "MessageN83",            //    Cc	"�Ƿ�Ʊ",			//    NONFISCAL
    "MessageN84",        //    liuj 0531 PM
    "MessageN85",            //    Cc	"��ʼ����",		//    RIQIF
    "MessageN86",            //    Cc	"��ʼʱ��",		//    SHIJIANF
    "MessageN87",                //    Cc	"��������",		//    JSHRQIF
    "MessageN88",            //    hf		"�ܼƷ�Ʊ��",	//    ZHJIFPHAO
    "MessageN89",                //    hf		"�ܼ��˻���",	//    ZHJITHHAO
    "MessageN90",                //    hf		"�ܼƷ�˰��",	//    ZHJIFSHHAO
    "MessageN91",            //    hf		"��Ʊ��",			//    FAPIAOHAO
    "MessageN92",                //    hf		"�˻���",			//    THUOHAO
    "MessageN93",                //    hf		"��˰��",			//    FEISHUIHAO
    "MessageN94",                //    hf		"˰�ر����",	//    SHKBBHAO
    "MessageN95",                    //    Cc	"���ʼ���"		//    QZHJSHU
    "MessageN96",                    //    REPLEFT
    "MessageN97",                            //SIZEFM
#endif//FISCAL
    "MessageN98 ",                            //SIZEEJ
    "MessageN99 ",            //    VALUEINIC
    "MessageN100",                               //    LOWBAT
    "MessageN101",               //    CLEARIC
    "MessageN102",           //    CHARGEIC
    "MessageN103",           //    ADPOINTS
    "MessageN104",               //    INITIC
    "MessageN105",           //WAITEICCARD
    "MessageN106",           //ICCARDOK
    "MessageN107",           //ePOSTAC //ccr epos//ccr "ePOS��֤��:",			//ePOSTAC //ccr epos
    "MessageN108",                   //WAITEPOS
    "MessageN109",           //ePOSCARNO//ccr "��ͨ����:",					//ePOSCARNO
    "MessageN110",   //eCARREMAIN//ccr "��ͨ�����:",					//eCARREMAIN
    "MessageN111",           //TMHYHAO//ccr "��Ա��:",			//TMHYHAO
    "MessageN112",   //BARREPORT//ccr  �� Ա �� ��   //
    "MessageN113",               //XFJE//ccr "���ѽ��",				//XFJE
    "MessageN114",                 //ZONGJI//Ccr "�ܼ�",                       //ZONGJI
    "MessageN115",                       //    BREAK485
    "MessageN116",                       //    LINK485
    "MessageWait",                       // WAITING
    "",
};
//extern int SDMMC_main(void);
//extern int FLASH_main(void);

#define TIMER_INTERVAL 	500

/**********************************************************************************************/

/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */

extern __IO uint32_t AsynchPrediv, SynchPrediv;


char cSRAMNOTFOUND[]="SRAM Not Found!";


/*--------------------------------------------------------------------------*/
/* Monitor                                                                  */
/*--------------------------------------------------------------------------*/
void Copy_LCD(BYTE ctrl)
{

}

static void ApplicationInit(void)
{


}

//==================================================================
//Test if has some key for read
//Return True if a key pressed
short   KbHit()
{
    WORD i;
#if (defined(DEBUGBYPC))
       ProcessMessages();
#elif (CASE_USBDISK)
       USB_Detect();//ѭ�����USB�豸
#endif


        i =Bios_1(BiosCmd_CheckKeyborad);//����Ƿ��а���

#if (PC_EMUKEY)
        if (i && (i==fTEST+1) && ApplVar.CentralLock==RG)
        {
            if (FisTestTask.Going)//�ڶ��ΰ����Լ�ʱ���رմ�ӡ��
            {
                if (FisTestTask.PrnOFF == 0)
                    FisTestTask.PrnOFF = 1;
                else
                    FisTestTask.Going = FisTestTask.PrnOFF = 0;//�رպ������Լ�ʱ���򿪴�ӡ��
                Getch();
                return 0;
            }
            else if (Appl_EntryCounter==0)
                {
                    FisTestTask.Going = 0x01;
                    Getch();
                    return 0;
                }
        }
#endif
        if (i==0)
        {//�ް���
            if (KeyFrHost!=0xff)
            {
                SETBIT(ApplVar.MyFlags, KEYFROMHOST);
                i = Bios_Key0_Key = KeyFrHost+1;
                KeyFrHost = 0xff;
#if (defined(DEBUGBYPC) && PC_EMUKEY)
                if ((i==fTEST+1) && ApplVar.CentralLock==RG)
                {
                    if (FisTestTask.Going)//�ڶ��ΰ����Լ�ʱ���رմ�ӡ��
                    {
                        if (FisTestTask.PrnOFF == 0)
                            FisTestTask.PrnOFF = 1;
                        else
                            FisTestTask.Going = FisTestTask.PrnOFF = 0;//�رպ������Լ�ʱ���򿪴�ӡ��
                        Getch();
                        return 0;
                    }
                    else if (Appl_EntryCounter==0)
                        {
                            FisTestTask.Going = 0x01;
                            Getch();
                            return 0;
                        }
                }
#endif
            }
        }
        return i;
}

//ccr20110824>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*-----------------------------------------------------------------------*/
/* CCR 20110824                                                         */
/* Display help messages                                                */
/*-----------------------------------------------------------------------*/
void DisplyMainHelp()
{
    xputs(" ? - Help me \n");
    xputs(" b <UARTx> <baudrate>- Set baudrate of UARTx(0,1)\n");
    xputs("           <baudrate>:(0-1200,1-2400,2-4800,3-9600,4-19200,5-38400,6-57600,7-115200)\n");
    xputs(" d <string>- USB Disk TEST.\n");
    xputs(" u - USB Disk TEST.\n");
	xputs(" m - External SRAM TEST.\n");
	xputs(" M - Backup SRAM TEST.\n");
    xputs(" s - SD,MMC Card Single block Test.\n");
    xputs(" S - SD,MMC Card Multi block Test.\n");
    xputs(" f - Serial flash Test.\n");
    xputs(" g [Gn Gt Gp Gl] - Print Graphic Gn at Gp;\n");
    xputs("                   Gt=0/1/2 for text attached.\n");
    xputs("                   Gl= Lines of text attached.\n");
    xputs(" p <string>- Print test.\n");
    xputs(" P - Print test by POS command.\n");
    xputs(" t [yy mm dd hh mm ss]- set Date & Time \n");
#if defined(CASE_ETHERNET)
    xputs(" n - Network Test \n");
#endif
}
/*------------------------------------------------------------------------
  Main menual
-------------------------------------------------------------------------*/
const uint32_t Baudrate[8]={1200,2400,4800,9600,19200,38400,57600,115200};
int main ()
{
    RTCTime rtc;
    char *ptr;
    long p1,p2,p3,p4;
    char c1,c2;
    int i;
    char dispBuf[30];
    BYTE secCurrent;

    xfunc_out = UART_PutChar;
    xfunc_in = UART_GetChar_nb;

    ApplicationInit();
    STM32_Initial();
	//p1=0x12345678;
	//CLONG(dispBuf[0])=p1;

	//p1=0xabcdef39;
	//CLONG(dispBuf[4])=p1;

#if EN_PRINT
    Print_Initial();

    ApplVar.PoolPushIn=0;
    ApplVar.PoolPopOut=0;
    ApplVar.Prn_Command=0;
#endif

#if !defined(CASE_ASCIIDISPLAY)
     Test_LCD12864();
#endif

#if defined(CASE_ETHERNET)
	Ethernet_Init();
#endif
    DisplyMainHelp();

    xputc('>');
    for (;;)
    {
        if (KbHit())
        {
            c1 = Getch();
            sprintf(dispBuf,"KbHit=%d",c1);
            DispStrLF(dispBuf);
        }
        i = get_line(ConsoleLine, sizeof(ConsoleLine));
        if (i)
        {
            ptr = ConsoleLine;
            switch (*ptr++)
            {
    #if defined(CASE_ETHERNET)
            case 'n':
                Ethernet_Test();
                break;
    #endif
            case '?':
                DisplyMainHelp();
                break;
            case 'b':
                while (*ptr == ' ') ptr++;
                c1 = *ptr++;
                if (c1==0 || c1>'1')
                {
                    xputs("UARTx needed for setup.\n");
                    break;
                }
                while (*ptr == ' ') ptr++;
                c2 = *ptr;
                if (c2==0 || c2>'7')
                {
                    xputs("Baudrate needed for setup.\n");
                    break;
                }
                c1 &= 0x0f;
                c2 &= 0x0f;
                xprintf("Set UART%d to %d\n",c1,Baudrate[c2]);

                UARTInit(c1,Baudrate[c2],8,0,1);

                break;
            case 'u':
                //USB_main();
                break;
            case 'f':
                if(FMDetect()==SUCCESS)
                    Test_SPIFlash();
                break;
            case 'm':
                TIM6_Init_Count();//���ڲ��Զ�дʱ��
                Test_SRAM();
                break;
            case 'M':
                TIM6_Init_Count();//���ڲ��Զ�дʱ��
                Test_BKPSRAM();
                break;
            case 's':
                Test_SD('s');
                break;
            case 'S':
                Test_SD('S');
                break;
            case 'd':
                i--;
                while (*ptr == ' ')
                {
                    ptr++;
                    i--;
                }
                if (*ptr)
                {
    //                DelayForLCD_1us(5000 * 1000);
                    xputs("Time out.\n");
                    //PutsO(ptr);
                }
                break;
    #if EN_PRINT
            case 'P':
                if (!TESTPAPER())
                {

                    xputs("Paper out.\n");
                    //break;
                }
                Print_ESC_Command();
                break;
            case 'p':
                if (!TESTPAPER())
                {
                    xputs("Paper out.\n");
                    //break;
                }
                i--;
                while (*ptr == ' ')
                {
                    ptr++;
                    i--;
                }
                if (*ptr)
                {
                    if (*ptr>='0' && *ptr<='9')
                    {
                        xatoi(&ptr, &p1);

                        while (p1)
                        {
                            p2 = PrnTemperature();
                            xprintf("ADC1.CH10= %d\n",p2);
                            p1--;
                            for (p2=0;p2<1000000;p2++);
                        }
                    }
                    else
                        while (RSPrint(ptr,i,PRN_RECEIPT_Mode)<0);
                }
                else
                    xputs("String needed for print.\n");
                break;
            case 'g':
                if (!TESTPAPER())
                {
                    xputs("Paper out.\n");
                    //break;
                }
                i--;
                while (*ptr == ' ')
                {
                    ptr++;
                    i--;
                }
                if (*ptr && *ptr>='0' && *ptr<='9')
                {
                    p1=p2=p3=p4=0;
                    xatoi(&ptr, &p1);
                    xatoi(&ptr, &p2);
                    xatoi(&ptr, &p3);
                    xatoi(&ptr, &p4);
                    if (p1)
                    {
                        ConsoleLine[0]=p1;//ͼƬ���
                        ConsoleLine[1]=(p2 & 3)+'0';//�Ƿ��������
                        ConsoleLine[2]=p3;//ͼƬ��ӡλ��
                        while (RSPrint(ConsoleLine,3,PRN_GRAPHIC_Mode)<0);
                        if (p2==1)
                        {
                            for (i=0;i<p4;i++)
                            {
                                xsprintf(ConsoleLine,"Attach line %d",i);
                                while (RSPrint(ConsoleLine,strlen(ConsoleLine),PRN_RECEIPT_Mode)<0);
                            }
                            ConsoleLine[0]=0;//ͼƬ�������ַ���������
                            while (RSPrint(ConsoleLine,1,PRN_RECEIPT_Mode)<0);
                        }
                        for (i=0;i<8;i++)
                        {//��ӡ��������
                            xsprintf(ConsoleLine,"Printer follow line %d",i);
                            while (RSPrint(ConsoleLine,strlen(ConsoleLine),PRN_RECEIPT_Mode)<0);
                        }
                    }
                }
                else
                {
                    for (i=0;i<GRAPHICMAX;i++)
                    {
                        ConsoleLine[0]=i+1;
                        ConsoleLine[1]='0';
                        while (RSPrint(ConsoleLine,2,PRN_GRAPHIC_Mode)<0);
                    }
                }
                break;
    #endif
            case 't' :  /* t [<year> <mon> <mday> <hour> <min> <sec>] */
                if (xatoi(&ptr, &p1))
                {
                    rtc.year = (WORD)p1;
                    xatoi(&ptr, &p1); rtc.mon = (BYTE)p1;
                    xatoi(&ptr, &p1); rtc.mday = (BYTE)p1;
                    xatoi(&ptr, &p1); rtc.hour = (BYTE)p1;
                    xatoi(&ptr, &p1); rtc.min = (BYTE)p1;
                    if (!xatoi(&ptr, &p1)) p1=0;
                    rtc.sec = (BYTE)p1;
                    rtc_settime(&rtc);
                }
                rtc_gettime(&rtc);
                xprintf("%u/%u/%u %02u:%02u:%02u\n", rtc.year, rtc.mon, rtc.mday, rtc.hour, rtc.min, rtc.sec);
                secCurrent = rtc.sec;
                while (!CheckKeyboard() && !UARTGet(PORT_DEBUG,0,0))
                {
                    rtc_gettime(&rtc);
                    if (secCurrent!=rtc.sec)
                    {
                        secCurrent = rtc.sec;
                        sprintf(dispBuf," %02u:%02u:%02u", rtc.hour, rtc.min, rtc.sec);
                        //PutsO(dispBuf);
                    }
                }
                break;

    #if !defined(CASE_ASCIIDISPLAY)
            case 'L':
                Test_LCD12864();
                break;
    #endif
            }//switch
            xputc('>');
        }
    }//for
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

