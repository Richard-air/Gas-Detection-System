/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "sdcard.h"


/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

//#define ATA		0
//#define MMC		1
//#define USB		2

extern SD_Error SD_USER_Init(void);
extern SD_CardInfo SDCardInfo;
SD_CardInfo SDCardInfo;
#define SECTOR_SIZE 512U
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	SD_Error Status;

	switch ( drv )	
	{
		case 0 :
			Status = SD_USER_Init(); /* SD_USER_Init() 这个函数由用户编写 */
			if ( Status == SD_OK )	 /* SD 卡初始化成功,这一步非常重要 */
				return 0;
			else
			return STA_NOINIT;

		case 1 :
			return STA_NOINIT;

		case 2 :
			return STA_NOINIT;
	}
	return STA_NOINIT;

/* 下面注释掉的部分为FATFS自带的 */
//	DSTATUS stat;
//	int result;
//
//	switch (drv) {
//	case ATA :
//		//result = ATA_disk_initialize();
//		// translate the reslut code here
//
//		return stat;
//
//	case MMC :
//		result = MMC_disk_initialize();
//		// translate the reslut code here
//
//		return stat;
//
//	case USB :
//		result = USB_disk_initialize();
//		// translate the reslut code here
//
//		return stat;
//	}
//	return STA_NOINIT;	
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	switch (drv) /* 用户自己添加应用代码 */
	{
	  case 0 :
		
	  /* translate the reslut code here	*/

	    return 0;

	  case 1 :
	
	  /* translate the reslut code here	*/

	    return 0;

	  case 2 :
	
	  /* translate the reslut code here	*/

	    return 0;

	  default:

        break;
	}
	return STA_NOINIT;

/* 下面注释掉的部分为FATFS自带的 */
//	DSTATUS stat;
//	int result;
//
//	switch (drv) {
//	case ATA :
//		//result = ATA_disk_status();
//		// translate the reslut code here
//
//		return stat;
//
//	case MMC :
//		result = MMC_disk_status();
//		// translate the reslut code here
//
//		return stat;
//
//	case USB :
//		result = USB_disk_status();
//		// translate the reslut code here
//
//		return stat;
//	}
//	return STA_NOINIT;
	
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{	
	if ( count == 1 )		/* 1个sector的读操作 */
  {	
  	SD_ReadBlock( sector << 9, (u32 *)(&buff[0]), SECTOR_SIZE );
		//SD_ReadBlock( sector << 9, (u32 *)(&buff[0]), SDCardInfo.CardBlockSize );
		
	}
	else        			 /* 多个sector的读操作 */
	{
		SD_ReadMultiBlocks( sector << 9, (u32 *)(&buff[0]), SECTOR_SIZE, count );
		
	}
	return RES_OK;		 

/* 下面注释掉的部分为FATFS自带的 */
//	DRESULT res;
//	int result;
//
//	switch (drv) {
//	case ATA :
//		//result = ATA_disk_read(buff, sector, count);
//		// translate the reslut code here
//
//		return res;
//
//	case MMC :
//		result = MMC_disk_read(buff, sector, count);
//		// translate the reslut code here
//
//		return res;
//
//	case USB :
//		result = USB_disk_read(buff, sector, count);
//		// translate the reslut code here
//
//		return res;
//	}
//	return RES_PARERR;	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{	
	if ( count == 1 )		/* 1个sector的写操作 */
  {		
		SD_WriteBlock(sector << 9 ,(u32 *)(&buff[0]),SECTOR_SIZE);
	}
	else							 /* 多个sector的写操作 */
  {		
    SD_WriteMultiBlocks(sector << 9 ,(u32 *)(&buff[0]),SECTOR_SIZE,count);
	}        
  return RES_OK;	

/* 下面注释掉的部分为FATFS自带的 */
//	DRESULT res;
//	int result;
//
//	switch (drv) {
//	//case ATA :
//		result = ATA_disk_write(buff, sector, count);
//		// translate the reslut code here
//
//		return res;
//
//	case MMC :
//		result = MMC_disk_write(buff, sector, count);
//		// translate the reslut code here
//
//		return res;
//
//	case USB :
//		result = USB_disk_write(buff, sector, count);
//		// translate the reslut code here
//
//		return res;
//	}
//	return RES_PARERR; 	
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;	// 0
/* 下面注释掉的部分为FATFS自带的 */
//	DRESULT res;
//	int result;
//
//	switch (drv) {
//	case ATA :
//		// pre-process here
//
//		//result = ATA_disk_ioctl(ctrl, buff);
//		// post-process here
//
//		return res;
//
//	case MMC :
//		// pre-process here
//
//		result = MMC_disk_ioctl(ctrl, buff);
//		// post-process here
//
//		return res;
//
//	case USB :
//		// pre-process here
//
//		result = USB_disk_ioctl(ctrl, buff);
//		// post-process here
//
//		return res;
//	}
//	return RES_PARERR;	
}

/* 得到文件Calendar格式的建立日期,是DWORD get_fattime (void) 逆变换 */							
/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{   
	return 0;
}

///
SD_Error SD_USER_Init(void)
{
	SD_Error Status = SD_OK;

	/* SD Init */
  Status = SD_Init();

  if (Status == SD_OK)
  {
    /* Read CSD/CID MSD registers */
   Status = SD_GetCardInfo( &SDCardInfo );
  }
  
  if (Status == SD_OK)
  {
    /* Select Card */
    Status = SD_SelectDeselect( (u32) (SDCardInfo.RCA << 16) );
  }
  
  if (Status == SD_OK)
  {
	  /* set bus wide */
    Status = SD_EnableWideBusOperation( SDIO_BusWide_1b );
  }
  
  /* Set Device Transfer Mode to DMA */
  if (Status == SD_OK)
  { 
	  /* 任选一种都可以工作 */ 
    //Status = SD_SetDeviceMode( SD_DMA_MODE );
    Status = SD_SetDeviceMode( SD_POLLING_MODE );
    //Status = SD_SetDeviceMode( SD_INTERRUPT_MODE );	
  }
	return ( Status );
}
