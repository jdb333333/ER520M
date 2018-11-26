
/* Includes ------------------------------------------------------------------*/
#include "king.h"                               /* Kingtron library and I/O routines */
#include "appl.h"
#include "include.h"
#include "monitor.h"
#include "nor_flash.h"
#include "test.h"
#include "spi_m25p80.h"

#if defined(CASE_ETHERNET)
#include "netconfig.h"
#include "enc28j60.h"
#include "cmd.h"
#include "httpd.h"
#include "tcp_echoserver.h"
#include "udp_echoserver.h"
#endif

#define  BLOCK_SIZE             512

//*******************************SRAM_DEBUG**************************************//
#ifdef 	SRAM_DEBUG

#define BUFFER_SIZE        10240
#define WRITE_READ_ADDR    (0x80000-0x1fff)		 // SRAM:0~0x80000
#define MAX_ADDR    (FiscalMem_SPI_MAXADDR+1)
unsigned long RamOffSet;
char    SysBuf[128];            /*    System buffer max 128 positions     */
APPLICATION_SAVE    ApplVar;


/**************************************************************************/
/**************************************************************************/
BYTE ApplVar_Restored=0x00;   //=0x5a,ApplVar中的变量数据被恢复

void Save_ApplRam()
{}

/**************************************************************************/
/**************************************************************************/


uint8_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

void Save_ConfigVar(void) //保护参数
{
    xputs("Power off!");
}

extern void ReadRam( char *buf,DWORD len);
extern void WriteRam( char *buf, DWORD len);

void Ethernet_Test(void);


void TIM6_Init_Count(void)
{//by ouhs test 用于测试程序执行时间
    uint16_t PrescalerValue = 0;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	/* Compute the prescaler value */
	// 定时器时钟为:60MHz/PrescalerValue  //1MHz
    PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 100000) - 1;   //10us
	/* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 9999; //100ms一次

    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	/* Prescaler configuration */
    TIM_PrescalerConfig(TIM6, PrescalerValue, TIM_PSCReloadMode_Immediate);

	/* Enable counter */
	//TIM_Cmd(TIM6, ENABLE);
}

uint8_t Test_SRAM(void)
{
	DWORD i;
	BYTE twr_data=0xA5,trd_data;
#if 1 //测试程序执行时间需要先设置TIM6
	/* Write data to FSMC SRAM memory */
	/* Fill the buffer to send */
	Fill_Buffer(TxBuffer, BUFFER_SIZE, 0xf321);	 // 0xf321

	RamOffSet = 0;
	//测试SRAM写入速度,10KB数据
	TIM_Cmd(TIM6, ENABLE);
	WriteRam(TxBuffer, BUFFER_SIZE);
	i = TIM_GetCounter(TIM6)*10;	//us
	USART_printf("\r\n 10KB write time:%dus\r\n",i);
	TIM_Cmd(TIM6, DISABLE);
	//测试SRAM写入速度

	RamOffSet = 0;
	/* Read data from FSMC SRAM memory */
	ReadRam(RxBuffer, BUFFER_SIZE);

	/* Read back SRAM memory and check content correctness */
	Buffercmp(TxBuffer, RxBuffer, BUFFER_SIZE);
#else
	USART_printf("\r\nTest_WR_RD_SRAM...");
	for (i=0; i<SRAMSIZE; i++)
	{
		SRAM_WriteBuffer(&twr_data, i, 1);
		SRAM_ReadBuffer(&trd_data, i, 1);
		if(trd_data != 0xA5)
		{
			USART_printf("\r\n SRAM_ERROR:twr_data=0x%x---trd_data=0x%x \r\n",twr_data,trd_data);
			return 0;
		}
	}

	USART_printf("\r\n SRAM Write & Read OK \r\n");
#endif

	return 1;

}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  BufferLenght: size of the buffer to fill
  * @param  Offset: first value to fill on the buffer
  * @retval None
  */
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  /* Put in global buffer different values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}
//return:0 or
//       >0 error at returen-1
uint32_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
	__IO uint32_t WriteReadStatus = 0;
	uint32_t Index;

	for (Index = 0x00; (Index < BufferLength) && (WriteReadStatus == 0); Index++)
	{
		if (pBuffer1[Index] != pBuffer2[Index])
		{
			WriteReadStatus++;
            USART_printf("\r\n ERROR:TxBuffer[%d]=0x%x---RxBuffer[%d]=0x%x \r\n",Index,pBuffer1[Index],Index, pBuffer2[Index]);
            return Index+1;
		}
	}

	if (WriteReadStatus == 0)
	{
		/* OK */
		//USART_printf("\r\n Write & Read OK:TxBuffer[%d]=0x%x---RxBuffer[%d]=0x%x \r\n",BufferLength-1,pBuffer1[BufferLength-1],BufferLength-1, pBuffer2[BufferLength-1]);
        USART_printf("\r\n SRAM Write & Read OK \r\n");
        return 0;
	}

	return 0;
}


