// Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_it.h"
#include "stm32_eval_sdio_sd.h"
#include "ff.h"
//以下文件软件调试使用

#include <stdio.h>                      

#define ADC1_DR_Address    ((uint32_t)0x4001244C) //the address of ADC1
#define ADC3_DR_Address    ((uint32_t)0x4001244C)
#define data_length 4096
uint16_t data_a[data_length];
uint16_t temprature;
uint16_t ret;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure; // ADC 初始化用结构体
DMA_InitTypeDef DMA_InitStructure; //DMA初始化用结构体
ADC_InitTypeDef ADC3_InitStructure;
USART_InitTypeDef USART_InitStructure;    //串口用初始化结构体申明
__IO uint16_t ADCConvertedValue;
FATFS Fs;
FRESULT res;
FIL file;				   		
UINT br;
BYTE buffer[4096];


//////////////////////////////// for debug.
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
//////////////////////////////////////////////////////////

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void); //定时器设置 函数声明
void GPIO_Configuration(void); // GPIO设置 函数声明
/* Private functions ---------------------------------------------------------*/
void Delay_(__IO uint32_t nCount)

{
  for (;  nCount != 0; nCount--);
}
int main(void) //这货只负责采集和存储 
{  





  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM32_Shenzhou_COMInit(&USART_InitStructure);
  printf("\n\r\n\r\n\r\n\r");
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
  printf("\r\n\n\n WWW.ARMJISHU.COM  %s configured....", "USART1");
  printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
  printf("\n\r www.armjishu.comÂÛÌ³ºóÐø»¹»áÓÐ¸ü¶à¾«²ÊµÄÊ¾Àý£¬»¶Ó­·ÃÎÊÂÛÌ³½»Á÷ÓëÑ§Ï°."); 
  printf("\n\r ±¾Ê¾ÀýÎªAD×ª»»Ê¾Àý£¬´®¿ÚÊä³ö×ª»»½á¹û£¬Ä£ÄâÐÅºÅÀ´×Ô°åÉÏµÄµçÎ»Æ÷£¡ \n\r");
  printf("\n\r==============================================================================");
  printf("\n\r");































  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration(); //调用RCC配置函数
  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration(); //调用GPIO函数配置GPIO,函数正文在本文件末尾
  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1); //将DMA的通道x寄存器重设为缺省值
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  //DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&data_a;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  /*
	DMA_PeripheralBaseAddr 该参数用以定义DMA外设基地址
	DMA_MemoryBaseAddr 该参数用以定义DMA内存基地址
	DMA_DIR DMA_DIR规定了外设是作为数据传输的目的地还是来源。 
	DMA_DIR_PeripheralDST 外设作为数据传输的目的地
	DMA_DIR_PeripheralSRC 外设作为数据传输的来源
	DMA_BufferSize DMA_BufferSize用以定义指定DMA通道的DMA缓存的大小，单位为数据单位。根据传输方向，数据单位等于结构中参数DMA_PeripheralDataSize或者参数DMA_MemoryDataSize的值。
	DMA_PeripheralInc 用来设定外设地址寄存器递增与否 DMA_PeripheralInc_Enable 外设地址寄存器递增 DMA_PeripheralInc_Disable 外设地址寄存器不变
	DMA_PeripheralInc_Enable 内存地址寄存器递增 DMA_PeripheralInc_Disable 内存地址寄存器不变
	DMA_PeripheralDataSize_Byte 数据宽度为8位DMA_PeripheralDataSize_HalfWord 数据宽度为16位DMA_PeripheralDataSize_Word 数据宽度为32位
	DMA_MemoryDataSize_Byte 数据宽度为8位DMA_MemoryDataSize_HalfWord 数据宽度为16位DMA_MemoryDataSize_Word 数据宽度为32位
	DMA_Mode_Circular 工作在循环缓存模式 DMA_Mode_Normal 工作在正常缓存模式
	 */
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    /* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA1_Channel1->CNDTR=data_length;
	DMA_Cmd(DMA1_Channel1, ENABLE);
    /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1和2 工作在相互独立的模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; //扫描多通道模式 ON
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换模式ON
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发模式 OFF
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// ADC的12位结果 右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1;//转换的通道数目
  
  ADC_Init(ADC1, &ADC_InitStructure); //调用函数,初始化
		
	ADC3_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1和2 工作在相互独立的模式
  ADC3_InitStructure.ADC_ScanConvMode = DISABLE; //扫描多通道模式 ON
  ADC3_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换模式ON
  ADC3_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发模式 OFF
  ADC3_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// ADC的12位结果 右对齐
  ADC3_InitStructure.ADC_NbrOfChannel = 1;//转换的通道数目
  
  ADC_Init(ADC1, &ADC_InitStructure); //调用函数,初始化
	ADC_Init(ADC3, &ADC3_InitStructure);
		
		
  /* ADC1 egular channel14 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5);
  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_55Cycles5)	;//设置ADC规则通道组,设置各通道的转换顺序和采样时间
  /* Enable ADC1 DMA */

    /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE); //使能或者失能指定的ADC
	ADC_Cmd(ADC3,ENABLE);
    /* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1); //重置指定的ADC的校准寄存器
	ADC_ResetCalibration(ADC3);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
	while(ADC_GetResetCalibrationStatus(ADC3));
  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
	ADC_StartCalibration(ADC3);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1)); //获取ADC重置校准寄存器的状态
	while(ADC_GetCalibrationStatus(ADC3)); 
  
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能或者失能指定的ADC的软件转换启动功能
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
   //=====
  ADC_DMACmd(ADC1, ENABLE); //使能或者失能指定的ADC的DMA请求
  DMA_Cmd(DMA1_Channel1, ENABLE);
	

	f_mount(0, &Fs);
	ret=disk_initialize(0);
	while (1)
  {
		res = f_open(&file, "0:/message.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
		Delay_(80);
  }
	
}
void RCC_Configuration(void)
{
	/*
CR 
时钟控制寄存器
CFGR
时钟配置寄存器
CIR
时钟中断寄存器
APB2RSTR
APB2 外设复位寄存器
APB1RSTR
APB1 外设复位寄存器
AHBENR
AHB外设时钟使能寄存器
APB2ENR
APB2 外设时钟使能寄存器
APB1ENR
APB1 外设时钟使能寄存器
BDCR
备份域控制寄存器
CSR
控制/状态寄存器
*/
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  /* ADCCLK = PCLK2/2 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div2); //设置ADC时钟 2分频还是 4分频 根据芯片型号决定
#else
  /* ADCCLK = PCLK2/4 */
  RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
#endif
  /* Enable peripheral clocks ------------------------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能或者失能AHB外设时钟

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE); //使能或者失能APB2外设时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Configure PC.04 (ADC Channel14) as analog input -------------------------*/
  //配置PC04口作为模拟输入,下面两行都是GPIOInit结构体的成员
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure); //注意这里的取地址
}
