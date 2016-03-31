#include "stm32f10x.h"
#include "ffconf.h"
#include "diskio.h"
#include "sdcard.h"

SD_CardInfo SDCardInfo2;
extern SD_Error States;



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
        BYTE drv                /* Physical drive number (0) */
)
{
  States=SD_Init();
  SD_GetCardInfo(&SDCardInfo2);
  SD_SelectDeselect((uint32_t) (SDCardInfo2.RCA << 16));
  SD_EnableWideBusOperation(SDIO_BusWide_4b);
  SD_SetDeviceMode(SD_POLLING_MODE);
  return 0;
}


/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
        BYTE drv                /* Physical drive number (0) */
)
{
        return 0;
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
        BYTE drv,                        /* Physical drive number (0) */
        BYTE *buff,                        /* Pointer to the data buffer to store read data */
        DWORD sector,                /* Start sector number (LBA) */
        BYTE count                        /* Sector count (1..255) */
)
{
        uint16_t Transfer_Length;
        uint32_t Memory_Offset;

        Transfer_Length =  count * 512;
        Memory_Offset = sector * 512;
        
        SD_ReadBlock(Memory_Offset, (uint32_t *)buff, Transfer_Length);
        return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
        BYTE drv,                        /* Physical drive number (0) */
        const BYTE *buff,        /* Pointer to the data to be written */
        DWORD sector,                /* Start sector number (LBA) */
        BYTE count                        /* Sector count (1..255) */
)
{
        uint16_t Transfer_Length;
        uint32_t Memory_Offset;

        Transfer_Length =  count * 512;
        Memory_Offset = sector * 512;

  SD_WriteBlock(Memory_Offset, (uint32_t *)buff, Transfer_Length);
        
        return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/

DWORD get_fattime ()
{
        return        ((2006UL-1980) << 25)              // Year = 2006
                        | (2UL << 21)              // Month = Feb
                        | (9UL << 16)              // Day = 9
                        | (22U << 11)              // Hour = 22
                        | (30U << 5)              // Min = 30
                        | (0U >> 1)              // Sec = 0
                        ;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
                
DRESULT disk_ioctl (
        BYTE drv,                // Physical drive number (0) 
        BYTE ctrl,                // Control code
        void *buff                // Buffer to send/receive control data
)
{                
        DRESULT res = RES_OK;
        uint32_t status = SD_NO_TRANSFER;
        //uint32_t status = NAND_READY;
        


                switch (ctrl) {
                case CTRL_SYNC :                /// Make sure that no pending write process
                        status = SD_GetTransferState();
                        if (status == SD_NO_TRANSFER)

                                {res = RES_OK;}
                        else{res = RES_ERROR;}
                        break;

                case GET_SECTOR_COUNT :          // Get number of sectors on the disk (DWORD)
                        *(DWORD*)buff = 131072;        // 4*1024*32 = 131072
                        res = RES_OK;
                        break;

                case GET_SECTOR_SIZE :          // Get R/W sector size (WORD) 
                        *(DWORD*)buff = 512;
                        res = RES_OK;
                        break;

                case GET_BLOCK_SIZE :            // Get erase block size in unit of sector (DWORD)
                        *(DWORD*)buff = 32;
                        res = RES_OK;
                  }
          
        return res;
} 
