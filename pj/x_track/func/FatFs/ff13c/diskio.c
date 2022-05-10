/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "drv_sdio_sd.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define SECTOR_SIZE		512

DRESULT MMC_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT MMC_disk_write(const BYTE *buff, DWORD sector, UINT count);

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = 0;
//	int result;

	switch (pdrv) {
	case DEV_RAM :
//		result = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
//		result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB :
//		result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = 0;
//	int result;

	switch (pdrv) {
	case DEV_RAM :
//		result = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
//		result = MMC_disk_initialize();

		// translate the reslut code here

//		return stat;
	
		if(get_sd_detect() == SD_OK)
			return 0;
		else
			return STA_NOINIT;

	case DEV_USB :
//		result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_OK;//妈的！不赋值的话有时不为0！坑！
//	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

//		result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

//		result = MMC_disk_read(buff, sector, count);
		res = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

//		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_OK;
//	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

//		result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

//		result = MMC_disk_write(buff, sector, count);
		res = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

//		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
//	int result;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card
	
		res = RES_ERROR;
		switch(cmd)
		{
			/* SD卡磁盘容量： SDCardInfo.CardCapacity */
			case CTRL_SYNC:       /* Wait for end of internal write process of the drive */
				res = RES_OK;
			break;
			
			case GET_SECTOR_COUNT:/* Get drive capacity in unit of sector (DWORD) */
				*(DWORD*)buff = SDCardInfo.CardCapacity / 512;
				res = RES_OK;
			break;
			
			case GET_BLOCK_SIZE:  /* Get erase block size in unit of sector (DWORD) */
				*(WORD*)buff = 512;
				res = RES_OK;
			break;
			
			case CTRL_TRIM:       /* Erase a block of sectors (used when _USE_ERASE == 1) */
			default:
				res = RES_PARERR;
			break;
		}

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
}

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	return 0;
}

/*
 * 读
 */
DRESULT MMC_disk_read(BYTE *buff, DWORD sector, UINT count)
{
	DRESULT result = RES_OK;
	
	SD_Error Status = SD_OK;
	
	if(count == 1)
	{
		Status = SD_ReadBlock(buff, sector << 9 , SECTOR_SIZE);
	}
	else
	{
		Status = SD_ReadMultiBlocks(buff, sector << 9 , SECTOR_SIZE, count);
	}
	
	if(Status != SD_OK)
	{
		result = RES_ERROR;
	}
	
	/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
	#ifdef SD_DMA_MODE
	
		Status = SD_WaitReadOperation();
		if (Status != SD_OK)
		{
			result = RES_ERROR;
		}
		while(SD_GetStatus() != SD_TRANSFER_OK);
		
	#endif
	
	return result;
}

/*
 * 写
 */
DRESULT MMC_disk_write(const BYTE *buff, DWORD sector, UINT count)
{
	DRESULT result = RES_OK;
	
	SD_Error Status = SD_OK;
	
	if(count == 1)
	{
		Status = SD_WriteBlock((uint8_t *)buff, sector << 9 ,SECTOR_SIZE);
	}
	else
	{
		/* 此处存在疑问： 扇区个数如果写 count ，将导致最后1个block无法写入 */
		//Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count);
		Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count + 1);
	}
	
	if (Status != SD_OK)
	{
		result = RES_ERROR;
	}
	
	/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
	#ifdef SD_DMA_MODE
	
	Status = SD_WaitReadOperation();
	if (Status != SD_OK)
	{
		result = RES_ERROR;
	}
	while(SD_GetStatus() != SD_TRANSFER_OK);
	
	#endif
	
	return result;
}