#endif

//*******************************SPIFLASH_DEBUG**********************************//
#ifdef SPIFLASH_DEBUG


/************************** PRIVATE DEFINITIONS *********************/
#define TEST_SIZE 64


uint32_t    ChipID = 0;


/*-------------------------MAIN FUNCTION------------------------------*/
/* CCR 20110824                                                         */
/* Display help messages                                                */
/*-----------------------------------------------------------------------*/
void DisplyFlashHelp()
{
    xprintf("\nTest serial flash SST25VF016B(SIZE:%d Bytes). \n",MAX_ADDR);
    xputs(" ? - Help for Flash. \n");
    xputs(" q - Quit.\n");
    xputs(" fi - Initialize SPI for Flash.\n");
    xprintf(" fw <addr:%d> <string:120> - Auto-Erase & then write the string. \n",MAX_ADDR-1);
    xprintf(" fc <addr:%d> <string:120> - Check the data of write by fW. \n",MAX_ADDR-1);
    xprintf(" fr <addr:%d> <len> - read data from Flash.\n",MAX_ADDR-1);
    xprintf(" fe <secF:%d> <secT> - Erase sections  from secF to secT. \n",FiscalMem_SEC_MAX);
    xputs(" fE - Erase chip.\n");

    xprintf(" fW <addr:%d> <len> <val> - write data to Flash. \n",MAX_ADDR-1);
    xprintf(" fC <addr:%d> <len> <val> - Check the data of write by fw. \n",MAX_ADDR-1);
    xputs(" fl - Load Chinese into flash.\n");
//    xputs(" ff - Send Flash data to host.\n");

}


