#include "stm32f10x_tim.h"   
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);

//  Using systick to dalay
  
static u8  fac_us=0;//us 
static u16 fac_ms=0;//ms


void delay_init(u8 SYSCLK)  // systick 8M *9 = 72M
{
SysTick->CTRL&=0xfffffffb;//?????? HCLK/8
// SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
fac_us=SYSCLK/8;      
fac_ms=(u16)fac_us*1000;
}            

//when the system running in 72M,Nms<=1864 
void delay_ms(u16 nms)
{         
u32 temp;     
SysTick->LOAD=(u32)nms*fac_ms;//load 
//SysTick_SetReload((u32)nms*fac_ms);
SysTick->VAL =0x00;           //clear 
//SysTick_CounterCmd(SysTick_Counter_Clear);
SysTick->CTRL=0x01 ;          //start 
//SysTick_CounterCmd(SysTick_Counter_Enable); 
do
{
  temp=SysTick->CTRL;
}
while(temp&0x01&&!(temp&(1<<16)));//time is up
SysTick->CTRL=0x00;       //clear 
SysTick->VAL =0x00;       //clear 
}   
