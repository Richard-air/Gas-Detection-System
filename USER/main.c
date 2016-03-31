#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include <stdio.h>
#include "ff.h"
#include "diskio.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "sdcard.h"

#define temprature_goal  60
SD_Error States;
FATFS fs;
FRESULT res;
FIL file;				
UINT br;
BYTE buffer[4096];
u16 ADCConvertedValueLocal, Precent = 0;
u16 percent=0; 
u16 nn=0;
u16 record_flag=0;
u16 carry_flag=0;
u16 inject_flag=0;

void ADC_GPIO_Configuration(void);
void Delay_(__IO uint32_t nCount);
void GPIO_config(void);
void COM_init(void);
void delay_init(u8 );
void delay_ms(u16 nms);
void sample_inject(void);
void n2_carry(void);
void record_data(void);
void EXTI_PA0_Config(void);



#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	USART1->SR;
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}return ch;
}
unsigned char ReadKeyDown(void)
{
  /* 1 key is pressed */ //user2
  if(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3))
  {
	return 1; 
  }	
  /* 2 key is pressed */ //user1
  if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))
  {
	return 2; 
  }
  /* 3 key is pressed */ //tamper
  if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
  {
	return 3; 
  }
  /* 4 key is pressed */ //wakeup
  if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
  {

	return 4; 
  }
  /* No key is pressed */
  else 
  {
    return 0;
  }
}
void main_init(void)
{  
	ADC_GPIO_Configuration();
	GPIO_config();
	delay_init(72);
	COM_init();
	EXTI_PA0_Config();
	disk_initialize (0);
	f_mount(0, &fs);
}

int main(void)

{
	main_init();	
	DMA_Cmd(DMA1_Channel1,ENABLE);ADC_Cmd(ADC1, ENABLE);
	res = f_open( &file , "0:/record.txt" , FA_OPEN_ALWAYS | FA_WRITE); 
	if(res)
		printf("File open failed.");
	  printf("init fisished.");
	while (1)
  {				
		percent = (ADCConvertedValueLocal*100/0x1000);
		Precent = (ADCConvertedValueLocal*10000/0x1000); 
		//printf("percent: %d\n",percent);
		record_data();
		if(ReadKeyDown()==2)//user1 key
		{
			carry_flag=1;
		}
		if(ReadKeyDown()==1)//user2 key
		{
			inject_flag=1;
		}
		if(ReadKeyDown()==3) //TAMTER KEY FINISH FILE
		{
			record_flag=0;
			f_close(&file);
			printf("nn=%d",nn);
		}
		temprature_control(60,percent);
		sample_inject();
		n2_carry();
	}

		}
		