/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
void Test_SPIFlash(void)
{
    uint8_t rc;
    char *ptr;
    uint32_t  numMax, blkSize,p1,p2,p3,i,timeRD;

    DisplyFlashHelp();

    for (;;)
    {
        xputs("FLASH>");
        if (get_line(ptr, sizeof(ConsoleLine)))
        {
            ptr = ConsoleLine;
            switch (*ptr++)
            {
            case '?':
                DisplyFlashHelp();
                break;
            case 'q':
                return ;

            case 'f' :
                switch (*ptr++)
                {
                case 'i' :  /* fi  - Initialize USB HOST device */

                    ChipID = FiscalMem_ReadID();

                    xprintf("Manufactory_ID is:%x\n",ChipID);

                    break;


                case 'r' :  /* fr <addr> <len> - read file */
                    if (!xatoi(&ptr, &p1) || p1>=MAX_ADDR)
                    {
                        xprintf("Input the address  for read.\n");
                        break;
                    }
                    if (!xatoi(&ptr, &p2) || (p1+p2)>MAX_ADDR)
                    {
                        xprintf("Input the length for read.\n");
                        break;
                    }
                    numMax = blkSize = p3 = p2;
                    if (blkSize>MAX_BUFFER_SIZE)
                    {
                        blkSize = MAX_BUFFER_SIZE;
                    }
                    timeRD = 0;
                    while (p2 >0 )
                    {
                        SetTimerDelay0(0);

                        if (FiscalMem_ReadBuffer(UserBuffer,p1, blkSize)==ERROR)
                        {
                            xprintf("Error at 0x%x when read.\n",p1);
                            break;
                        }
                        timeRD+= GetTimerDelay0();
                        for (i=0;i<blkSize;i++)
                        {
                            xputc(UserBuffer[i]);
                        }
                        p1 += blkSize;//Totals of the read
                        p2 -= blkSize;
                        p3 -= blkSize;

                        if (blkSize>p3)
                        {
                            blkSize = p3;
                        }
                     }
                     xprintf("\n<---%lu bytes read in  %lu m.seconds.\n", numMax, timeRD );
                    break;

                case 'W' :  /* fw <addr> <len> <val> - write file */
                    if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)
                        || p1>=MAX_ADDR ||(p1+p2)>MAX_ADDR
                        )
                    {
                        xprintf("Input the address,length , val for write.\n");
                        break;
                    }
                    xmemset(UserBuffer, p3, MAX_BUFFER_SIZE);

                    numMax = blkSize = p3 = p2;

                    if (blkSize>MAX_BUFFER_SIZE)
                    {
                        blkSize = MAX_BUFFER_SIZE;
                    }
                    p2 += p1;
                    timeRD = 0;
                    while (p1<p2)
                    {
                        SetTimerDelay0(0);
                        if (FiscalMem_WriteBuffer(UserBuffer, p1, blkSize)==ERROR)
                            break;
                        timeRD+=GetTimerDelay0();
                        p1+=blkSize;
                        p3-=blkSize;
                        if (blkSize>p3)
                        {
                            blkSize = p3;
                        }
                        xputc('.');
                    }
                    xprintf("\n<---%lu bytes written in %lu m.seconds.\n", numMax, timeRD );
                    break;

                case 'C' :  /* fc <addr> <len> <val> - Check the data of write by fw */
                    if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || !xatoi(&ptr, &p3)
                        || p1>=MAX_ADDR ||(p1+p2)>MAX_ADDR
                        )
                    {
                        xprintf("Input the address,length , val for check.\n");
                        break;
                    }
                    xmemset(UserBuffer, 0, MAX_BUFFER_SIZE);
                    rc = p3;
                    SetTimerDelay0(0);

                    numMax = blkSize = p3 = p2;

                    if (blkSize>MAX_BUFFER_SIZE)
                    {
                        blkSize = MAX_BUFFER_SIZE;
                    }
                    p2 += p1;
                    while (p1 < p2 )
                    {

                        if (FiscalMem_ReadBuffer(UserBuffer,p1, blkSize)==ERROR)
                        {
                            xprintf("Error at 0x%x when read.\n",p1);
                            break;
                        }
                        for (i=0;i<blkSize;i++)
                        {
                            if (UserBuffer[i]!=rc)
                                break;
                        }
                        if (i<blkSize)
                        {
                            xprintf("Error write at 0x%x .\n",p1+i);
                            break;
                        }
                        p1 += blkSize;//Totals of the read
                        p3 -= blkSize;

                        if (blkSize>p3)
                        {
                            blkSize = p3;
                        }
                    }
                    break;
                case 'w' :  /* fW <addr> <string> - write a string to flash,erase the section auto*/
                    if (!xatoi(&ptr, &p1) || p1>=MAX_ADDR)//get the address for write
                    {
                        xprintf("Input the address,string for write.\n");
                        break;
                    }
                    if (*ptr==' ') ptr++;

                    for (p2=0;*ptr;p2++)//get the string for write
                    {
                        UserBuffer[p2] = *ptr++	;
                    }
                    if ((p1+p2)>=MAX_ADDR)//out of range
                    {
                        p2 = MAX_ADDR-p1;
                    }
                    SetTimerDelay0(0);
                    timeRD = 0;
                    if (1)//(WriteBlock(p1,UserBuffer,p2)==ERROR)
                        xputs("Error found for fw command.\n");
                    else
                    {
                        timeRD+=GetTimerDelay0();
                        xprintf("\n<---%lu bytes written in %lu m.seconds.\n", p2, timeRD );
                    }
                    break;
                case 'c' :  /* fC <addr> <string> - check the write by fW */
                    if (!xatoi(&ptr, &p1) || p1>=MAX_ADDR)//get the address for check
                    {
                        xprintf("Input the address,string for check.\n");
                        break;
                    }
                    if (*ptr==' ') ptr++;
                    for (p2=0;*ptr;p2++)//get the string for write
                    {
                        UserBuffer[p2] = *ptr++	 ;
                    }
                    if ((p1+p2)>=MAX_ADDR)//out of range
                    {
                        p2 = MAX_ADDR-p1;
                    }
                    if (FiscalMem_ReadBuffer(UserBuffer+p2,p1, p2)==ERROR)
                    {
                        xputs("Error when read for fc command.\n");
                        break;
                    }
                    for (i=0;i<p2;i++)
                    {
                        if (UserBuffer[i]!=UserBuffer[i+p2])
                        {
                            xprintf("Error found for fc command at:0x%x.\n",p1+i);
                            break;
                        }
                    }
                    if (i==p2)
                        xputs("OK for fW command.\n");
                    break;

                case 'E':
                    if (FiscalMem_EraseBulk()==ERROR)
                        xputs("Error found when erase Chip\n");
                    else
                        xputs("All of the Chip erased");

                    break;
                case 'e' :  /* fe <secFrom> <secTo> - erase blocks */
                    if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2) || p1>p2 || p1>FiscalMem_SEC_MAX || p2>FiscalMem_SEC_MAX)
                    {
                        xputs("Input the section.from and section.to for erase.\n");
                        break;
                    }
                    if (FiscalMem_Erase(p1, p2)==ERROR)
                        xputs("Error found when erase flash\n");
                    else
                        xprintf("Chip erased from %d to %d (Section)\n",p1,p2);

                    break;
                case 'l':
                    xputs("Start to Loading Chinese. \n");
                    //DowmLoadBINCLIB();
                    xputs("Finish of Loading Chinese. \n");
                    break;
                }
                break;

            }//switch
        }
    }//for
}

