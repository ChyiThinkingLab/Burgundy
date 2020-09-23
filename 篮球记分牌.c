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
/******************** IO配置函数 **************************/
void	LED_GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;	
	GPIO_InitStructure.Mode = GPIO_OUT_PP;
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;		
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;		
	GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);	
}

/******************** IO配置函数 **************************/
void GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		
	
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin  = GPIO_Pin_All;
	GPIO_Inilize(GPIO_P7, &GPIO_InitStructure);	
}

/************************ 定时器配置 ****************************/
void Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	
/*	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 2000);		//初值2K
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0, &TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
*/
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//指定时钟源, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 12 / 100);		//初值// 100Hz
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer1, &TIM_InitStructure);				//初始化Timer1	  Timer0,Timer1,Timer2

	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE. (注意: Timer2固定为16位自动重装, 中断固定为低优先级)
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 12 / 50);		//初值// 50Hz
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer2, &TIM_InitStructure);				//初始化Timer2	  Timer0,Timer1,Timer2
	EA = 1;//开总中断
}


/******************** 主函数 **************************/
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

/********************* Timer0中断函数************************/
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

/********************* Timer1中断函数************************/
void timer1_int (void) interrupt TIMER1_VECTOR
{
	timer1_cnt_fun();//10ms

}

/********************* Timer2中断函数************************/
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