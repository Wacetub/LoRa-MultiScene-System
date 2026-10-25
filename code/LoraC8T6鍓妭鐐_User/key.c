#include "sys.h"

u8 key1,key2,key3,key4;//按键标志位

void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//配置时钟
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_2|GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//配置为输入模式
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}

/*按键扫描函数*/
void KEY_Scan()
{
	if(PAin(0) == 0)
	{
		delay_ms(5);
		if(PAin(0) == 0)
		{
			key4=1;
		}
		while(!PAin(0));
	}
	
	if(PAin(5) == 0)
	{
		delay_ms(5);
		if(PAin(5) == 0)
		{
			key3=1;
		}
		while(!PAin(5));
	}
	
	if(PAin(6) == 0)
	{
		delay_ms(5);
		if(PAin(6) == 0)
		{
			key2=1;
		}
		while(!PAin(6));
	}
	
	if(PAin(7) == 0)
	{
		delay_ms(5);
		if(PAin(7) == 0)
		{
			key1=1;
		}
		while(!PAin(7));
	}
}

u8 KEY_Scan_123(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return 1;
	}else if(KEY0==1)key_up=1; 	    
 	return 0;// 无按键按下
}

