#include "stm32f10x_dma.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"

#define BufferSize 32

extern uint16_t data_a[];
void Delay_(__IO uint32_t nCount);
uint16_t CurrDataCounter;



void Dma_Init(void)
{
  DMA_InitTypeDef  DMA_InitStructure;
	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA channel6 configuration */
  DMA_DeInit(DMA1_Channel2);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)data_a;			
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DST_Buffer;					
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;   						     
  DMA_InitStructure.DMA_BufferSize = BufferSize;							
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);

  /* Enable DMA Channel1 Transfer Complete interrupt */
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	
  /* Get Current Data Counter value before transfer begins */
  CurrDataCounter= DMA_GetCurrDataCounter(DMA1_Channel2);
	
  /* Enable DMA Channel6 transfer */
  DMA_Cmd(DMA1_Channel2, ENABLE);
	}

	//===========
	
	
//void NVIC_Config(void)  
//{  
//    NVIC_InitTypeDef NVIC_InitStructure;  
//      
//    /* Configure one bit for preemption priority -------------------------------- */  
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
//  
//     /* Enable DMA channel1 IRQ Channel */  
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//    NVIC_Init(&NVIC_InitStructure);  
//}  



int ramdmaflash(void)
{   
// 	int count;
	uint32_t judge;
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x00);  // NVIC_VectTab_FLASH=0x08000000
// 	RCC_Config();
 	//SysTick_Init();
	//GPIO_Config();
	
 	
 	NVIC_Config();
	USART1_Init(19200);	
	
	FLASH_SetLatency(FLASH_Latency_1);			//??2?????
    /* Enable Prefetch Buffer --???????*/
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	Dma_Init();
	/* Get Current Data Counter value before transfer begins */
	CurrDataCounter = DMA_GetCurrDataCounter(DMA1_Channel2);	//????DMA??x, ?????????
	
 	while( CurrDataCounter!=0) ;
	judge = memcmp(SRC_Const_Buffer,DST_Buffer,BufferSize);
    
	if(0==judge)
	{
		USART1_SendData(" Same !!!\r\n",sizeof(" Same !!!\r\n"));
	}
	else
	{
		USART1_SendData(" different !!!\r\n",sizeof(" different !!!\r\n"));
	}

	Delay_ms(1);
	
}
