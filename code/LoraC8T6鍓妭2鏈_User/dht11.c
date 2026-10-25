#include "stm32f10x.h"
#include "dht11.h"
#include "delay.h"

uint8_t dat[5]={0x00,0x00,0x00,0x00,0x00};    //存储读取的温湿度信息
uint32_t sum=0;         //存放校验时的求和

/****************************************
设置PB12端口为输出
*****************************************/
void DHT11_PortOutput(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;

 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;//推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//输出50MHz
 GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/****************************************
设置PB12端口为输入
*****************************************/
void DHT11_PortInput(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;

 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//输出50MHz
 GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/****************************************
函数名称：void DHT_Read_Byte()
函数功能：对DHT的数据中的一个字节读取函数
输入参数：无
返回值：无
*****************************************/
uint8_t DHT_Read_Byte(void)
{
  uint8_t temp;      //存放读取到的位数据
	uint8_t ReadDat = 0;
  uint8_t i;
	uint8_t retry = 0;
  for(i=0;i<8;i++)
  {
    while(DHT11 ==0 && retry < 100)     //等待DHT11输出高电平
		{
			delay_us(1);
			retry++;
		}
		retry = 0;
    delay_us(30);          //延时30us，由于‘0’代码高电平时间26~28us，'1'代码高电平时间70us,延时30us可判断出是1,还是0
    temp = 0;             //先将寄存器清零
    if(DHT11 ==1)        //延时30us之后如果还是高电平，证明为1代码
      temp = 1;            //将1存储
    while(DHT11 ==1 && retry < 100)      //等待信号被拉低，跳出
		{
			delay_us(1);
			retry++;			
		}
		retry = 0;
    ReadDat<<=1;           //数据左移1位，存放新得到的数据
    ReadDat|= temp;         //新得到的数据放到最后1位
  }
	return ReadDat;
}

/*********************************************
函数名称：unsigned char DHT_Read()
函数功能：读取DHT11的温湿度
输入参数：无
返回值： flag--数据读取、校验成功标志
*********************************************/
uint8_t DHT_Read(void)
{
  uint8_t i;
	uint8_t retry = 0;
  DHT11_PortOutput();           //端口方向设置为输出
  DHT11_LOW;           //端口数据拉低
  delay_ms(18);          //延时18ms，时序要求
  DHT11_HIGH;           //端口数据拉高
  delay_us(40);          //延时40us,
  DHT11_PortInput();           //方向设置为输入
  delay_us(20);          //延时20us
  if(DHT11 ==0)          //如果读取到低电平，证明DHT11响应
  {
    while(DHT11 ==0 && retry < 100)  //等待变高电平
		{
			delay_us(1);
			retry++;				
		}
		retry = 0;
    while(DHT11 ==1 && retry < 100) //等待变低电平
		{
			delay_us(1);
			retry++;	
		}
		retry = 0;
    for(i=0;i<5;i++)    //循环5次将40位读出
     {
      dat[i] = DHT_Read_Byte();    //读出1个字节
     }
    delay_us(50);         //最后延时等待50us
  }
  sum=dat[0]+dat[1]+dat[2]+dat[3];     //前4个字节数据的和
  if(dat[4]==(u8)(sum))     //前4个数据和的末8位要和第5个数据相等，才算读取正确
    {
      return 1;                  //校验正确，返回1
    }
  else
     return 0;                  //校验错误，返回0
}




