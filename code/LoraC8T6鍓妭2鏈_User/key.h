/*条件编译*/
#ifndef __KEY_H__
#define __KEY_H__

#include "sys.h"

#define KEY_GPIO_PIN   GPIO_Pin_0
#define KEY_GPIO_PORT  GPIOA
#define KEY_GPIO_CLK   RCC_APB2Periph_GPIOA
#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键0

void KEY_GPIO_Config(void);
void KEY_Scan(void);
u8 KEY_Scan_123(u8 mode);
extern u8 flag;

#endif