#endif


#if	(1)//SDIO_SD_DEBUG

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

#define NUMBER_OF_BLOCKS      5  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (1024 * NUMBER_OF_BLOCKS)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
volatile SD_Error SD_Status = SD_OK;
SD_Error Status = SD_OK;

#define Buffer_Block_Tx Buffer_MultiBlock_Tx
#define Buffer_Block_Rx Buffer_MultiBlock_Rx
extern SD_CardInfo SDCardInfo;

/* Private function prototypes -----------------------------------------------*/
void SD_EraseTest(void);

uint32_t SD_SingleBlockTest(uint32_t SDAddr);
uint32_t SD_MultiBlockTest(uint32_t SDAddr);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void Test_SD(char mode)
{
    uint32_t CardCapacity,ret,i;
	NVIC_SDConfiguration();

	if (SD_Detect()== SD_PRESENT)
	{
		xputs(" SD is present. \n");
		//SD_PWR_ON();
		SD_Status = SD_Init();

		if ((SD_Status == SD_OK))
		{
            CardCapacity = SDCardInfo.CardCapacity;
			USART_printf("\r\n SD Init success ");

			USART_printf( " \r\n SD_GetCardInfo is OK ");
			USART_printf( " \r\n CardType is ..%d ", SDCardInfo.CardType );
			USART_printf( " \r\n CardCapacity is ..0x%x", CardCapacity);
            //USART_printf( " \r\n CardCapacity is ..0x%x", (SDCardInfo.CardCapacity % 0x100000000));
			USART_printf( " \r\n CardBlockSize is ..%d ", SDCardInfo.CardBlockSize );
			USART_printf( " \r\n RCA is ..%d ", SDCardInfo.RCA);
			USART_printf( " \r\n ManufacturerID is ..%d ", SDCardInfo.SD_cid.ManufacturerID );

            //SD_EraseTest();

            if (mode=='S')
            {//
                for (i=0;i<CardCapacity;i+=BLOCK_SIZE*NUMBER_OF_BLOCKS)
                {
                    sprintf(TxBuffer,"%d",i);
                    //PutsO(TxBuffer);
                    ret = SD_MultiBlockTest(i);
                    if (ret)
                    {
                        USART_printf( " \r\n SD_MultiBlockTest Error at:%d \n", i+ret-1);
                        break;
                    }
                }
            }
            else
            {
                for (i=0;i<CardCapacity;i+=BLOCK_SIZE)
                {
                    sprintf(TxBuffer,"%d",i);
                    //PutsO(TxBuffer);
                    ret = SD_SingleBlockTest(i);
                    if (ret)
                    {
                        USART_printf( " \r\n SD_SingleBlockTest Error at:%d \n", i+ret-1);
                        break;
                    }
                }
            }
		}
		else
		{
			USART_printf("\r\n ERROR:SD Init failed! ");
			xprintf("\r\n SD_Status=%d \r\n", SD_Status);
		}
	}
	else
	{
		xputs(" SD is not present. \n");
	}
}


