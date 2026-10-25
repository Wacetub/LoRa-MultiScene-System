#ifndef __MY_USART1_H
#define __MY_USART1_H
#include "sys.h"

#define USART1_GPIO_PIN_TX   GPIO_Pin_9
#define USART1_GPIO_PIN_RX   GPIO_Pin_10


void My_USART1(void);
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data);
void USART_SendString(USART_TypeDef * USARTx, char *str);
uint8_t USART_ReceiveByte(USART_TypeDef* USARTx);
#endif
