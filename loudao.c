#include<regx51.h>
#define uchar unsigned char
#define uint unsigned int

sbit k1 = P3^2;
sbit k2 = P3^3;
uint a = 0;
uint b = 0;
uchar code dsy_code[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00};
uchar code scan_bits[] = {0x20, 0x10, 0x08};
uchar data buffer_counts[] = {0, 0, 0};
uint dat_count = 0;

/* INT0初始化 */
void INT0_init (void)
{
	IT0 = 1;
	EA  = 1;
	EX0 = 1;
}

/* INT1初始化 */
void INT1_init (void)
{
	IT1 = 1;
	EA  = 1;
	EX1 = 1;
}

/* 外部中断0服务程序 */
void Init0 () interrupt 0 using 0
{
	EX0 = 0;
	dat_count ++;
	EX0 = 1;
}

/* 外部中断1服务程序 */
void Init1 () interrupt 2 using 1
{
	EX1 = 0;
	dat_count --;
	EX1 = 1;
}

/* 延时程序 */
void delayms(uint x)
{
	uchar t;
	while(x--)
	{
		for(t=0; t<200; t++);
	}
}

/* 显示程序 */
void show_counts()
{
	uint i;
	buffer_counts[2] = dat_count / 100;
	buffer_counts[1] = dat_count % 100 / 10;
	buffer_counts[0] = dat_count % 10;
	if(buffer_counts[2] == 0)
	{
		buffer_counts[2] = 0x0a;
		if(buffer_counts[1] == 0)
		{
			buffer_counts[1] = 0x0a;
		}
	}

	for(i=0; i<3; i++)
	{
		P2 = scan_bits[i];
		P1 = dsy_code[buffer_counts[i]];
		delayms(1);
	}
}


/* 主 */
void main()
{
	IT0 = 1;
	IT1 = 1;
	PX0 = 1;
	IE  = 0x85;
	
	while(1)
	{
		show_counts();
		if(k1 == 0)
		{
			delayms(5);
			if(k1 == 0)
			{
				a ++;
				if(b != 0)
				{
					dat_count ++;
					a = 0;
					b = 0;
				}
				while(!k1);
			}
		}
		else if(k2 == 0)
		{
			delayms(5);
			if(k2 == 0)
			{
				b ++;
				if(a != 0)
				{
					dat_count ++;
					a = 0;
					b = 0;
				}
				while(!k2);
			}
		}
	}
}