/**
  * @brief  Tests the SD card erase operation.
  * @param  None
  * @retval None
  */
void SD_EraseTest(void)
{
  /*------------------- Block Erase ------------------------------------------*/
  if (Status == SD_OK)
  {
    /* Erase NumberOfBlocks Blocks of WRITE_BL_LEN(512 Bytes) */
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
	xprintf("\r\n SD_Erase Status=%d ", Status);
  }

  if (Status == SD_OK)
  {
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);

	xprintf("\r\n SD_ReadMultiBlocks Status=%d ", Status);

    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();

	xprintf("\r\n SD_WaitReadOperation Status=%d ", Status);

    /* Wait until end of DMA transfer */
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of erased blocks */
  if (Status == SD_OK)
  {
  	USART_printf("\r\n SD_EraseTest success");
  }
  else
  {
  	USART_printf("\r\n SD_EraseTest failed");
  }
}

/**
  * @brief  Tests the SD card Single Blocks operations.
  * @param  None
  * @retval None
  */
uint32_t SD_SingleBlockTest(uint32_t SDAddr)
{
  /*------------------- Block Read/Write --------------------------*/
  /* Fill the buffer to send */

  uint32_t i;
  /*--------------- Multiple Block Read/Write ---------------------*/
  /* Fill the buffer to send */
  for (i=0;i<BLOCK_SIZE/2;i++)
  {
      Buffer_Block_Tx[2*i]= i & 0xff;
      Buffer_Block_Tx[2*i+1]= SDAddr & 0xff;
  }

  if (Status == SD_OK)
  {
    /* Write block of 512 bytes on address 0 */
    //Status = SD_WriteBlock(Buffer_Block_Tx, SDAddr, BLOCK_SIZE);
    Status = SD_WriteMultiBlocks(Buffer_Block_Tx, SDAddr, BLOCK_SIZE, 2);
  }

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    //Status = SD_ReadBlock(Buffer_Block_Rx, SDAddr, BLOCK_SIZE);
    Status = SD_ReadMultiBlocks(Buffer_Block_Rx, SDAddr, BLOCK_SIZE, 2);

  }
  if (Status == SD_OK)
      return  Buffercmp(Buffer_Block_Rx, Buffer_Block_Tx, BLOCK_SIZE);
  else
      return 1;

}

/**
  * @brief  Tests the SD card Multiple Blocks operations.
  * @param  None
  * @retval None
  */
