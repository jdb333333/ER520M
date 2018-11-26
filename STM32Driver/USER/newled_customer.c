
#include "stm32f2xx.h"
#include "led_customer.h"

/*
//          a
//        ┏━┓
//       f┃g ┃ b
//        ┣━┫
//       e┃d ┃ c
//        ┗━┛● DP
*/

#define LED_DATA	 		GPIO_Pin_13
#define	LED_CLK_High()	 	GPIOA->BSRRL =  GPIO_Pin_14
#define	LED_CLK_Low()	 	GPIOA->BSRRH =  GPIO_Pin_14
#define	LED_LATCH_High()	GPIOA->BSRRL =  GPIO_Pin_15
#define	LED_LATCH_Low()		GPIOA->BSRRH =  GPIO_Pin_15


/* LED字模, 共阴数码管 */
const uint8_t LED_TAB[] = {
0x00,//0x00,//00000000B    ; 32 blank
0x86,//0x61,//01100001B    ; 33 !
0x22,//0x44,//01000100B    ; 34 @
0x00,//0x00,//00000000B    ; 35 #
0x00,//0x00,//00000000B    ; 36 $
0x00,//0x00,//00000000B    ; 37 %
0x00,//0x00,//00000000B    ; 38 &
0x02,//0x40,//01000000B    ; 39 '
0x39,//0x9C,//10011100B    ; 40 (
0x0F,//0xF0,//11110000B    ; 41 )
0x00,//0x00,//00000000B           ; 42 *
0x00,//0x00,//00000000B           ; 43 +
0x80,//0x01,//00000001B           ; 44 ,
0x40,//0x02,//00000010B           ; 45 -
0x80,//0x01,//00000001B           ; 46 .
0x52,//0x4A,//01001010B           ; 47 /
0x3F,//0xFC,//11111100B           ; 48 0
0x06,//0x60,//01100000B           ; 49 1
0x5B,//0xDA,//11011010B           ; 50 2
0x4F,//0xF2,//11110010B           ; 51 3
0x66,//0x66,//01100110B           ; 52 4
0x6D,//0xB6,//10110110B           ; 53 5
0x7D,//0xBE,//10111110B           ; 54 6
0x07,//0xE0,//11100000B           ; 55 7
0x7F,//0xFE,//11111110B           ; 56 8
0x6F,//0xF6,//11110110B           ; 57 9
0x09,//0x90,//10010000B           ; 58 :
0x48,//0x12,//00010010B           ; 59 ;
0x00,//0x00,//00000000B           ; 60 <
0x48,//0x12,//00010010B           ; 61 =
0x00,//0x00,//00000000B           ; 62 >
0x08,//0x10,//00010000B           ; 63 _
0x00,//0x00,//00000000B           ; 64 @
0x77,//0xEE,//11101110B           ; 65 A
0x7C,//0x3E,//00111110B           ; 66 B
0x39,//0x9C,//10011100B           ; 67 C
0x5E,//0x7A,//01111010B           ; 68 D
0x79,//0x9E,//10011110B           ; 69 E
0x71,//0x8E,//10001110B           ; 70 F
0x3D,//0xBC,//10111100B           ; 71 G
0x76,//0x6E,//01101110B           ; 72 H
0x30,//0x0C,//00001100B           ; 73 I
0x1E,//0x78,//01111000B           ; 74 J
0x72,//0x4E,//01001110B           ; 75 K
0x38,//0x1C,//00011100B           ; 76 L
0x37,//0xEC,//11101100B           ; 77 M
0x54,//0x2A,//00101010B           ; 78 N
0x5C,//0x3A,//00111010B           ; 79 O
0x73,//0xCE,//11001110B           ; 80 P
0x67,//0xE6,//11100110B           ; 81 Q
0x50,//0x0A,//00001010B           ; 82 R
0x6D,//0xB6,//10110110B           ; 83 S
0x78,//0x1E,//00011110B           ; 84 T
0x1C,//0x38,//00111000B           ; 85 U
0x3E,//0x7C,//01111100B           ; 86 V
0x6A,//0x56,//01010110B           ; 87 W
0x49,//0x92,//10010010B           ; 88 X
0x6E,//0x76,//01110110B           ; 89 Y
0x5B,//0xDA,//11011010B           ; 90 Z
0x39,//0x9C,//10011100B           ; 91 [
0x64,//0x26,//00100110B           ; 92 \
0x0F,//0xF0,//11110000B           ; 93 ]
0x23,//0xC4,//11000100B           ; 94 ^
0x08,//0x10,//00010000B           ; 95 _
0x00,//0x00,//00000000B           ; 96 '
0x77,//0xEE,//11101110B           ;  a
0x7C,//0x3E,//00111110B           ;  b
0x39,//0x9C,//10011100B           ;  c
0x5E,//0x7A,//01111010B           ;  d
0x79,//0x9E,//10011110B           ;  e
0x71,//0x8E,//10001110B           ;  f
0x3D,//0xBC,//10111100B           ;  g
0x76,//0x6E,//01101110B           ;  h
0x30,//0x0C,//00001100B           ;  i
0x1E,//0x78,//01111000B           ;  j
0x72,//0x4E,//01001110B           ;  k
0x38,//0x1C,//00011100B           ;  l
0x37,//0xEC,//11101100B           ;  m
0x54,//0x2A,//00101010B           ;  n
0x5C,//0x3A,//00111010B           ;  o
0x73,//0xCE,//11001110B           ;  p
0x67,//0xE6,//11100110B           ;  q
0x50,//0x0A,//00001010B           ;  r
0x6D,//0xB6,//10110110B           ;  s
0x78,//0x1E,//00011110B           ;  t
0x1C,//0x38,//00111000B           ;  u
0x3E,//0x7C,//01111100B           ;  v
0x6A,//0x56,//01010110B           ;  w
0x49,//0x92,//10010010B           ;  x
0x6E,//0x76,//01110110B           ;  y
0x5B,//0xDA,//11011010B           ;  z
};

