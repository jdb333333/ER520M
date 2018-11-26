/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
//#include "king.h"		//king.h中的某个定义与以下的某个定义有冲突,会引起串口通信异常
//#include "exthead.h"
//#include "bios_dir.h"

#include "stm32f2xx_it.h"
#include "sdio_sd.h"
#include "usart.h"
#include "TypeDef.h"
#if defined(CASE_ASCIIDISPLAY)
#include "lcd_160x.h"
#else
#include "chnDisp.h"
#endif

#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"

#include "pwr_ctrl.h"
#include "SysTick.h"

#if defined(CASE_ETHERNET)
#include "enc28j60.h"
#endif

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/** SysTick Interrupt Handler (1ms)
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
volatile unsigned long RTCTimer=0;//系统计时器,以毫秒为单位递增计时
volatile unsigned long msTimer_Delay0=0,//延时计时器0
         msTimer_Delay1=0,//延时计时器1
         msTimer_Delay2=0;//延时计时器2

void SysTick_Handler(void)
{
  if (msTimer_Delay0)    msTimer_Delay0--;
  if (msTimer_Delay1)    msTimer_Delay1--;
  if (msTimer_Delay2)    msTimer_Delay2--;

  RTCTimer++;

#if !defined(CASE_INNERDOTPRN)
  if (Prn_Delay)
  {//打印机因为过热而暂停
	  Prn_Delay++;//打印机暂停时长
	  if (Prn_Delay>PRNPAUSEMAX)
		  Prn_Delay = 0;//打印机结束暂停
  }
#endif

}


/**
* @brief  This function handles USRAT interrupt request.
* @param  None
* @retval None
*/
extern void SetRTS(BYTE port);  //ouhs 20140814
void USART1_IRQHandler(void)
{
	uint8_t RxData;
	uint32_t sCount;

	/* USART in mode Tramitter -------------------------------------------------*/
	if (USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		/* Disable the USARTx transmit data register empty interrupt */
		//USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
		UART0TxEmpty = 1;
	}

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		/* Receive the USART data */
		RxData = USART_ReceiveData(USART1);

		sCount = RxUART[0].PutIn+1;
		if (sCount>=rxBUFSIZE)
			sCount = 0;

		if (sCount != RxUART[0].GetOut)
		{
			RxUART[0].RxBuffer[RxUART[0].PutIn] = RxData;
			RxUART[0].PutIn = sCount;
			if (sCount>RxUART[0].GetOut)
				sCount -= RxUART[0].GetOut;
			else
				sCount += (rxBUFSIZE-RxUART[0].GetOut);
            if (RxUART[0].Status & XOFF_FLOWCTRL)
            {
	            if (sCount>XOFF_AT && !BIT(RxUART[0].Status ,XOFF_Flag))
	            {
	                SETBIT(RxUART[0].Status ,XOFF_Flag);
					UARTSend(LOGIC_COM1,XOFF);  //by ouhs
				}
            }
            else
            {
                if (RxUART[0].Status & RTS_FLOWCTRL)
                {
                    if (sCount>XOFF_AT && !BIT(RxUART[0].Status ,RTS_Flag))
                    {
                        SetRTS(0);
                        SETBIT(RxUART[0].Status ,RTS_Flag);
                    }
                }
            }
		}
	}
}

void USART3_IRQHandler(void)
{
	uint8_t RxData;
	uint32_t sCount;

	/* USART in mode Tramitter -------------------------------------------------*/
	if (USART_GetITStatus(USART3, USART_IT_TXE) == SET)
	{
        UART1TxEmpty = 1;
	}

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		/* Receive the USART data */
		RxData = USART_ReceiveData(USART3);
        sCount = RxUART[1].PutIn+1;//address for new data
        if (sCount>=rxBUFSIZE)
            sCount = 0;

        if (sCount != RxUART[1].GetOut)
        {//buffer dose not full
            RxUART[1].RxBuffer[RxUART[1].PutIn] = RxData;
            RxUART[1].PutIn = sCount;//point to the next address
            //count of the data in the buffer
            if (sCount>RxUART[1].GetOut)
                sCount -= RxUART[1].GetOut;
            else
                sCount += (rxBUFSIZE-RxUART[1].GetOut);
            if (RxUART[1].Status & XOFF_FLOWCTRL)
            {

	            if (sCount>XOFF_AT && !BIT(RxUART[1].Status ,XOFF_Flag))
	            {
	                SETBIT(RxUART[1].Status ,XOFF_Flag);
					UARTSend(LOGIC_COM2,XOFF);  //by ouhs
                }
            }
            else
            {
                if (RxUART[1].Status & RTS_FLOWCTRL)
                {
                    if (sCount>XOFF_AT && !BIT(RxUART[1].Status ,RTS_Flag))
                    {
                        SetRTS(1);
                        SETBIT(RxUART[1].Status ,RTS_Flag);
                    }
                }
            }
		}
	}
}

