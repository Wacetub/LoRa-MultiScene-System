#include "lora.h"
#include "sys.h"
#include "delay.h"
#include "usart3.h"
#include "string.h"
#include "stdio.h"
#include "led.h"
#include "key.h"
#include "lora_cfg.h"
//#include "bh1750.h"

//Éè±¸¹¤×÷Ä£Ê½(ÓÃÓÚ¼ÇÂ¼Éè±¸×´Ì¬)
u8 Lora_mode=0;//0:ÅäÖÃÄ£Ê½ 1:½ÓÊÕÄ£Ê½ 2:·¢ËÍÄ£Ê½
static u8 Int_mode=0;//0:¹Ø±Õ 1:ÉÏÉıÑØ 2:ÏÂ½µÑØ

_LoRa_CFG LoRa_CFG=
{
	.addr = LORA_ADDR,       //Éè±¸µØÖ·
	.power = LORA_POWER,     //·¢Éä¹¦ÂÊ
	.chn = LORA_CHN,         //ĞÅµÀ
	.wlrate = LORA_RATE,     //¿ÕÖĞËÙÂÊ
	.wltime = LORA_WLTIME,   //Ë¯ÃßÊ±¼ä
	.mode = LORA_MODE,       //¹¤×÷Ä£Ê½
	.mode_sta = LORA_STA,    //·¢ËÍ×´Ì¬
	.bps = LORA_TTLBPS ,     //²¨ÌØÂÊÉèÖÃ
	.parity = LORA_TTLPAR    //Ğ£ÑéÎ»ÉèÖÃ
};


EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;


extern u8 N;
extern u8	P;
extern u8 K;
extern u8 H;
extern u8 T;
extern u8 PH;
extern u8 PH1;



void lora_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//Ìí¼Ó½áÊø·û
		printf("%s",USART3_RX_BUF);	//·¢ËÍµ½´®¿Ú
		if(mode)USART3_RX_STA=0;
	} 
}
//lora·¢ËÍÃüÁîºó,¼ì²â½ÓÊÕµ½µÄÓ¦´ğ
//str:ÆÚ´ıµÄÓ¦´ğ½á¹û
//·µ»ØÖµ:0,Ã»ÓĞµÃµ½ÆÚ´ıµÄÓ¦´ğ½á¹û
//ÆäËû,ÆÚ´ıÓ¦´ğ½á¹ûµÄÎ»ÖÃ(strµÄÎ»ÖÃ)
u8* lora_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//½ÓÊÕµ½Ò»´ÎÊı¾İÁË
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//Ìí¼Ó½áÊø·û
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//lora·¢ËÍÃüÁî
//cmd:·¢ËÍµÄÃüÁî×Ö·û´®(²»ĞèÒªÌí¼Ó»Ø³µÁË),µ±cmd<0XFFµÄÊ±ºò,·¢ËÍÊı×Ö(±ÈÈç·¢ËÍ0X1A),´óÓÚµÄÊ±ºò·¢ËÍ×Ö·û´®.
//ack:ÆÚ´ıµÄÓ¦´ğ½á¹û,Èç¹ûÎª¿Õ,Ôò±íÊ¾²»ĞèÒªµÈ´ıÓ¦´ğ
//waittime:µÈ´ıÊ±¼ä(µ¥Î»:10ms)
//·µ»ØÖµ:0,·¢ËÍ³É¹¦(µÃµ½ÁËÆÚ´ıµÄÓ¦´ğ½á¹û)
//       1,·¢ËÍÊ§°Ü
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART3->SR&0X40)==0);//µÈ´ıÉÏÒ»´ÎÊı¾İ·¢ËÍÍê³É  
		USART3->DR=(u32)cmd;
	}else u3_printf("%s\r\n",cmd);//·¢ËÍÃüÁî
	
	if(ack&&waittime)		//ĞèÒªµÈ´ıÓ¦´ğ
	{
	   while(--waittime)	//µÈ´ıµ¹¼ÆÊ±
	   { 
		  delay_ms(10);
		  if(USART3_RX_STA&0X8000)//½ÓÊÕµ½ÆÚ´ıµÄÓ¦´ğ½á¹û
		  {
			  if(lora_check_cmd(ack))break;//µÃµ½ÓĞĞ§Êı¾İ 
			  USART3_RX_STA=0;
		  } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
} 

//½«1¸ö×Ö·û×ª»»Îª16½øÖÆÊı×Ö
//chr:×Ö·û,0~9/A~F/a~F
//·µ»ØÖµ:chr¶ÔÓ¦µÄ16½øÖÆÊıÖµ
u8 chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}

