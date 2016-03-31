#include "stm32f10x.h"
#include "fsmc_nand.h"
#include <stdio.h>
#define data_length 5000
extern uint16_t data_a[data_length];
void Delay_(__IO uint32_t nCount);

/* Private typedef -----------------------------------------------------------*/
/*神舟III号LED灯相关定义*/
#define RCC_GPIO_LED                    RCC_APB2Periph_GPIOF    /*LED使用的GPIO时钟*/
#define LEDn                            4                       /*神舟III号LED数量*/
#define GPIO_LED                        GPIOF                   /*神舟III号LED灯使用的GPIO组*/

#define DS1_PIN                         GPIO_Pin_6              /*DS1使用的GPIO管脚*/
#define DS2_PIN                         GPIO_Pin_7				/*DS2使用的GPIO管脚*/
#define DS3_PIN                         GPIO_Pin_8  			/*DS3使用的GPIO管脚*/
//#define DS4_PIN                         GPIO_Pin_9				/*DS4使用的GPIO管脚*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         0x400
#define NAND_HY_MakerID     0xAD
#define NAND_HY_DeviceID    0xF1
//#define NAND_ST_MakerID     0x20
//#define NAND_ST_DeviceID    0x76

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

NAND_IDTypeDef NAND_ID;
GPIO_InitTypeDef GPIO_InitStructure;
NAND_ADDRESS WriteReadAddr;
USART_InitTypeDef USART_InitStructure;

uint16_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];
__IO uint32_t PageNumber = 2, WriteReadStatus = 0, status= 0;
uint32_t j = 0;


/* Private function prototypes -----------------------------------------------*/
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

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

void main_ram2flash(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     

  /* Initialize Leds mounted on STM3210X-EVAL board */
  RCC_APB2PeriphClockCmd(RCC_GPIO_LED, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = DS1_PIN | DS2_PIN | DS3_PIN ;//| DS4_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIO_LED, &GPIO_InitStructure);
  GPIO_SetBits(GPIO_LED, DS1_PIN | DS2_PIN | DS3_PIN );//| DS4_PIN);   
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
  /* Output a message on Hyperterminal using printf function */
  printf("\n\r--------------------------------------------- ");
  printf("\n\r神舟III号 Nand Flash读写程序");
  printf("\n\r   --DS1闪烁表示神舟III号正常运行");
  printf("\n\r   --DS2--亮，表示读写Nand Flash成功");
  printf("\n\r   --DS3--亮，表示读写Nand Flash失败");
  printf("\n\r   --DS4--亮，表示没有读到Nand Flash的ID");
  printf("\n\r--------------------------------------------- ");


  /*使能FSMC时钟 */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  /*配置与SRAM连接的FSMC BANK2 NAND*/
  NAND_Init();

  /*读取Nand Flash ID并打印*/
  NAND_ReadID(&NAND_ID);
  printf("\n\r Nand Flash ID:0x%x\t 0x%x\t 0x%x\t 0x%x",NAND_ID.Maker_ID,NAND_ID.Device_ID,
  	                                                    NAND_ID.Third_ID,NAND_ID.Fourth_ID);

  /*校验Nand Flash 的ID是否正确*/
  if((NAND_ID.Maker_ID == NAND_HY_MakerID) && (NAND_ID.Device_ID == NAND_HY_DeviceID))  
  {

	/*设置NAND FLASH的写地址*/
    WriteReadAddr.Zone = 0x00;
    WriteReadAddr.Block = 0x00;
    WriteReadAddr.Page = 0x00;
////////////////////////////////////////////////////
	//////////////////////////////////
	////////////////////////
	////nand flash 的数据是8位的.
	/*擦除待写入数据的块*/
    status = NAND_EraseBlock(WriteReadAddr);

    /*将写Nand Flash的数据BUFFER填充为从0x25开始的连续递增的一串数据 */
    //Fill_Buffer(TxBuffer, BUFFER_SIZE , 0x25);
    /*将数据写入到Nand Flash中。WriteReadAddr:读写的起始地址*/	
    	TxBuffer=data_a;
    status = NAND_WriteSmallPage(TxBuffer, WriteReadAddr, PageNumber);

    /*从Nand Flash中读回刚写入的数据。riteReadAddr:读写的起始地址*/
    status = NAND_ReadSmallPage (RxBuffer, WriteReadAddr, PageNumber);
  
    /*判断读回的数据与写入的数据是否一致*/  
    for(j = 0; j < BUFFER_SIZE; j++)
    {
      if(TxBuffer[j] != RxBuffer[j])
      {
        WriteReadStatus++;
      }
    }
    printf("\n\r Nand Flash读写访问程序运行结果: ");
    if (WriteReadStatus == 0)
    { 
   	  printf("\n\r Nand Flash读写访问成功");
      GPIO_ResetBits(GPIO_LED, DS2_PIN);	  
    }
    else
    { 
   	  printf("\n\r Nand Flash读写访问失败");	  
	  printf("0x%x",WriteReadStatus);
    
      GPIO_ResetBits(GPIO_LED, DS3_PIN); 	  
	  
    }
  }
  else
  {
   	  printf("\n\r 没有检测到Nand Flash的ID");	  
      //GPIO_ResetBits(GPIO_LED, DS4_PIN); 
  }

  while(1)
  {
    	GPIO_ResetBits(GPIO_LED, DS1_PIN);
		Delay_(0x3FFFFF);
  		GPIO_SetBits(GPIO_LED, DS1_PIN);
		Delay_(0x3FFFFF);	
  }
}

/**
  *   Function name : Fill_Buffer
  * @brief  Fill the buffer
  * @param  pBuffer: pointer on the Buffer to fill
  * @param  BufferSize: size of the buffer to fill
  * @param  Offset: first value to fill on the Buffer
  */
void Fill_Buffer(uint8_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  /* Put in global buffer same values */
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  }

  return ch;
}
