
#include "stm32f2xx_it.h"
#include "pwr_ctrl.h"


                            //

__IO uint16_t Vin_Vrtc_Val[2]; // Vin_Vrtc_Val[0]---��ص�ѹ ,Vin_Vrtc_Val[1]---RTC��ѹ

uint8_t PowerFlags;		//��⵽�ĵ�Դ״̬
/*
    Bit0:=0,Ϊ����������;=1,Ϊ��ع���
    Bit1:=1,Ϊ��ع���ʱ,���ֱ���PWR_WARNING1(��ѹ�ε�)
    Bit2:=1,Ϊ��ع���ʱ,���ֱ���PWR_WARNING2(��ѹ����)

    Bit4:=1,Ŧ�۵�س��ֱ���PWR_WARNING1(��ѹ�ε�)
    Bit5:=1,Ŧ�۵�س��ֱ���PWR_WARNING2(��ѹ����)
*/

void ADC2_DMA_Config(void);
void pwrIO_Init(void);
void pwrVolDetecCtrl(uint8_t NewState);
void pwrADCCtrl(FunctionalState NewState);


extern void	USART_printf (const uint8_t *format, ...);

/**************************************************************************/
/* ��Դ�����ʼ������
***************************************************************************/
void pwrDelay_ms(uint16_t cnt)
{
	uint16_t i;
	while (cnt--)
		for(i=0; i<20000; i++);
}


#if 0
void pwr_Test(void)
{
	uint8_t stat;
	uint8_t cnt=1;

	while(cnt--)
	{
		pwrDelay_ms(1000);
		//if(DC_DET_GET() == 0)  //test by ouhs
		{
			pwrDelay_ms(2);
			//if(DC_DET_GET() == 0)
			{//ϵͳ�ɵ�ع���ʱ����ص���
				stat = pwrGetStatus(GET_VIN_STATUS);
				switch (stat)
				{
					case PWR_STAT_OK: USART_printf("Battery OK"); break;
					case PWR_WARNING1: USART_printf("Warning:Low Battery!"); break;
					case PWR_WARNING2: USART_printf("Fatal Warning:Low Battery!"); break;
					default: break;
				}
			}
		}

		// ��ϵͳ����ʱ���RTC��ص���һ��
		stat = pwrGetStatus(GET_VRTC_STATUS);
		switch (stat)
		{
			case PWR_STAT_OK: USART_printf("Battery OK\r\n"); break;
			case PWR_WARNING1: USART_printf("Warning:Low RTC Battery!\r\n"); break;
			case PWR_WARNING2: USART_printf("Fatal Warning:Low RTC Battery!\r\n"); break;
			default: break;
		}
	}
}
#endif

void pwr_Init(void)
{
	pwrIO_Init();
	ADC2_DMA_Config();
	pwrADCCtrl(ENABLE);
}

void pwrIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DC_DET_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ADC Channel 14 -> PC4, 15 -> PC5 */
	GPIO_InitStructure.GPIO_Pin = VIN_Pin | VRTC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


/**************************************************************************/
/* ADC2��������:ADC2_14���ڼ������Դ��ѹ��ADC2_15���ڼ��RTC��ص�ѹ
***************************************************************************/
void ADC2_DMA_Config(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
#if (1)
	/* DMA2_Stream2 channel1 configuration **************************************/
	DMA_DeInit(DMA2_Stream2);
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC2_DR_ADDR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Vin_Vrtc_Val;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;//DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	/* DMA2_Stream2 enable */
	DMA_Cmd(DMA2_Stream2, ENABLE);
#endif
	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;//ADC_Prescaler_Div2;	//testonly
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	//ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_15Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC2 regular channels 14 15 configuration*/

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//ת������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 2;	   //ɨ��ͨ����
	ADC_Init(ADC2, &ADC_InitStructure);

	/* ADC2 regular channels 14, 15 configuration */   //	�ⲿ�����迹̫����Ҫ�϶�Ĳ������ڲ��ܱ�֤����
	ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 1, ADC_SampleTime_480Cycles);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_15, 2, ADC_SampleTime_480Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);

	//ADC_DMACmd(ADC2, ENABLE);

	/* Enable ADC2 */
	//ADC_Cmd(ADC2, ENABLE);

	/* Start ADC2 Software Conversion */
	//ADC_SoftwareStartConv(ADC2);
}