//×Ö·û×ª³É16½øÖÆÊı¾İ
//test[] ´æ´¢Î»ÖÃ
//buffer[] Ğè×ª»»µÄÊı×é
//len£º ³¤¶È
void char2hex_buff(u8 test[],u8 buff[],u8 len)
{
	u8 i=0;
	for(i=0;i<len;i++)
	{
 		test[i] = chr2hex(buff[i]);
	}
}

//½«16½øÖÆÊı¾İ×ª»»³ÉÊı×Ö
u32 hexbuff_num(u8 test[],u8 buff[],u8 len)
{
	u8 i=0;
	u32 num=0; 
	
	for(i=0;i<len;i++) test[i] = chr2hex(buff[i]);
    if(len==5)  num = test[0]*10000+test[1]*1000+test[2]*100+test[3]*10+test[4];
	else if(len==4) num = test[0]*1000+test[1]*100+test[2]*10+test[3];		
	else if(len==3) num = test[0]*100+test[1]*10+test[2];		
	else if(len==2) num = test[0]*10+test[1];
	else if(len==1) num = test[0];
	
	return num;
}

//¼ÆËãÊı×ÖÎ»Êı
//Èç:100,3Î»Êı¡¢10,1Î»Êı
u8 num_count(u32 num)
{
	u32 date=0;
	u8 count=0;
	
	date = num;
	do
	{
		count++;
		date/=10;
	}while(date>0);
	
	return count;
}





//AUXÖĞ¶ÏÉèÖÃ
//mode:ÅäÖÃµÄÄ£Ê½ 0:¹Ø±Õ 1:ÉÏÉıÑØ 2:ÏÂ½µÑØ




void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//¹Ø±ÕÖĞ¶Ï
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //ÉÏÉıÑØ
	    else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//ÏÂ½µÑØ
		
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//¼ÇÂ¼ÖĞ¶ÏÄ£Ê½
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
    
}

//LORA_AUXÖĞ¶Ï·şÎñº¯Êı
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//ÉÏÉıÑØ(·¢ËÍ:¿ªÊ¼·¢ËÍÊı¾İ ½ÓÊÕ:Êı¾İ¿ªÊ¼Êä³ö)     
	   {
		  if(Lora_mode==1)//½ÓÊÕÄ£Ê½
		  {
			 USART3_RX_STA=0;//Êı¾İ¼ÆÊıÇå0
		  }
		  Int_mode=2;//ÉèÖÃÏÂ½µÑØ´¥·¢
		  LED1=0;//DS0ÁÁ
	   }
       else if(Int_mode==2)//ÏÂ½µÑØ(·¢ËÍ:Êı¾İÒÑ·¢ËÍÍê ½ÓÊÕ:Êı¾İÊä³ö½áÊø)	
	   {
		  if(Lora_mode==1)//½ÓÊÕÄ£Ê½
		  {
			 USART3_RX_STA|=1<<15;//Êı¾İ¼ÆÊı±ê¼ÇÍê³É
		  }else if(Lora_mode==2)//·¢ËÍÄ£Ê½(´®¿ÚÊı¾İ·¢ËÍÍê±Ï)
		  {
			 Lora_mode=1;//½øÈë½ÓÊÕÄ£Ê½
		  }
		  Int_mode=1;//ÉèÖÃÉÏÉıÑØ´¥·¢
          LED1=1;//DS0Ãğ		   
	   }
       Aux_Int(Int_mode);//ÖØĞÂÉèÖÃÖĞ¶Ï±ßÑØ
	   EXTI_ClearITPendingBit(EXTI_Line4); //Çå³ıLINE4ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
	}	
}

