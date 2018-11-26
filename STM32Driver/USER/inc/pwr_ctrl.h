
#ifndef __PWR_CTRL_H
#define __PWR_CTRL_H

#define POWERCTRL       1 //ouhs 1   //=1,������е�ؼ��


#define	VIN_Pin		   	GPIO_Pin_4
#define	VRTC_Pin		GPIO_Pin_5

#define ADC2_DR_ADDR   ((uint32_t)0x4001214C)	//(APB2PERIPH_BASE + 0x2100) + 0x4C

//���ⲿ�����迹������ʵ�ʲ���ֵС�����ۼ���ֵ��
// ER-260: Vbat=Vad+5.625  ���緧ֵVpfi=5.65V
// ER-220��V=Vin*12K/(47K+12K) ����yֵVpfi=5.65V

//����ؼ���׼ֵ.����ֵ:(DR/0xfff)*3300=DR*0.8059
#define VIN_8V7 0xEC8 //8.7V---3075mV---0xEE8
#define VIN_8V6 0xE4C //8.6V---2975mV---0xE6C
#define VIN_8V5 0xDCF //8.5V---2875mV---0xDEF
#define VIN_8V4 0xD53 //8.4V---2775mV---0xD73
#define VIN_8V3 0xCD7 //8.3V---2675mV---0xCF7
#define VIN_8V2 0xC5B //8.2V---2575mV---0xC7B
#define VIN_8V1 0xBDF //8.1V---2475mV---0xBFF
#define VIN_8V0 0xB63 //8.0V---2375mV---0xB83
#define VIN_7V9 0xAE7 //7.9V---2275mV---0xB07
#define VIN_7V8 0xA6B //7.8V---2175mV---0xA8B
#define VIN_7V7 0x9EF //7.7V---2075mV---0xA0F
#define VIN_7V6 0x973 //7.6V---1975mV---0x993
#define VIN_7V5 0x8F7 //7.5V---1875mV---0x917
#define VIN_7V4 0x87B //7.4V---1775mV---0x89B
#define VIN_7V3 0x7FE //7.3V---1675mV---0x81E
#define VIN_7V2 0x782 //7.2V---1575mV---0x7A2
#define VIN_7V1 0x706 //7.1V---1475mV---0x726
#define VIN_7V0 0x68A //7.0V---1375mV---0x6AA

#define VIN_OK  VIN_7V2
#define VIN_LOW VIN_7V0

//Ŧ�۵�ؼ���׼ֵ
#define VRTC_3V0	0xD68	//3.0V---2769mV---0xD74, 3.16---2917mV---0xE2C-0xE20=0xC
#define	VRTC_2V5	0xB2B	//2.5V---2308mV---0xB37
#define	VRTC_2V2	0x9D2	//2.2V---2031mV---0x9DE


//���ϵͳ���緽ʽ
#define DC_DET_Pin		GPIO_Pin_10
#define DC_DET_GET()	((GPIOA->IDR & DC_DET_Pin) != 0)    /* TRUE--����������, FALSE--��ع���*/


#define	GET_VIN_STATUS	1
#define	GET_VRTC_STATUS	2

#define PWR_BY_BAT      0x01    //Ϊ��ع���

#define PWR_STAT_OK		1

#define PWR_WARNING1	0x02        //�����ص�ѹ������1
#define PWR_WARNING2	0x04        //�����ص�ѹ������2

#define PWR_BUT_WARNING1     0x10   //Ŧ�۵�ص�ѹ������1
#define PWR_BUT_WARNING2     0x20   //Ŧ�۵�ص�ѹ������2

#if (!defined(DEBUGBYPC))
extern __IO uint16_t Vin_Vrtc_Val[2];
extern uint8_t PowerFlags;		//��⵽�ĵ�Դ״̬
/*
    Bit0:=0,Ϊ����������;=1,Ϊ��ع���
    Bit1:=1,Ϊ��ع���ʱ,���ֱ���PWR_WARNING1(��ѹ�ε�)
    Bit2:=1,Ϊ��ع���ʱ,���ֱ���PWR_WARNING2(��ѹ����)

    Bit4:=1,Ŧ�۵�س��ֱ���PWR_WARNING1(��ѹ�ε�)
    Bit5:=1,Ŧ�۵�س��ֱ���PWR_WARNING2(��ѹ����)
*/

#define BATTERYTooLow(v)  ((PowerFlags & (v))==(v))
#define BatteryVoltage()   (Vin_Vrtc_Val[0])
#define BatteryExist()   (Vin_Vrtc_Val[0]<0xfff)//����Ƿ��е��(4095)

void pwr_Init(void);
uint8_t pwrGetStatus(uint8_t GetStatus);
#endif


#endif	//__PWR_CTRL_H
