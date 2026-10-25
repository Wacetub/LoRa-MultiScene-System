/*条件编译*/
#ifndef __LED_H__
#define __LED_H__

#include "sys.h"
/*宏定义方便代码移植*/
#define LED_GPIO_PIN  GPIO_Pin_13 //引脚GPIO_Pin_x
#define LED_GPIO_PORT GPIOC       //那一个GPIOx
#define LED_GPIO_CLK RCC_APB2Periph_GPIOC  //哪一个GPIOx的时钟

#define LED1 PCout(13)

/*函数声明*/
void LED_GPIO_Config(void);
void LED_OFF(void);
void LED_NO(void);
void LED_Change(void);

#endif
