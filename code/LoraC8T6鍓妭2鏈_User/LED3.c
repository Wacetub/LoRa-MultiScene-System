#include "sys.h"

extern u8 key1,key2,key3;
/*GPIO初始化*/
void LED3_GPIO_Config(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct;  //示例化一个GPIO对象（必须在上面大括号下面）
	
	//配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//配置引脚
	GPIO_InitTypeDef GPIO_InitStruct;  //示例化一个GPIO对象（必须在上面大括号下面）/（也可以直接放在这里）
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //配置输出模式为推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8;        //选择配置引脚为568引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //配置输出速率
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);  //本函数意思为：配置寄存器（必须得写很重要）
}

/*红灯开灯*/
void RED_LED_ON(void)
{
	//输出低电平
	GPIO_ResetBits(GPIOB,GPIO_Pin_5 );//输出低电平点亮
}
/*红灯关灯*/
void RED_LED_OFF(void)
{
	//输出高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_5);//输出高电平熄灭
}


/*绿灯开灯*/
void GREEN_LED_ON(void)
{
	//输出低电平
	GPIO_ResetBits(GPIOB,GPIO_Pin_6 );//输出低电平点亮
}
/*绿灯关灯*/
void GREEN_LED_OFF(void)
{
	//输出高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_6);//输出高电平熄灭
}

/*黄灯开灯*/
void YELLOW_LED_ON(void)
{
	//输出低电平
	GPIO_ResetBits(GPIOB,GPIO_Pin_8 );//输出低电平点亮
}
/*黄灯关灯*/
void YELLOW_LED_OFF(void)
{
	//输出高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_8);//输出高电平熄灭
}

/*红灯状态翻转*/
void RED_LED_Change(void)
{
	static u8 i =0;
	if(key1 == 1)
	{
		key1 = 0;	
		switch (i)
		{
			case 0:PBout(5) = 1;i++;break;
			case 1:PBout(5) = 0;i=0;break;
		}	
	}	
}

/*绿灯状态翻转*/
void GREEN_LED_Change(void)
{
	static u8 i =0;
	if(key2 == 1)
	{
		key2 = 0;	
		switch (i)
		{
			case 0:PBout(6) = 1;i++;break;
			case 1:PBout(6) = 0;i=0;break;
		}	
	}	
}

/*黄灯状态翻转*/
void YELLOW_LED_Change(void)
{
	static u8 i =0;
	if(key3 == 1)
	{
		key3 = 0;	
		switch (i)
		{
			case 0:PBout(8) = 1;i++;break;
			case 1:PBout(8) = 0;i=0;break;
		}	
	}	
}

