
#include "stm32f10x.h"
#include <stdio.h>

#define DR_ADDRESS                  ((uint32_t)0x4001244C) //ADC1 DR寄存器基地址
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
void usart_send(void);

void STM32_Shenzhou_COMInit(USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(USART1, USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}
void usart_send(void)
{
USART_InitTypeDef USART_InitStructure;    //串口初始化结构体声明
  
  
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM32_Shenzhou_COMInit(&USART_InitStructure);

//  /* Output a message on Hyperterminal using printf function */
//  printf("\n\r\n\r\n\r\n\r");
//  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
//  printf("\r\n\n\n WWW.ARMJISHU.COM  %s configured....", "USART1");
//  printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
//  printf("\n\r www.armjishu.com论坛后续还会有更多精彩的示例，欢迎访问论坛交流与学习."); 
  printf("\n\r 本示例为AD转换示例，串口输出转换结果，模拟信号来自板上的电位器！ \n\r");
//  printf("\n\r==============================================================================");
//  printf("\n\r");
//  


}

