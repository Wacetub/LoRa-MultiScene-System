/**************************
芯片：STM32F103C8T6

实现功能：低电平点亮LED

引脚：PC13
***************************/

#include "sys.h"
extern u8 Lora_mode;

int main(void)
{
	My_USART1(); //串口1初始化
	delay_init(); //滴答定时器初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_GPIO_Config();
	KEY_GPIO_Config();
	LoRa_Init();
	LoRa_Set();
		
	while(1)
	{
		
		LoRa_SendData(2,23,2);//发送地址，信道，以及问询帧
		delay_ms(500);
		LoRa_ReceData();
		LoRa_SendData(1,23,1);
		delay_ms(500);
		LoRa_ReceData();
	

		
	
}
}
