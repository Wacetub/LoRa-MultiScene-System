#ifndef  __DHT11_H__
#define  __DHT11_H__
#include "stdint.h"

#define  DHT11  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define  DHT11_HIGH  GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define  DHT11_LOW  GPIO_ResetBits(GPIOA,GPIO_Pin_4)

void DHT11_PortOutput(void);
void DHT11_PortInput(void);
uint8_t DHT_Read_Byte(void);
uint8_t DHT_Read(void);

#endif