/**************************************************************************/
/* ��ѹ���ADC���ܿ�����رա�������NewState=ENABLE---������DISABLE---�ر�
***************************************************************************/
void pwrADCCtrl(FunctionalState NewState)
{
	// ����ADC�������һ��PFO�����źţ�����
	ADC_DMACmd(ADC2, NewState);
	ADC_Cmd(ADC2, NewState);
	ADC_SoftwareStartConv(ADC2);
	if(NewState == DISABLE)
	{//�رյ�ѹ���ʱ��0
		Vin_Vrtc_Val[0] = 0;
		Vin_Vrtc_Val[1] = 0;
	}
}

/**************************************************************************/
/*@param	GET_VIN_STATUS
			GET_VRTC_STATUS
  @retval	PWR_STAT_OK
			PWR_WARNING1
			PWR_WARNING2
***************************************************************************/
/*
    Bit0:=0,Ϊ����������;=1,Ϊ��ع���
    Bit1:=1,Ϊ��ع���ʱ,���ֱ���PWR_WARNING1(��ѹ�ε�)
    Bit2:=1,Ϊ��ع���ʱ,���ֱ���PWR_WARNING2(��ѹ����)

    Bit4:=1,Ŧ�۵�س��ֱ���PWR_WARNING1(��ѹ�ε�)
    Bit5:=1,Ŧ�۵�س��ֱ���PWR_WARNING2(��ѹ����)
*/
uint8_t pwrGetStatus(uint8_t GetStatus)
{
	uint8_t status=0;
	uint16_t Vin_Val, Vrtc_Val;

	PowerFlags = 0;

#if POWERCTRL
	//return 0;//testonly skip test
	if (!DC_DET_GET())
	{//Ϊ��ع���
		PowerFlags = PWR_BY_BAT;//=1,Ϊ��ع���

		if (GetStatus == GET_VIN_STATUS)
		{
			Vin_Val = Vin_Vrtc_Val[0];
			//USART_printf("VIN=0x%x\n", Vin_Val);	//testonly by ouhs
		    if(Vin_Val > VIN_OK)
			{
				status = PWR_STAT_OK;
			}
			else if(Vin_Val > VIN_LOW)
			{
				status = PWR_WARNING1;
				PowerFlags = PWR_BY_BAT | PWR_WARNING1;//Ϊ��ع���ʱ,���ֱ���PWR_WARNING1(��ѹ�ε�)����ʾ���
			}
			else
			{
				status = PWR_WARNING2;
				PowerFlags = PWR_BY_BAT | PWR_WARNING2;//Ϊ��ع���ʱ,���ֱ���PWR_WARNING2(��ѹ����),ֹͣʹ��
			}
		}
	}
#endif
	if(GetStatus == GET_VRTC_STATUS)
	{
		Vrtc_Val = Vin_Vrtc_Val[1];
		//USART_printf("\r\nVrtc=0x%x  \n", Vrtc_Val);	//test by ouhs
		if(Vrtc_Val > VRTC_2V5)
		{
			status = PWR_STAT_OK;
		}
		else if(Vrtc_Val > VRTC_2V2)
		{
			status = PWR_WARNING1;
			PowerFlags |= PWR_BUT_WARNING1;//Ŧ�۵�س��ֱ���PWR_WARNING1(��ѹ�ε�)
		}
		else
		{
			status = PWR_WARNING2;
			PowerFlags |= PWR_BUT_WARNING2;//Ŧ�۵�س��ֱ���PWR_WARNING2(��ѹ����)
		}
	}
	return status;
}
