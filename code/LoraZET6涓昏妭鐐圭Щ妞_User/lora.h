#ifndef __LORA_H
#define __LORA_H
#include "sys.h"
#include "lora_cfg.h"
#define LORA_AUX  PAin(4)    //LORA模块状态引脚
#define LORA_MD0  PAout(15)  //LORA模块控制引脚

extern _LoRa_CFG LoRa_CFG;
extern u8 Lora_mode;

u8 LoRa_Init(void);
void Aux_Int(u8 mode);
void LoRa_Set(void);
void LoRa_SendData(u16 addr,u8 addrs,u8 sta);
void LoRa_ReceData(void);
void LoRa_Process(void);
void Lora_Test(void);


#endif