uint32_t SD_MultiBlockTest(uint32_t SDAddr)
{
  uint32_t i;
  /*--------------- Multiple Block Read/Write ---------------------*/
  /* Fill the buffer to send */
  //Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);
  for (i=0;i<MULTI_BUFFER_SIZE/2;i++)
  {
      Buffer_MultiBlock_Tx[2*i]= i & 0xff;
      Buffer_MultiBlock_Tx[2*i+1]= SDAddr & 0xff;
  }

  if (Status == SD_OK)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, SDAddr, BLOCK_SIZE, NUMBER_OF_BLOCKS);
  }

  if (Status == SD_OK)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, SDAddr, BLOCK_SIZE, NUMBER_OF_BLOCKS);

  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
      return Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, BLOCK_SIZE*NUMBER_OF_BLOCKS);
  }
  return 1;
}


#endif	 // SDIO_SD_DEBUG



#if NOR_FLASH_TEST

uint16_t hBuff[128];
uint16_t wrBuff[25]={0x1230,0x1231,0x1232,0x1233,0x1234,0x1235,0x1236,0x1237,0x1238,0x1239,0x123a,0x123b,0x123c,0x123d,0x123e,0x123f};
uint16_t rdBuff[25];
void NOR_Flash_Test(void)
{
	uint16_t s_ID, d_ID;
	uint8_t Index;

	Read_SiliconID_OP(&s_ID);
	Read_DeviceID_OP(0, &d_ID);

	xprintf("\r\n SiliconID=0x%x, DeviceID=0x%x\r\n", s_ID, d_ID);
	/*
	NorFlash_WReadBuffer(hBuff, 0x1000, 128);

	for(Index = 0x00; Index < 128; Index++)
	{
		xprintf("\r\n hBuff[%d]=0x%x", Index, hBuff[Index]);
	}
	*/
	xprintf("---------------------------------------------");
	Sector_Erase_OP(NOR_FLASH_DATA_ADDR, 5);
	NorFlash_WriteBuffer(wrBuff, NOR_FLASH_DATA_ADDR, 23);
	NorFlash_WReadBuffer(rdBuff, NOR_FLASH_DATA_ADDR, 23);
	for(Index = 0x00; Index < 23; Index++)
	{
		xprintf("\r\n hBuff[%d]=0x%x", Index, rdBuff[Index]);
	}

}

#endif  //	NOR_FLASH_TEST

/*
void Test_OPDisplay()
{
	uint8_t cnt = 20;

#if 0  //by ouhs
	short i,j;
    BYTE ch=32;

    Cursor.x = 0;
    Cursor.y = 0;

    Bios1_ByPTR(BiosCmd_OPDisp_Cursor,&Cursor);

	for (j=32;j<=128;j+=SCREENWD)
	{
		if (j>32)
	        Bios1_ByVAL(BiosCmd_OPDisp_Scroll,1);
        for (i=0;i<SCREENWD;i++)
        {
            TxBuffer[i] = ch++;
            if (ch>129)
                ch =32;
        }
        TxBuffer[SCREENWD]=0;
        Bios_ByPTR(BiosCmd_OPDisp_StringXY,TxBuffer,0xff,0xff);
		Bios1_ByVAL(BiosCmd_msDelay,400);
	}
#else
	uint16_t i,j=1000;
	mSetInvAttr();
	memcpy(BiosVar.ScreenMap[0],"                ",16);
	mRefreshLine(0);

	mClearInvAttr();
	memcpy(BiosVar.ScreenMap[1],"型号:ER-220GT&TT",16);
	mRefreshLine(1);

	mSetInvAttr();
	memcpy(BiosVar.ScreenMap[2],"Fast & Reliable ",16);
	mRefreshLine(2);

	mClearInvAttr();
	memcpy(BiosVar.ScreenMap[3],"  1  1  1  1  1 ",16);
	mRefreshLine(3);
	memcpy(BiosVar.ScreenMap[4],"设置==确定==取消",16);
	mRefreshLine(4);
	//mMoveCursor(16, 3);

	while(cnt)
	{
		if(cnt-->10)
			mContrastDown();
		else
			mContrastUp();
		while (j--)  for(i=0; i<20000; i++); j=500;	//500ms
	}
	cnt = 20;
	while(cnt)
	{
		if(cnt-->10)
			mContrastUp();
		else
			mContrastDown();
		while (j--)  for(i=0; i<20000; i++); j=500;
	}

	while(0)
	{
		while(i<123)
		{
			i++;
			//mDispStringXY(BiosVar.ScreenMap[i%5], 0, 0);
			//mDispStringXY(BiosVar.ScreenMap[(i+1)%5], 0, 1);
			//mDispStringXY(BiosVar.ScreenMap[(i+2)%5], 0, 2);
			mDispStringXY(BiosVar.ScreenMap[(i+3)%5], 10, 3);
		}
		i = 0;
		SysDelay_ms(1500);
	}
#endif
}
*/