uint8_t LED_Buffer[12] = {0x3f,0x3f,0xbf,};	//0.00
uint8_t pos_shift=0;
volatile uint8_t slen=0;
uint16_t LED_G;
uint16_t LED_Ctrl=0;

static void LED_SendBit(uint8_t databit);
static void LED_SendData(uint16_t pos, uint8_t dat);
static void LED_Delay(void);

void LED_Test(void);

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// GPIO_Pin_13--LED_DATA, GPIO_Pin_14--LED_CLK, GPIO_Pin_15--LED_LATCH(CS)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//LED_SendData(0xff, 0xff);  // 在去掉ULN2003直接用74HC595驱动数码管的电路中，执行这一条可能会使74HC595过载
	LED_SendData(0, 0xff);
	LED_Ctrl = 500;	//500ms后刷新LED显示

}

static void LED_Delay(void)
{
	uint8_t i;
	//while (cnt--)
		for(i=0; i<10; i++);
}

// GPIO模拟SPI通讯发送位数据
static void LED_SendBit(uint8_t databit)
{
	if (databit & 0x01)
		GPIOA->BSRRL = LED_DATA;
	else
		GPIOA->BSRRH = LED_DATA;
	LED_Delay();
	LED_CLK_High();
	LED_Delay();
	LED_CLK_Low();
}

// 发送LED位置、数据
static void LED_SendData(uint16_t pos, uint8_t dat)
{
	uint8_t i;
	LED_LATCH_Low();
	for (i=0; i<DISP_WIDTH; i++)	  // 位置
	{
		LED_SendBit(pos>>i);
	}
	for (i=0; i<8; i++)				  // 数据
	{
		LED_SendBit(dat>>i);
	}
	LED_LATCH_High();
}

// 刷新LED显示
void LED_Refresh(void)
{
	if(pos_shift<DISP_WIDTH)
	{
#if 0
		//LED_G = 0;				//使用ULN2003A
		//LED_G |= (1<<pos_shift);
#else
		LED_G = 0xfff;				//没有使用ULN2003A
		if(pos_shift<slen)
			LED_G &= ~(1<<(slen-pos_shift-1));
		else
			LED_G &= ~(1<<(pos_shift));
#endif
		LED_SendData(LED_G, LED_Buffer[pos_shift]);
		pos_shift++;
	}
	else
	{
		pos_shift = 0;
	}
}

// 更新需要显示的数据
void PutsO( const char *str)
{
	uint8_t pos, ch;
	slen = 0;
	for(pos=0; pos<DISP_WIDTH; pos++)
    {
        if(*str)
		{
			ch = *str;
			if (ch == 0x2E)	  // 小数点处理
			{
				LED_Buffer[--pos] |= 0x80;
			}
			else
			{
				LED_Buffer[pos] = LED_TAB[ch - 0x20];
				slen++;
			}
			str++;
		}
		else
		{
			LED_Buffer[pos] = 0;
		}
    }
}


// 关闭定时刷新，向所有LED写0关闭显示
void LED_Off(void)
{
	LED_Ctrl = 0;
	SysDelay_ms(10);
	LED_SendData(0, 0);
}

// 点亮LED,显示前次显示的数据
void LED_On(void)
{
	LED_Ctrl = 1;
}

// TIM6 1ms 中断处理
volatile uint16_t TimeDelay;
extern void mDispCursor(void);
uint16_t CursorTime;
void TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		/*  LED客显  */
		if(LED_Ctrl > 1)
			LED_Ctrl--;
		else if(LED_Ctrl==1)
			LED_Refresh();

		 /*  LCD光标 */
		CursorTime++;
		if(CursorTime == 500)
		{
			CursorTime = 0;
			//if( ScreenStat & CURSOREN )
			{
				mDispCursor();
			}
		}

		if(TimeDelay)
			TimeDelay--;

		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}

// 延时ms
void SysDelay_ms(uint16_t delay_ms)
{
	TimeDelay = delay_ms;
	while(TimeDelay);
}

#if (1)

void LED_Test(void)
{
	while(1)
	{
		SysDelay_ms(2000);
		PutsO("123.456");
		SysDelay_ms(2000);
		PutsO("0123456789ab");
		SysDelay_ms(2000);
		PutsO("789:456");
		SysDelay_ms(2000);
		PutsO("123---===456");
	}
}
#endif
