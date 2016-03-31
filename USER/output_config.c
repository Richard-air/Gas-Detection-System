//this file contains functions to output.
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include <stdio.h>
#include "ff.h"
#include "stm32_eval_sdio_sd.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "v_extern.h"

void delay_ms(u16 nms);
void output_gpio_bit(u16 num,u16 state);


void Led_light(unsigned char LED_NUM)
{
	switch(LED_NUM)
	{
        case 4:
          GPIO_ResetBits(GPIOF,GPIO_Pin_9);  /*点亮DS4灯*/
          break;
        case 3:
          GPIO_ResetBits(GPIOF,GPIO_Pin_8);  /*点亮DS3灯*/
          break;
        case 2:
          GPIO_ResetBits(GPIOF,GPIO_Pin_7);  /*点亮DS2灯*/
          break;
        case 1:
          GPIO_ResetBits(GPIOF,GPIO_Pin_6);  /*点亮DS1灯*/
          break;          
        default:
		  GPIO_SetBits(GPIOF,GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9); /*关掉所有的灯*/
          break;
	}
}


void Led_OFF(unsigned char LED_NUM)
	{
	switch(LED_NUM)
	{
        case 4:
          GPIO_SetBits(GPIOF,GPIO_Pin_9);  /*点亮DS4灯*/
          break;
        case 3:
          GPIO_SetBits(GPIOF,GPIO_Pin_8);  /*点亮DS3灯*/
          break;
        case 2:
          GPIO_SetBits(GPIOF,GPIO_Pin_7);  /*点亮DS2灯*/
          break;
        case 1:
          GPIO_SetBits(GPIOF,GPIO_Pin_6);  /*点亮DS1灯*/
          break;          
        default:
		  GPIO_SetBits(GPIOF,GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9); /*关掉所有的灯*/
          break;
	}
}


void n2_carry(void)
{
	if(carry_flag){output_gpio_bit(1,1);Led_light(1);}// n2 carry on
}

void sample_inject(void)
{
	
	if(inject_flag)
	{
	printf("\n INJECT");
	output_gpio_bit(1,1); // n2 carry on  valve 1 ,constant
	output_gpio_bit(2,1); // n2 side-way valve 2,  only in injection
	
	output_gpio_bit(3,0);
	output_gpio_bit(4,1);
	output_gpio_bit(5,1); // sample pump  on
	Led_light(3);
	delay_ms(1000); //sample injection time 2s
	delay_ms(1000); 
	delay_ms(1000); 
		
	Led_OFF(3);
	output_gpio_bit(3,1);// n2 not using side-way
	output_gpio_bit(2,0); // n2 side-way valve 2 OFF
	output_gpio_bit(4,1);// n2 with sample go throuth
	inject_flag=0;
	}
	
}	





void output_gpio_bit(u16 num,u16 state)
	
{
	switch(num)
	{
		case 1:
			if(state)
			GPIOF->BSRR = GPIO_Pin_1;
			else 
			GPIOF->BRR = GPIO_Pin_1;
		break;
		case 2:
			if(state)
			GPIOF->BSRR = GPIO_Pin_2;
			else 
			GPIOF->BRR = GPIO_Pin_2;
		break;
		case 3:
			if(state)
			GPIOF->BSRR = GPIO_Pin_3;
			else 
			GPIOF->BRR = GPIO_Pin_3;
		break;
		case 4:
			if(state)
			GPIOF->BSRR = GPIO_Pin_4;
			else 
			GPIOF->BRR = GPIO_Pin_4;
		break;
		case 5:
			if(state)
			GPIOF->BSRR = GPIO_Pin_5;
			else 
			GPIOF->BRR = GPIO_Pin_5;
		break;
	}	
	
}
	