//LoRaÄ£¿é³õÊ¼»¯
//·µ»ØÖµ: 0,¼ì²â³É¹¦
//        1,¼ì²âÊ§°Ü
u8 LoRa_Init(void)
{
	 u8 retry=0;
	 u8 temp=1;
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//Ê¹ÄÜPA¶Ë¿ÚÊ±ÖÓ
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//Ê¹ÄÜ¸´ÓÃ¹¦ÄÜÊ±ÖÓ

     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//½ûÖ¹JTAG,´Ó¶øPA15¿ÉÒÔ×öÆÕÍ¨IOÊ¹ÓÃ,·ñÔòPA15²»ÄÜ×öÆÕÍ¨IO!!!	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LORA_MD0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO¿ÚËÙ¶ÈÎª50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //ÍÆÍìÊä³ö £¬IO¿ÚËÙ¶ÈÎª50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //ÏÂÀ­ÊäÈë
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO¿ÚËÙ¶ÈÎª50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //¸ù¾İÉè¶¨²ÎÊı³õÊ¼»¯GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //ÉÏÉıÑØ´¥·¢
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //ÖĞ¶ÏÏß¹Ø±Õ(ÏÈ¹Ø±ÕºóÃæÔÙ´ò¿ª)
  	 EXTI_Init(&EXTI_InitStructure);//¸ù¾İEXTI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèEXTI¼Ä´æÆ÷
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//ÇÀÕ¼ÓÅÏÈ¼¶2£¬ 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//×ÓÓÅÏÈ¼¶3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //¹Ø±ÕÍâ²¿ÖĞ¶ÏÍ¨µÀ£¨ºóÃæÔÙ´ò¿ª£©
   	 NVIC_Init(&NVIC_InitStructure); 
	 
	 LORA_MD0=0;
	 LORA_AUX=0;
	
	 while(LORA_AUX)//È·±£LORAÄ£¿éÔÚ¿ÕÏĞ×´Ì¬ÏÂ(LORA_AUX=0)
	 {
//		 Show_Str(40+30,50+20,200,16,"Ä£¿éÕıÃ¦,ÇëÉÔµÈ!!",16,0); 	
//		 delay_ms(500);
//		 Show_Str(40+30,50+20,200,16,"                    ",16,0);
         delay_ms(100);		 
	 }
	 usart3_init(115200);//³õÊ¼»¯´®¿Ú3
	 
	 LORA_MD0=1;//½øÈëATÄ£Ê½
	 delay_ms(40);
	 retry=3;
	 while(retry--)
	 {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//¼ì²â³É¹¦
			 break;
		 }	
	 }
	 if(retry==0) temp=1;//¼ì²âÊ§°Ü
	 return temp;
}

//LoraÄ£¿é²ÎÊıÅäÖÃ
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	usart3_init(115200);//½øÈëÅäÖÃÄ£Ê½Ç°ÉèÖÃÍ¨ĞÅ²¨ÌØÂÊºÍĞ£ÑéÎ»(115200 8Î»Êı¾İ 1Î»Í£Ö¹ ÎŞÊı¾İĞ£Ñé£©
	usart3_rx(1);//¿ªÆô´®¿Ú3½ÓÊÕ
	
	while(LORA_AUX);//µÈ´ıÄ£¿é¿ÕÏĞ
	LORA_MD0=1; //½øÈëÅäÖÃÄ£Ê½
	delay_ms(40);
	Lora_mode=0;//±ê¼Ç"ÅäÖÃÄ£Ê½"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//ÉèÖÃÉè±¸µØÖ·
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//ÉèÖÃĞÅµÀºÍ¿ÕÖĞËÙÂÊ
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//ÉèÖÃ·¢Éä¹¦ÂÊ
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//ÉèÖÃ¹¤×÷Ä£Ê½
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//ÉèÖÃ·¢ËÍ×´Ì¬
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//ÉèÖÃË¯ÃßÊ±¼ä
	lora_send_cmd(sendbuf,"OK",50);
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//ÉèÖÃ´®¿Ú²¨ÌØÂÊ¡¢Êı¾İĞ£ÑéÎ»
	lora_send_cmd(sendbuf,"OK",50);
      
	LORA_MD0=0;//ÍË³öÅäÖÃ,½øÈëÍ¨ĞÅ
	delay_ms(40);
	while(LORA_AUX);//ÅĞ¶ÏÊÇ·ñ¿ÕÏĞ(Ä£¿é»áÖØĞÂÅäÖÃ²ÎÊı)
	USART3_RX_STA=0;
	Lora_mode=1;//±ê¼Ç"½ÓÊÕÄ£Ê½"
	usart3_init(9600);//·µ»ØÍ¨ĞÅ,¸üĞÂÍ¨ĞÅ´®¿ÚÅäÖÃ(²¨ÌØÂÊ¡¢Êı¾İĞ£ÑéÎ»)
	Aux_Int(1);//ÉèÖÃLORA_AUXÉÏÉıÑØÖĞ¶Ï	
	printf("ÒÑ¾­ÉèÖÃÍê³É");
}

