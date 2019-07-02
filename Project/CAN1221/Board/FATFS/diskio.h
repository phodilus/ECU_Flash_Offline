/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2014          /
/-----------------------------------------------------------------------*/
#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#include "integer.h"
	
#define SD_MODE		0	//0 dma,1 interrupt
#define SD_BLOCK	512	//512bytes / block
	
#define SD_USE_WRITE	1	/* 1: Enable disk_write function */
#define SD_USE_IOCTL	1	/* 1: Enable disk_ioctl fucntion */

/* Media Type of Disk */
typedef enum {
	DISK_ATA = 0,		/* 0: ATA drive */
	DISK_SDC = 1,		/* 1: SD card   */
	DISK_USB = 2,		/* 2: USB drive */
} DMEDIA;

/* Status of Disk Functions */
typedef enum {
	STA_NORMAL = 0,		/* 0: Initialized */
	STA_NOINIT = 1,		/* 1: Drive not initialized */
	STA_NODISK = 2,		/* 2: No medium in the drive */
	STA_PROTECT = 3,	/* 3: Write protected */
} DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */
	RES_ERROR = 1,	/* 1: R/W Error */
	RES_WRPRT = 2,	/* 2: Write Protected */
	RES_NOTRDY = 3,	/* 3: Not Ready */
	RES_PARERR = 4,	/* 4: Invalid Parameter */
} DRESULT;

/* Prototypes for disk control functions */
void disk_setmedia(DMEDIA media);
DSTATUS disk_initialize (BYTE drive);
DSTATUS disk_status (BYTE drive);
DRESULT disk_read (BYTE drive, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (BYTE drive, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (BYTE drive, BYTE cmd, void* buff);

/* Generic command (Used by FatFs) */
#define CTRL_SYNC			0	/* Complete pending write process (needed at _FS_READONLY == 0) */
#define GET_SECTOR_COUNT	1	/* Get media size (needed at _USE_MKFS == 1) */
#define GET_SECTOR_SIZE		2	/* Get sector size (needed at _MAX_SS != _MIN_SS) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (needed at _USE_MKFS == 1) */
#define CTRL_TRIM			4	/* Inform device that the data on the block of sectors is no longer used (needed at _USE_TRIM == 1) */

/* Generic command (Not used by FatFs) */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */
#define CTRL_FORMAT			8	/* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20	/* Get F/W revision */
#define ATA_GET_MODEL		21	/* Get model name */
#define ATA_GET_SN			22	/* Get serial number */

#ifdef __cplusplus
}
#endif
#endif
