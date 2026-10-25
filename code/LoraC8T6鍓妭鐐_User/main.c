/**************************
芯片：STM32F103C8T6

实现功能：低电平点亮LED

引脚：PC13
***************************/

#include "sys.h"
#include "rs485.h"
extern u8 Lora_mode;
u8 N=2,P=3,K=4;
u8  H=7,T=6,PH=5,PH1;
void   for_function(void)
{
	

	u8 RS485_TX_TEMP_N_BUF[8]={0x01,0x03,0x00,0x1E,0x00,0x01,0xE4,0x0C}; //环境温湿度发送缓冲.//的自己改问询帧
	u8 RS485_TX_TEMP_P_BUF[8]={0x01,0x03,0x00,0x1F,0x00,0x01,0xB5,0xCC}; 
	u8 RS485_TX_TEMP_K_BUF[8]={0x01,0x03,0x00,0x20,0x00,0x01,0x85,0xC0};
	u8 RS485_TX_TEMP_HUM_BUF[8]={0x02,0x03,0x00,0x00,0x00,0x02,0xC4,0x38}; //环境温湿度发送缓冲.//的自己改问询帧
	u8 RS485_TX_TEMP_PH_BUF[8]={0x03,0x03,0x00,0x00,0x00,0x01,0x85,0xE8}; //环境温湿度发送缓冲.//的自己改问询帧
	
		RS485_Send_Data(RS485_TX_TEMP_HUM_BUF,8);//发送温湿度问询帧
		delay_ms(100);
		if(RS485_RX_BUF[0]==0x02){              //判断返回帧
		T=((RS485_RX_BUF[5]<<8)|(RS485_RX_BUF[6]))/10.0;
	     delay_ms(100);	
	     H=((RS485_RX_BUF[3]<<8)|(RS485_RX_BUF[4]))/10.0;
		delay_ms(100);		

		}
			
	     RS485_Send_Data(RS485_TX_TEMP_N_BUF,8);//发送土壤传感器-氮-问询帧
		delay_ms(100);
		if(RS485_RX_BUF[0]==0x01){          //判断返回帧
		N=((RS485_RX_BUF[3]<<8)|(RS485_RX_BUF[4]));
		delay_ms(100);
		
		}
		
		RS485_Send_Data(RS485_TX_TEMP_P_BUF,8);//发送土壤传感器-磷-问询帧
		delay_ms(100);
		if(RS485_RX_BUF[0]==0x01){           //判断返回帧
	     P=((RS485_RX_BUF[3]<<8)|(RS485_RX_BUF[4]));
		delay_ms(100);
		
		}
		
		RS485_Send_Data(RS485_TX_TEMP_K_BUF,8);//发送土壤传感器-钾-问询帧
		delay_ms(100);
		if(RS485_RX_BUF[0]==0x01){          //判断返回帧
		K=((RS485_RX_BUF[3]<<8)|(RS485_RX_BUF[4]));
	     delay_ms(10);
		
		}
		
		RS485_Send_Data(RS485_TX_TEMP_PH_BUF,8);//发送土壤传感器-钾-问询帧
		delay_ms(100);
		if(RS485_RX_BUF[0]==0x03){          //判断返回帧
		PH=((RS485_RX_BUF[3]<<8)|(RS485_RX_BUF[4]));
		PH1=PH*1;
	     delay_ms(10);
		
		}
		
		
		
		

}
int main(void)
{
	My_USART1(); //串口1初始化
	delay_init(); //滴答定时器初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	LED_GPIO_Config();
	KEY_GPIO_Config();
	LoRa_Init();
	LoRa_Set();
	RS485_Init(4800);	
		
	while(1)
	{
		for_function();
	     delay_ms(500);
		LoRa_ReceData();

		
	
}
}
