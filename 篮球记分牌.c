#include "config.h"
#include "GPIO.h"
#include "delay.h"
#include "timer.h"
#define LED0	P37
#define LED1	P41
#define Data_Bit	P0
#define A138_Bit	P76
#define B138_Bit	P77
#define C138_Bit	P74
#define Buzzer_Bit	P71

sbit	K0 = P3^2;	//int0
sbit	K1 = P3^3;	//int1
sbit	K2 = P3^4;	
sbit	K3 = P5^0;
sbit	K4 = P5^1;	
sbit	K5 = P3^5;
sbit	K6 = P3^6;	//int2 

unsigned char code dis_t[] = {0xFC,0x90,0xE5,0xF1,0x99,0x79,0x7D,0xD0,0xFD,0xF9,0xDD,0x3D,0x6C,0xB5,0x6D,0x4D,0x02, 0x01};
unsigned char smg_data[8] ={0};
unsigned char bai1 = 0, shi1 = 0, ge1 = 0, bai2 = 0, shi2 = 0, ge2 = 0;
unsigned char timer0_cnt = 0;

void Smg_test(void);
void timer1_cnt_fun(void);
/******************** IO���ú��� **************************/
void	LED_GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;		
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;		
	GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);	
}

/******************** IO���ú��� **************************/
void GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		
	
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P7, &GPIO_InitStructure);	
}

/************************ ��ʱ������ ****************************/
void Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//�ṹ����
	
/*	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 2000);		//��ֵ2K
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer0, &TIM_InitStructure);				//��ʼ��Timer0	  Timer0,Timer1,Timer2
*/
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//ָ������ģʽ,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//ָ���ж����ȼ�, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//ָ��ʱ��Դ, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 12 / 100);		//��ֵ// 100Hz
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer1, &TIM_InitStructure);				//��ʼ��Timer1	  Timer0,Timer1,Timer2

	TIM_InitStructure.TIM_Interrupt = ENABLE;				//�ж��Ƿ�����,   ENABLE��DISABLE. (ע��: Timer2�̶�Ϊ16λ�Զ���װ, �жϹ̶�Ϊ�����ȼ�)
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//ָ��ʱ��Դ,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//�Ƿ������������, ENABLE��DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 12 / 50);		//��ֵ// 50Hz
	TIM_InitStructure.TIM_Run       = ENABLE;				//�Ƿ��ʼ����������ʱ��, ENABLE��DISABLE
	Timer_Inilize(Timer2, &TIM_InitStructure);				//��ʼ��Timer2	  Timer0,Timer1,Timer2
	EA = 1;//�����ж�
}


/******************** ������ **************************/
void main(void)
{
	LED_GPIO_config();
	GPIO_config();
	Timer_config();
	
	while(1)
	{
		LED0 = 0;
		delay_ms(100);
		LED0 = 1;
		delay_ms(100);
	}
}

/********************* Timer0�жϺ���************************/
void timer0_int (void) interrupt TIMER0_VECTOR
{
	if(timer0_cnt >= 200)
	{
		timer0_cnt = 0;
		
		LED1 = ~LED1;
	}else
	{
		timer0_cnt++;
	}
}

/********************* Timer1�жϺ���************************/
void timer1_int (void) interrupt TIMER1_VECTOR
{
	timer1_cnt_fun();//10ms

}

/********************* Timer2�жϺ���************************/
void timer2_int (void) interrupt TIMER2_VECTOR
{
	Smg_test();
}


 void delay_us(unsigned int ms)
{
	while(ms--)
		_nop_();
}


void HC138_bit(unsigned char dat)
{
	A138_Bit = (dat & 0x01) ? 1 : 0;
	B138_Bit = (dat & 0x02) ? 1 : 0;
	C138_Bit = (dat & 0x04) ? 1 : 0;
}


void Smg_test(void)
{
	unsigned char i;
	
	for(i=0; i<8; i++)
	{
		HC138_bit(i);
		Data_Bit = dis_t[smg_data[i]];
		delay_us(600);
		Data_Bit = 0x00;
	}
}

void updata_display(void)
{
	smg_data[0] = ge1;
	smg_data[1] = shi1;
	
	smg_data[2] = bai1;
	
	smg_data[3] = 17;
	smg_data[4] = 17;
	
	smg_data[5] = bai2;
	
	smg_data[6] = shi2;
	smg_data[7] = ge2;
}

void timer1_cnt_fun(void)
{

	if(K0 == 0)
	{			  
		delay_ms(1000);
		ge1 ++;
		delay_ms(1000);
		if(ge1 > 9)
		{
		shi1 ++;
		ge1 = 0;
		delay_ms(1000);
		if(shi1 >9)
		{
			bai1 ++;
			shi1 = 0;
			delay_ms(1000);
			if( ge1 == 9 && bai1 == 9 && shi1 ==9)
			{
				ge1 = 9;  bai1 = 9;  shi1 =9;
			}
		}
		}
	}

	if(K1 == 0)
	{			  
		delay_ms(1000);
		if(bai1 == 0 && shi1 ==0)
		{
			ge1 --;
			if(ge1 <= 0)
			{
				ge1 = 0;
			}
		}
		if(bai1 == 0 && shi1 > 0)
		{
			if(ge1 > 0)
			{
				ge1 --;
			}
			else
			{
				shi1 --;
				ge1 = 9;
			}
		}
		if(bai1 > 0)
		{
			if(ge1 > 0)
			{
				ge1 --;
			}
			else
			{
				if(shi1 > 0)
				{
					shi1 --;
					ge1 = 9;
				}
				else
				{
					bai1 --;
					shi1 = 9;
					ge1 = 9;
				}
				
			}
		}
			
	}	 	

	if(K3 == 0)
	{
		delay_ms(1000);
		ge2 ++;
		delay_ms(1000);
		if(ge2 > 9)
		{
			shi2 ++;
			ge2 = 0;
			if(shi2 >9)
			{
				bai2 ++;
				shi2 = 0;
				if( ge2 == 9 && bai2 == 9 && shi2 ==9)
				{
					ge2 = 9;  bai2 = 9;  shi2 =9;
				}
			}
		}
	}

	if(K4 == 0)
	{			  
		delay_ms(1000);
		if(bai2 == 0 && shi2 ==0)
		{
			ge2 --;
			if(ge2 <= 0)
			{
				ge2 = 0;
			}
		}
		if(bai2 == 0 && shi2 > 0)
		{
			if(ge2 > 0)
			{
				ge2 --;
			}
			else
			{
				shi2 --;
				ge2 = 9;
			}
		}
		if(bai2 > 0)
		{
			if(ge2 > 0)
			{
				ge2 --;
			}
			else
			{
				if(shi2 > 0)
				{
					shi2 --;
					ge2 = 9;
				}
				else
				{
					bai2 --;
					shi2 = 9;
					ge2 = 9;
				}
				
			}
		}
			
	}

	if(K2 == 0)
	{
		ge1 = 0;
		shi1 = 0;
		bai1 = 0;
	}

	if(K5 == 0)
	{
		ge2 = 0;
		shi2 = 0;
		bai2 = 0;
	}

	if(K6 == 0)
	{
		ge1 = 0;
		shi1 = 0;
		bai1 = 0;
		ge2 = 0;
		shi2 = 0;
		bai2 = 0;
	}
		
	updata_display();
}