u8 Dire_Date[30]={0};//¶¨Ïò´«ÊäÊı¾İ
u8 date[40]={0};//¶¨ÏòÊı×é
u8 Tran_Data[30]={0};//Í¸´«Êı×é
u8 wlcd_buff[10]={0}; //LCDÏÔÊ¾×Ö·û´®»º³åÇø
u8 test[30]={0};
//LoraÄ£¿é·¢ËÍÊı¾İ
void LoRa_SendData(void)
{      
	static u8 num=0;
    u16 addr;
	u8 chn;
	u16 i=0; 
		
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//Í¸Ã÷´«Êä
	{
		sprintf((char*)Tran_Data,"%d %d %d %d %d ",H,T,N,P,K);
		u3_printf("%s\r\n",Tran_Data);
	
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//¶¨Ïò´«Êä
	{
		
		addr = (u16)0;//Ä¿±êµØÖ·
		chn = 23;//Ä¿±êĞÅµÀ	
		date[i++] =(addr>>8)&0xff;//¸ßÎ»µØÖ·
		date[i++] = addr&0xff;//µÍÎ»µØÖ·
		date[i] = chn;//ÎŞÏßĞÅµÀ
		date[3] = 2;//ÎŞÏßĞÅµÀ
		date[4] = H;//ÎŞÏßĞÅµ
		date[5] = T;//ÎŞÏßĞÅµ
		date[6] = N;//ÎŞÏßĞÅµ
		date[7] = P;//ÎŞÏßĞÅµ
		date[8] = K;//ÎŞÏßĞÅµ
          date[9] = PH;	
		for(i=0;i<(8+3);i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
			USART_SendData(USART3,date[i]); 
		}	
		
        //½«Ê®Áù½øÖÆµÄÊı¾İ×ª»¯Îª×Ö·û´®´òÓ¡ÔÚlcd_buffÊı×é
		sprintf((char*)wlcd_buff,"%x %x %x %x %x %x %x %x %x %x  ",
				date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7],date[8],date[9]);

		
	}
			
}

u8 rlcd_buff[10]={0}; //LCDÏÔÊ¾×Ö·û´®»º³åÇø
//LoraÄ£¿é½ÓÊÕÊı¾İ
void LoRa_ReceData(void)
{
    u16 i=0;
    u16 len=0;
   
	//ÓĞÊı¾İÀ´ÁË
	if(USART3_RX_STA&0x8000)
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//Ìí¼Ó½áÊø·û
		USART3_RX_STA=0;

		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
			USART_SendData(USART1,USART3_RX_BUF[i]); 
		}


			//½«Ê®Áù½øÖÆµÄÊı¾İ×ª»¯Îª×Ö·û´®´òÓ¡ÔÚlcd_buffÊı×é
//			sprintf((char*)rlcd_buff,"%d %d %d %d %d %d %d %d %d %d %d",
//			USART3_RX_BUF[0],USART3_RX_BUF[1],USART3_RX_BUF[2],USART3_RX_BUF[3],USART3_RX_BUF[4],USART3_RX_BUF[5],USART3_RX_BUF[6],USART3_RX_BUF[7],USART3_RX_BUF[8],USART3_RX_BUF[9],USART3_RX_BUF[10]);
				
			if(USART3_RX_BUF[0]==0x01)
			{				

                LoRa_SendData();
				USART_SendData(USART1,3); 
			}
		}
		memset((char*)USART3_RX_BUF,0x00,len);//´®¿Ú½ÓÊÕ»º³åÇøÇå0

	}


