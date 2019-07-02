/*----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014		*/
/*----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be		*/
/* attached to the FatFs via a glue function rather than modifying it.	*/
/* This is an example of glue functions to attach various exsisting		*/
/* storage control modules to the FatFs module with a defined API.		*/
/*----------------------------------------------------------------------*/

#include "diskio.h"	/* FatFs lower layer API */
#include "stm322xg_eval_sdio_sd.h"

BYTE m_nMedia;
/*----------------------------------------------------------------------*/
/* Interface Implementation												*/
/*----------------------------------------------------------------------*/
DSTATUS ATA_disk_status(void)		{ return STA_NODISK; }
DSTATUS USB_disk_status(void)		{ return STA_NODISK; }
DSTATUS ATA_disk_initialize(void)	{ return STA_NODISK; }
DSTATUS USB_disk_initialize(void)	{ return STA_NODISK; }
DRESULT ATA_disk_read(BYTE* buff, DWORD sector, UINT count)	{ return RES_NOTRDY; }
DRESULT USB_disk_read(BYTE* buff, DWORD sector, UINT count)	{ return RES_NOTRDY; }
DRESULT ATA_disk_write(const BYTE* buff, DWORD sector, UINT count)	{ return RES_NOTRDY; }
DRESULT USB_disk_write(const BYTE* buff, DWORD sector, UINT count)	{ return RES_NOTRDY; }
DRESULT ATA_disk_ioctl(BYTE cmd, void *buff) { return RES_NOTRDY; }
DRESULT USB_disk_ioctl(BYTE cmd, void *buff) { return RES_NOTRDY; }
DSTATUS SDC_disk_status(void)		{ return STA_NORMAL; }
DSTATUS SDC_disk_initialize(void)
{
	SD_CardInfo cinfo;
	if (SD_Init() != SD_OK)
	{
		return STA_NOINIT; //other error
	}
	else
	{
		//read sd card information
		if (SD_GetCardInfo(&cinfo) != SD_OK)
			return  STA_NOINIT;
		if (SD_SelectDeselect((u32) (cinfo.RCA << 16)) != SD_OK)
			return  STA_NOINIT;

		switch(SD_MODE)
		{
			case 0:  //dma mode
				if (SD_EnableWideBusOperation(SDIO_BusWide_4b) != SD_OK)
					return STA_NOINIT;
				#ifdef _SDCARD_H
				if (SD_SetDeviceMode(SD_DMA_MODE) != SD_OK)
					return STA_NOINIT;
				#endif
				break;
			case 1:  //interrupt mode
				if (SD_EnableWideBusOperation(SDIO_BusWide_1b) != SD_OK)
					return STA_NOINIT;
				#ifdef _SDCARD_H
				if (SD_SetDeviceMode(SD_INTERRUPT_MODE) != SD_OK)
					return STA_NOINIT;
				#endif
				break;
			default :
				return STA_NOINIT;
		}
		return STA_NORMAL;
	}
}
DRESULT SDC_disk_read(BYTE* buff, DWORD sector, UINT count)
{
	SD_Error Status; 
	if (count <= 0)
		return RES_PARERR;
	switch (SD_MODE)
	{
		//(sector << 9 is converting sector address to byte address. 1 sector = 512 bytes.)
		case 0://dma mode
			if(count == 1)
				Status = SD_ReadBlock(sector << 9, buff, SD_BLOCK);
			else
				Status = SD_ReadMultiBlocks(sector << 9, buff, SD_BLOCK, count);
			#ifdef _SD_WAIT_DMA
			if (Status != SD_OK)
				break;
			Status = SD_WaitReadOperation();
			if (Status != SD_OK)
				break;
			while (SD_GetTransStatus() != SD_TRANSFER_IDLE)
				;
			#endif
			break;
		case 1://interrupt mode
			if(count == 1)
				Status = SD_ReadBlock(sector << 9, buff, SD_BLOCK);
			else
				Status = SD_ReadMultiBlocks(sector << 9, buff, SD_BLOCK, count);
			break;
		default:
			Status = SD_ERROR;
	}
	if (Status == SD_OK)
		return RES_OK;
	else
		return RES_ERROR;
}
DRESULT SDC_disk_write(const BYTE* buff, DWORD sector, UINT count)
{
	SD_Error Status;
	if (count <= 0)
		return RES_PARERR;
	switch (SD_MODE)
	{
		//(sector << 9 is converting sector address to byte address. 1 sector = 512 bytes.)
		case 0://dma mode
			if(count == 1)
				Status = SD_WriteBlock(sector << 9, (BYTE*)buff, SD_BLOCK);
			else
				Status = SD_WriteMultiBlocks(sector << 9, (BYTE*)buff, SD_BLOCK, count);
			#ifdef _SD_WAIT_DMA
			if (Status != SD_OK)
				break;
			Status = SD_WaitWriteOperation();
			if (Status != SD_OK)
				break;
			while (SD_GetTransStatus() != SD_TRANSFER_IDLE)
				;
			#endif
			break;
		case 1://interrupt mode
			if(count == 1)
				Status = SD_WriteBlock(sector << 9, (BYTE*)buff, SD_BLOCK);
			else
				Status = SD_WriteMultiBlocks(sector << 9, (BYTE*)buff, SD_BLOCK, count);
			break;
		default:
			Status = SD_ERROR;
	}
	if (Status == SD_OK)
		return RES_OK;
	else
		return RES_ERROR;
}
DRESULT SDC_disk_ioctl(BYTE cmd, void *buff)
{
	SD_CardInfo cinfo;
	u32 x, y, z;
    DRESULT res;

	//current fatfs version only need CTRL_SYNC, GET_SECTOR_COUNT and GET_BLOCK_SIZE command.
	switch(cmd)
	{
		case CTRL_SYNC:
			if(SD_GetTransferState() == SD_TRANSFER_IDLE)
				res = RES_OK;
			else
				res = RES_ERROR;
			break;
		case GET_BLOCK_SIZE:
			*(WORD*)buff = SD_BLOCK;
			res = RES_OK;
			break;
		case GET_SECTOR_COUNT:
			////formula of the capacity///////////////
			// memory capacity = BLOCKNR * BLOCK_LEN
			// BLOCKNR = (C_SIZE + 1)* MULT
			//           C_SIZE_MULT+2
			// MULT = 2
			//           READ_BL_LEN
			// BLOCK_LEN = 2
			//////////////////////////////////////////
			if (SD_GetCardInfo(&cinfo) == SD_OK)
			{
				x = cinfo.SD_csd.DeviceSize + 1; //C_SIZE + 1
				y = cinfo.SD_csd.DeviceSizeMul + 2; //C_SIZE_MULT+2
				z = cinfo.SD_csd.RdBlockLen + y;
				*(DWORD*)buff = x << z; 
				res = RES_OK;
			}
			else
			{
				res = RES_ERROR;
			}
			break;
		default:
			res = RES_PARERR;
	}
	return res;
}

