#include "led_display.h"
#include "delay.h"
#include "string.h"



static void delay(void)
{
	unsigned char i = 250;
	
	while(i--)
	{
		;
	}
}

void Display_16_16(unsigned char *str)
{
	unsigned char i, m;
	
	//上半屏
	for(m=0; m<8; m++)
	{
		//送入列数据
		for(i=0; i<16; i++)
		{
			if(str[15 - i] & (0x01 << m))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}
			
			S_CLK = 0;
			S_CLK = 1;
		}
		S_LSTB = 1;
		S_LSTB = 0;
		
		//开起行是能
		for(i=0; i<16; i++)
		{
			if((0x0001 << m) & (0x0001 << i))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}
			
			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;
		
		delay();
		delay();
		delay();
		delay();
		
		//消隐
		S_DATA = 1;
		for(i=0; i<16; i++)
		{
			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;
	}
	
	//下半屏
	for(m=0; m<8; m++)
	{
		//送入列数据
		for(i=0; i<16; i++)
		{
			if(str[15 - i + 16] & (0x01 << m))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}
			
			S_CLK = 0;
			S_CLK = 1;
		}
		S_LSTB = 1;
		S_LSTB = 0;
		
		//开起行是能
		for(i=0; i<16; i++)
		{
			if((0x0100 << m) & (0x0001 << i))
			{
				S_DATA = 0;
			}
			else
			{
				S_DATA = 1;
			}
			
			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;
		
		delay();
		delay();
		delay();
		delay();
		
		//消隐
		S_DATA = 1;
		for(i=0; i<16; i++)
		{
			S_CLK = 0;
			S_CLK = 1;
		}
		S_HSTB = 1;
		S_HSTB = 0;
	}
}

void L_Display(unsigned char str[][16], unsigned char speed, unsigned char cnt)
{
	unsigned char temp[32];
	unsigned char j, offset;
	unsigned char speed_cnt;
	
	for(j=0; j<cnt; j++)
	{
		offset = 2 * j;
	
			memcpy(&temp[0], &str[offset][0], 16);
			
			memcpy(&temp[16], &str[offset + 1][0], 16);
			
			speed_cnt = speed;
			while(speed_cnt--)
			{
				Display_16_16(temp);
			}
	}
	
	speed_cnt = speed * 10;
	while(speed_cnt--)
	{
		Display_16_16(temp);
	}
}

