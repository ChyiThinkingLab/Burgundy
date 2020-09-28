#include "config.h"
#include "GPIO.h"
#include "delay.h"
#include "timer.h"
#include "led_display.h"

sbit	K0 = P3^2;	//int
sbit	K1 = P3^3;	//int
sbit	K2 = P3^4;	
sbit	K3 = P5^0;
sbit	K4 = P5^1;	
sbit	K5 = P3^5;
sbit	K6 = P3^6;
unsigned char timer0_cnt = 0;

/*************	功能说明	**************

本程序演示点阵屏

******************************************/

const unsigned char code XJN[][16]={

{0x10,0x0C,0x24,0xA4,0xA4,0xA4,0xA5,0xA6,0xA4,0xA4,0xA4,0xA4,0x24,0x14,0x0C,0x00},
{0x40,0x40,0x40,0x5F,0x4A,0x4A,0x4A,0x4A,0x4A,0x4A,0x4A,0x5F,0x40,0x40,0x40,0x00},/*"宣",0*/


{0x80,0x80,0x40,0x20,0x50,0x48,0x44,0xC3,0x44,0x48,0x50,0x20,0x40,0x80,0x80,0x00},
{0x40,0x40,0x42,0x4A,0x72,0x42,0x42,0x7F,0x42,0x42,0x62,0x5A,0x42,0x40,0x40,0x00},/*"金",0*/



{0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0xFE,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00},
{0x80,0x84,0x44,0x44,0x24,0x14,0x0C,0x07,0x04,0x04,0x44,0x84,0x44,0x3C,0x00,0x00},/*"男",0*/
};

const unsigned char code QXW[][16]={
{0x00,0x04,0x84,0x84,0x4C,0x54,0x25,0x26,0x24,0x54,0x4C,0x84,0x84,0x04,0x00,0x00},
{0x01,0x01,0x80,0x60,0x1F,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x01,0x01,0x00},/*"齐",0*/



{0x00,0x08,0xC8,0x38,0x8F,0x08,0x28,0x10,0xFC,0x4B,0x48,0xF9,0x4A,0x48,0x08,0x00},
{0x04,0x23,0x70,0x2C,0x23,0x28,0x70,0x00,0xFF,0x22,0x22,0x3F,0x22,0x22,0x20,0x00},/*"雄",0*/

{0x00,0x80,0x60,0xF8,0x07,0x08,0x48,0x48,0x48,0xFF,0x48,0x48,0x48,0x48,0x08,0x00},
{0x01,0x00,0x00,0xFF,0x00,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x12,0x22,0x1E,0x00},/*"伟",0*/
};

const unsigned char code NO[][16]={
{0x40,0x30,0xEF,0x24,0x64,0x48,0x48,0x7F,0x48,0x48,0x48,0x7F,0x48,0x48,0x40,0x00},
{0x01,0x01,0x7F,0x21,0x11,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00,0x00},/*"错",0*/
};

const unsigned char code WU[][16]={
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"错",0*/
};
/*{0x24,0x24,0xA4,0xFE,0xA3,0x22,0x00,0x22,0xCC,0x00,0x00,0xFF,0x00,0x00,0x00,0x00},
{0x08,0x06,0x01,0xFF,0x00,0x01,0x04,0x04,0x04,0x04,0x04,0xFF,0x02,0x02,0x02,0x00},/*"科",6*/
/* (16 X 16 , 宋体 )*/






/******************** IO配置函数 **************************/
void	LED_GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	
	GPIO_InitStructure.Pin  = GPIO_Pin_7;		//指定要初始化的IO, GPIO_Pin_7, 或操作
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);	//初始化
	
	GPIO_InitStructure.Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;		//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);	//初始化
}

/******************** IO配置函数 **************************/
void	GPIO_config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	
	//推完输出
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_6;//指定要初始化的IO, GPIO_Pin_0, 2, 4, 6: 点阵L_STB, H_STB, DATA, CLK
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);	//初始化
}

/************************ 定时器配置 ****************************/
void Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 2000);		//初值1K
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0, &TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2

	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;			//指定时钟源, TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000);		//初值//500
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer1, &TIM_InitStructure);				//初始化Timer1	  Timer0,Timer1,Timer2

	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE. (注意: Timer2固定为16位自动重装, 中断固定为低优先级)
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 12 / 1000);		//初值//10ms 0.5K
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer2, &TIM_InitStructure);				//初始化Timer2	  Timer0,Timer1,Timer2
}


/******************** 主函数 **************************/
void main(void)
{
	LED_GPIO_config();//初始化P3.7、P4.1、P4.2、P4.3
	GPIO_config();
	Timer_config();
	EA = 1;//开总中断

	while(1)
	{
		P37 = 0;
		delay_ms(100);
		P37 = 1;
		delay_ms(100);
		
	if(K0==0)
	{ 
	  K0=0;
	  delay_ms(100);
	  if(K6==0)
     	{
         
		 K6=0;	
	     L_Display(XJN, 8, 4);
		}
	else
	   {
	     L_Display(NO, 8, 2);
	   }
	 }
	if(K1==0)
	{ 
	  K1=0;
	  delay_ms(100);
	  if(K5==0)
     	{
         
		 K5=0;	
	     L_Display(QXW, 8, 4);
		}
	else
	   {
	     L_Display(NO, 8, 2);
	   }
	 }
	 L_Display(WU, 8, 2);   
	}
}




