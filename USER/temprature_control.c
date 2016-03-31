#include "stm32f10x_gpio.h"
#include <stdio.h>
void delay_ms(u16 nms);

void temprature_control(u16 temprature_volt_goal, u16 temprature_volt_now)

{
	u16 error =  2;// set error to 2
	u16 upper_limit = temprature_volt_goal+error;
	u16 lower_limit = temprature_volt_goal-error;

//	if (temprature_volt_now<lower_limit)
//	{
//		GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);
//		GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//		delay_ms(500);
//		GPIO_SetBits(GPIOF,GPIO_Pin_9);

//	}
//	if (temprature_volt_now>upper_limit)
//	{
//		GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);
//		GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//	};

	if (temprature_volt_now>lower_limit & temprature_volt_now<upper_limit)
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_9);
		//delay_ms(1);
		//printf("\n OFF__OK\n ");
		
	};
	if(temprature_volt_now<lower_limit)
	{
		//GPIO_SetBits(GPIOF,GPIO_Pin_9);
		Led_OFF(4);
		delay_ms(500);
		Led_light(4);
		//GPIO_ResetBits(GPIOF,GPIO_Pin_9);
	};
	if(temprature_volt_now>upper_limit)
	{
		//printf("\n OFF___HIGH \n");	
	}
}