#if USART_EXTEND_EN
void USART2_IRQHandler(void)
{
	uint8_t RxData;
	uint32_t sCount;

	/* USART in mode Tramitter -------------------------------------------------*/
	if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
	{
		/* Disable the USARTx transmit data register empty interrupt */
		//USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
		UART2TxEmpty = 1;
	}

	/* USART in mode Receiver --------------------------------------------------*/
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		/* Receive the USART data */
		RxData = USART_ReceiveData(USART2);//        USART_SendData(UART0, RxData);//testonly//ccr2014-12-25   			// 写入数据

		sCount = RxUART[2].PutIn+1;
		if (sCount>=rxBUFSIZE)
			sCount = 0;

		if (sCount != RxUART[2].GetOut)
		{
			RxUART[2].RxBuffer[RxUART[2].PutIn] = RxData;
			RxUART[2].PutIn = sCount;
			if (sCount>RxUART[2].GetOut)
				sCount -= RxUART[2].GetOut;
			else
				sCount += (rxBUFSIZE-RxUART[2].GetOut);

            if (RxUART[2].Status & XOFF_FLOWCTRL)
            {
	            if (sCount>XOFF_AT && !BIT(RxUART[2].Status ,XOFF_Flag))
	            {
	                SETBIT(RxUART[2].Status ,XOFF_Flag);
					UARTSend(LOGIC_COM3,XOFF);  //by ouhs
                }
            }
            else
            {
                if (RxUART[2].Status & RTS_FLOWCTRL)
                {
                    if (sCount>XOFF_AT && !BIT(RxUART[2].Status ,RTS_Flag))
                    {
                        SetRTS(2);
                        SETBIT(RxUART[2].Status ,RTS_Flag);
                    }
                }
            }
		}
	}
}
#endif	//USART_EXTEND_EN



/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/
extern volatile uint8_t KeyScanEnable;

/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  None
  * @retval None
  */
extern void (*PWF_CallBack)(void);//ccr20110901
extern volatile uint8_t KeyScanEnable; //ouhs 20140814
void EXTI2_IRQHandler(void)
{//掉电中断服务程序
    uint16_t t1,t2;

#if defined(CASE_ASCIIDISPLAY)
	Copy_LCD(false);
#else
	mLightLCDOff();
    LED_Off();
#endif

    Save_Config(false);//ccr2016-01-15


    if (ApplVar_Restored==0x5a)
	{
		Save_ApplRam(); //Save_Config(true);
	}

	EXTI_ClearITPendingBit(EXTI_Line2);

    //ccr2014-08-27>>>>>>>>>>>>>>>>
#if POWERCTRL
    if (DC_DET_GET())/* TRUE--适配器供电, FALSE--电池供电*/
#endif
    {//适配器供电
        t2 = GPIOE->IDR & GPIO_Pin_2;//判断掉电信号检测位
        do {
            t1 = t2;
            for (t2=0;t2<5000;t2++){};
            t2 = GPIOE->IDR & GPIO_Pin_2;
        } while (t1!=t2);

        if (t1==0)
        {//低电平:掉电
            __disable_irq();    //开机有打印时,有可能引起掉电,此时禁止所有中断将引起系统运行异常
            __disable_fault_irq();
            t2 = GPIOE->IDR & GPIO_Pin_2;//判断掉电信号检测位
            do {
                t1 = t2;
                for (t2=0;t2<5000;t2++){};
                t2 = GPIOE->IDR & GPIO_Pin_2;
            } while (t1!=t2 || t2==0);
            __enable_irq();    //开机有打印时,有可能引起掉电,此时禁止所有中断将引起系统运行异常
            __enable_fault_irq();
            Start_When_Ready(0);//如果有未完成的打印任务,启动打印
        }
    }
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}


/**
  * @brief  This function handles External line 9..5 interrupt request.
  * @param  None
  * @retval None
  */
void LwIP_Periodic_Handle(__IO uint32_t localtime,uint8_t fromINT);

extern volatile uint8_t KeyScanEnable;

void EXTI9_5_IRQHandler(void)
{

  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {	//按键中断
	  KeyScanEnable = 1;
	  EXTI_ClearITPendingBit(EXTI_Line6);
	  //USART_printf("EXTI_IRQHandler\r\n");  //testonly by ouhs
  }
  if(EXTI_GetITStatus(EXTI_Line8) != RESET)
  {	//以太网中断
	  /**/
#if defined(CASE_ETHERNET)
#if !defined(CASE_ASCIIDISPLAY)
      mDrawGPRS(SCREENWD-1,0,0);//ccr2015-01-29 testonly
#endif
      LwIP_Periodic_Handle(GetSystemTimer(),1);//ccr2015-01-20
#endif
      EXTI_ClearITPendingBit(EXTI_Line8);
  }
}


/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles DMA2 Stream3 or DMA2 Stream6 global interrupts
  *         requests.
  * @param  None
  * @retval None
  */
void SD_SDIO_DMA_IRQHANDLER(void)
{
  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
  SD_ProcessDMAIRQ();
}


/**
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;

void OTG_FS_IRQHandler(void)
{
	USBH_OTG_ISR_Handler(&USB_OTG_Core);
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
