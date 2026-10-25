#include "sys.h"

/*GPIO初始化*/
void LED_GPIO_Config(void)
{
	//GPIO_InitTypeDef GPIO_InitStruct;  //示例化一个GPIO对象（必须在上面大括号下面）
	
	//配置时钟
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK,ENABLE);
	
	//配置引脚
	GPIO_InitTypeDef GPIO_InitStruct;  //示例化一个GPIO对象（必须在上面大括号下面）/（也可以直接放在这里）
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  //配置输出模式为推挽输出
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_PIN;        //选择配置引脚为13引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //配置输出速率
	
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);  //本函数意思为：配置寄存器（必须得写很重要）
}

/*开灯*/
void LED_OFF(void)
{
	//输出低电平
	GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN);//输出低电平点亮
}

/*关灯*/
void LED_NO(void)
{
	//输出高电平
	GPIO_SetBits(LED_GPIO_PORT, LED_GPIO_PIN);//输出高电平熄灭
}
/*LED状态改变函数*/
void LED_Change(void)
{
//	static u8 i =0;
//	if(flag == 1)
//	{
//		flag = 0;	
//		switch (i)
//		{
//			case 0:PCout(13) = 1;i++;break;
//			case 1:PCout(13) = 0;i=0;break;
//		}	
//	}	
}
