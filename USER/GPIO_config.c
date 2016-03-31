//This file configs the GPIO.
//Using lib of ST.
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include <stdio.h>
#include "ff.h"
#include "stm32_eval_sdio_sd.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
void GPIO_config(void)
{
	//////////GPIO-F OUTPUT
	GPIO_InitTypeDef GPIO_InitStructure; //Init cofig structure
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); //enable GPIO-F clk
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5; //These pins are used as OUTPUT for valve and pump.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //PUSH-PULL OUTPUT.TO DRIVE the pumps and valve.
	GPIO_Init(GPIOF,&GPIO_InitStructure); //init GPIO-F
	
	///////////////////////////
	////////GPIO F LED
	///////6-LED DS1, 7-DS2, 8-DS3,9 DS4.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9; //PINS to config.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //push-pull mode to light LED
	GPIO_Init(GPIOF,&GPIO_InitStructure);//init
	GPIO_SetBits(GPIOF,GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8| GPIO_Pin_9);//pin=0 will light LED
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//keys
	
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_GPIOC| RCC_APB2Periph_GPIOD,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_8;                     
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                     
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	    GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                     
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	    GPIO_Init(GPIOD, &GPIO_InitStructure);
			
			//SDIO
			
			  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

  /* Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure PD.02 CMD line */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
		
}