// 测试看门狗
void TEST_IWDG(void)
{
	uint16_t i, j;
	while (j--)  for(i=0; i<20000; i++); j=500;	//500ms
	USART_printf("\r\nStarting IWDG test...\r\n");
	*(__IO uint32_t *) 0xA0001000 = 0x5A;
	USART_printf("\r\nIWDG test Failed!\r\n");
}

// 测试Backup SRAM
BYTE Test_BKPSRAM(void)
{
	DWORD i;
	BYTE twr_data=0xA5,trd_data;
	//测试程序执行时间需要先设置TIM6
	/* Write data to FSMC SRAM memory */
	/* Fill the buffer to send */
	Fill_Buffer(TxBuffer, 4096, 0xf321);	 // 0xf321

	//测试SRAM写入速度,10KB数据
	TIM_Cmd(TIM6, ENABLE);
	BKPSRAM_WriteBuffer(TxBuffer, 0, 4096);
	i = TIM_GetCounter(TIM6)*10;	//us
	USART_printf("\r\n Backup SRAM test: 4KB write time:%dus\r\n",i);
	TIM_Cmd(TIM6, DISABLE);
	//测试SRAM写入速度

	BKPSRAM_ReadBuffer(RxBuffer, 0, 4096);

	/* Read back SRAM memory and check content correctness */
	Buffercmp(TxBuffer, RxBuffer, 4096);
}

//--------------------------------------------------------------------------------//
#if defined(CASE_ETHERNET)
//55 24 31 32 33 34 35 36 37 38 39 30 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F 50 51 52 53 54 55 56 57 58 59 5A
void Ethernet_Test(void)
{
    BYTE sEIE=0,sEIR=0,sESTAT=0;
    BYTE sEIEc=0,sEIRc=0,sESTATc=0;

    if (!Get_ETHERNET_Ready())
    {
        mClearScreen();
        DispStrXY("192.168.1.218",0,0);
        DispStrXY("TCP PORT:7000",0,1);
        DispStrXY("UDP PORT:7001",0,2);
        DispStrXY("ETHERNET TEST....",0,3);
        USART_printf("\r\n*******STM32_enc8j60+lwip测试(192.168.1.218)*********\r\n");


        /*初始化web server 显示网页程序  //用户 EUTR 密码123456 端口80 */
        USART_printf("Initial HTTP(%d).............\r\n",HTTP_PORT);
          httpd_init();

        /* 初始化telnet   远程控制 程序  //用户 EUTR 密码123456 端口23 */
        USART_printf("Initial TelNet.............\r\n");
        CMD_init();


        /* tcp echo server Init */
        USART_printf("Initial TCP/IP(%d).............\r\n",TCPIP_PORT);
        tcp_echoserver_init();

        /* UDP echo server Init */
        USART_printf("Initial UDP(%d).............\r\n",UDP_SERVER_PORT);
        udp_echoserver_init();

        Set_ETHERNET_Ready();//ccr2015-01-22

        LwIP_Periodic_Handle(GetSystemTimer(),1);//ccr2015-01-29 it is must

    }
}
#endif
