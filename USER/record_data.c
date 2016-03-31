#include "stdio.h"
#include "diskio.h"
#include "stm32f10x.h"
#include "ff.h"

extern FATFS fs;
extern FRESULT res;
extern FIL file;				
extern UINT br;
extern u16 Precent; 
extern u16 nn;
extern u16 record_flag;

void record_data(void)
{
	if(record_flag)
	{
		Led_light(2);
	 res=f_lseek(&file,file.fsize);
   f_printf(&file,"%d \n",Precent);
	 delay_ms(20);
	 nn++;
		Led_OFF(2);
}
	
}