/*----------------------------------------------------------------------*/
/* Set Drive Media														*/
/*----------------------------------------------------------------------*/
void disk_setmedia(DMEDIA media)
{
	m_nMedia = media;
}
/*----------------------------------------------------------------------*/
/* Get Drive Status														*/
/*----------------------------------------------------------------------*/
DSTATUS disk_status(BYTE drive/* Physical drive nmuber to identify the drive */)
{
	if (drive != 0)
		return STA_NODISK;
	switch (m_nMedia)
	{
		case DISK_ATA:	return ATA_disk_status();
		case DISK_SDC:	return SDC_disk_status();
		case DISK_USB:	return USB_disk_status();
		default: return STA_NODISK;
	}
}
/*----------------------------------------------------------------------*/
/* Inidialize a Drive													*/
/*----------------------------------------------------------------------*/
DSTATUS disk_initialize(BYTE drive/* Physical drive nmuber to identify the drive */)
{
	if (drive != 0)
		return STA_NODISK;
	switch (m_nMedia)
	{
		case DISK_ATA:	return ATA_disk_initialize();
		case DISK_SDC:	return SDC_disk_initialize();
		case DISK_USB:	return USB_disk_initialize();
		default: return STA_NODISK;
	}
}
/*----------------------------------------------------------------------*/
/* Read Sector(s)														*/
/*----------------------------------------------------------------------*/
DRESULT disk_read(
	BYTE drive,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */)
{
	if (drive != 0)
		return RES_ERROR;
	switch (m_nMedia)
	{
		case DISK_ATA:	return ATA_disk_read(buff, sector, count);
		case DISK_SDC:	return SDC_disk_read(buff, sector, count);
		case DISK_USB:	return USB_disk_read(buff, sector, count);
		default: return RES_ERROR;
	}
}
/*----------------------------------------------------------------------*/
/* Write Sector(s)														*/
/*----------------------------------------------------------------------*/
DRESULT disk_write(
	BYTE drive,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */)
{
	if (drive != 0)
		return RES_ERROR;
	switch (m_nMedia)
	{
		case DISK_ATA:	return ATA_disk_write(buff, sector, count);
		case DISK_SDC:	return SDC_disk_write(buff, sector, count);
		case DISK_USB:	return USB_disk_write(buff, sector, count);
		default: return RES_ERROR;
	}
}
/*----------------------------------------------------------------------*/
/* Miscellaneous Functions												*/
/*----------------------------------------------------------------------*/
DRESULT disk_ioctl(
	BYTE drive,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */)
{
	if (drive != 0)
		return RES_ERROR;
	switch (m_nMedia)
	{
		case DISK_ATA:	return ATA_disk_ioctl(cmd, buff);
		case DISK_SDC:	return SDC_disk_ioctl(cmd, buff);
		case DISK_USB:	return USB_disk_ioctl(cmd, buff);
		default: return RES_ERROR;
	}
}
/*----------------------------------------------------------------------*/
/* User defined to give a current time to fatfs module          		*/
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31)    */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2)          */ 
/*----------------------------------------------------------------------*/                                                                                                                                                                                                                                               
DWORD get_fattime(void)
{
#if 0
    struct tm t;
    DWORD date;
    
    t = Time_GetCalendarTime();
    t.tm_year -= 1980;  //year start from 1980
    t.tm_mon++;			//month 0~11 -> 1~12
    t.tm_sec /= 2;		//second -> 0~29

    date = 0;
    date = (t.tm_year << 25) | (t.tm_mon<<21) | (t.tm_mday<<16) | (t.tm_hour<<11) | (t.tm_min<<5) | (t.tm_sec);
    return date;
#else
	return 0;
#endif
}
