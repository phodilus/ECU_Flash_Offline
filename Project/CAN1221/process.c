//
#include "process.h"
#include "stm322xg_eval_sdio_sd.h"
#include "ff.h"
#include "diskio.h"
#include <stdlib.h>
#include <string.h>
FATFS 	m_fatfs;	//sd card file system

LONG	m_nBinPos;
BYTE*	m_pBinBuf = NULL;
DWORD	m_nBinIndex[200][2];
LONG	m_nIndexLen;
LONG	m_nBinSize;

DWORD String2Hex(CHAR* str)
{
	DWORD ret = 0;
	LONG i = 0;
	while (str[i] >= '0' && str[i] <= '9' || str[i] >= 'A' && str[i] <= 'F')
	{
		ret = (ret << 4) | (str[i] >= '0' && str[i] <= '9' ? str[i] - '0' : str[i] - 'A' + 10);
		i ++;
	}
	return ret;
}
DWORD String2Dec(CHAR* str)
{
	DWORD ret = 0;
	LONG i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		ret = ret * 10 + (str[i] - '0');
		i ++;
	}
	return ret;
}
CHAR* SendMsg(CHAR* cmd, LONG maxl)
{
	FIL FBIN;
	BYTE data[SD_BLOCK], tbuf[8], rmsg[8];
	UINT size;
	LONG ID;
	CHAR temp[4];
	LONG i;

	UINT SLEN, SCNT;//max data length & count to send one time.
	UINT BSIZE;//bin file size.
	LONG pos, len, len2;//parameter used to send message.
	LONG snum, fnum;//send count in a frame, frame number.
	UINT csum;//check sum value.

	//transmit command data.
	i = 0;
	if (cmd[i] == '<')
	{
		i += 2;//(skip "<<")
		//get transmit id.
		ID = String2Hex(cmd + i);
		//skip "ID:".
		while (i < maxl && cmd[i] != '=' && cmd[i] != ':')
			i ++;
		//send bin file.
		if (cmd[i] == '=')
		{
			i ++;//(skip "=")
			//get file format.
			len = 0;
			while (i + len < maxl && cmd[i + len] != '.')
				len ++;
			if (memcmp(cmd + i + len + 1, "BIN", 3) != 0 && memcmp(cmd + i + len + 1, "bin", 3) != 0)
				return NULL;

			//get file name & open file.
			len = 0;
			while (i + len < maxl && cmd[i + len] != '%')
				len ++;
			cmd[i + len] = NULL;
			if (f_open(&FBIN, cmd + i, FA_OPEN_EXISTING | FA_READ) != FR_OK)
				return NULL;
			BSIZE = f_size(&FBIN);
			i += len + 1;//(skip "filename.bin%")

			//get message length & count.
			SLEN = String2Dec(cmd + i);
			SCNT = String2Dec(cmd + i + 2);
			//skip "8*63:".
			while (i < maxl && cmd[i] != ':')
				i ++;
			i ++;

			//reset frame number & check sum value.
			cmd += i;
			fnum = 1;

			//read a new data block to send.
			len2 = BSIZE - f_tell(&FBIN) < SLEN * SCNT ? BSIZE - f_tell(&FBIN) : SLEN * SCNT;
			if (f_read(&FBIN, data, len2, &size) != FR_OK)
				return NULL;
			if (size == 0)
				return NULL;
			i = 0;

			//loop to send command & hex data.
			while (size > 0 || //(only check in the end of a line)
				(cmd[i + 1] != '\0' && cmd[i + 1] != '\r' && cmd[i + 1] != '\n'))
			{
				LED_Switch(LED_1, -1);
				switch (cmd[i + 1])
				{
				case '\0':
				case '\r':	//go back to the front.
				case '\n':
					//read a new data block to send.
					len2 = BSIZE - f_tell(&FBIN) < SLEN * SCNT ? BSIZE - f_tell(&FBIN) : SLEN * SCNT;
					if (f_read(&FBIN, data, len2, &size) != FR_OK)
						return NULL;
					if (size == 0)
						break;
					i = 0;
					break;
				case 'L':	//send frame length.
					len = 1;
					len2 += 2;//(including index & checksum byte)
					if (len2 > 0xFF)
					{
						if (cmd[i] != 'L')//0x41, 0xFA
							tbuf[0] = (0x0F00 & len2) >> 8 | (String2Hex(cmd + i) & 0x0F) << 4;
						else
							tbuf[0] = (0xFF00 & len2) >> 8;
						csum += tbuf[0];
						USER_CAN_Transmit(CAN_1, ID, len, tbuf);
					}
					tbuf[0] = (0x00FF & len2);
					csum += tbuf[0];
					USER_CAN_Transmit(CAN_1, ID, len, tbuf);
					i += 3;//(skip "LL;")
					break;
				case 'N':	//send frame number.
					len = 1;
					tbuf[0] = fnum & 0xFF;
					csum += tbuf[0];
					USER_CAN_Transmit(CAN_1, ID, len, tbuf);
					fnum ++;
					i += 3;//(skip "NN;")
					break;
				case '&':	//send checksum value.
					if (cmd[i] == '0')//"0&;", reset check sum value.
					{
						csum = 0;
					}
					else//"&&;", send check sum value.
					{
						len = 1;
						tbuf[0] = csum & 0xFF;
						USER_CAN_Transmit(CAN_1, ID, len, tbuf);
						//target need some time to compare this value.
						Sleep(5);
					}
					i += 3;//(skip "0&;" or "&&;")
					break;
				case 'X':	//send bin data.
					pos = 0;
					snum = 0;
					while (snum < SCNT)
					{
						len = size - pos < SLEN ? size - pos : SLEN;
						if (len == 0)
							break;
						for (len2 = pos; len2 < pos + len; len2 ++)
							csum += data[len2];
						USER_CAN_Transmit(CAN_1, ID, len, data + pos);
						pos += len;
						snum ++;
					}
					i += 3;//(skip "XX;")
					break;
				case '>':	//receive message.
					i += 2;//(skip ">>")
					//wait for receiving.
					if (USER_CAN_Receive(CAN_1, String2Hex(cmd + i), rmsg, 2000) <= 0)
						return NULL;
					//skip "ID:".
					while (i < maxl && cmd[i] != ':')
						i ++;
					i ++;
					//if not echo data, prepare data content to check.
					if (cmd[i] != '~')
					{
						if (cmd[i + 1] == 'N')
						{
							len = 1;
							if (cmd[i] == 'N')
								data[0] = fnum;
							else//cmd[i] == 5
								data[0] = (0x0F & fnum) | (String2Hex(cmd + i) & 0x0F) << 4;
							i += 3;//(skip "5N;")
						}
						else//(other commands)
						{
							//convert hex string to value.
							len = 0;
							while (i + len * 2 < maxl && len < 8 && cmd[i + len * 2] != ';')
							{
								temp[0] = cmd[i + len * 2];
								temp[1] = cmd[i + len * 2 + 1];
								temp[2] = NULL;
								data[len] = String2Hex(temp);
								len ++;
							}
							i += len * 2 + 1;//(skip "MESSAGE;")
						}
					}
					else
					{
						i += 2;//(skip "~;")
					}
					//check content from tail.
					len --;
					while (len >= 0 && data[len] == rmsg[len])
						len --;
					if (len >= 0)
						return NULL;
					break;
				default:	//send normal command.
					//convert hex string to value.
					//(use tbuf to avoid modifying data buffer)
					len = 0;
					while (i + len * 2 < maxl && len < 8 && cmd[i + len * 2] != ';')
					{
						temp[0] = cmd[i + len * 2];
						temp[1] = cmd[i + len * 2 + 1];
						tbuf[len] = String2Hex(temp);
						csum += tbuf[len];
						len ++;
					}
					//start transmission.
					USER_CAN_Transmit(CAN_1, ID, len, tbuf);
					i += len * 2 + 1;//(skip "CCMMDD;")
					break;
				}//switch (cmd[i + 1])
			}//while (size > 0)
			if (i > 0)
				return cmd + i;//this line is complete.
			else
				return NULL;
		}//if (cmd[i] == '=')
		else if (cmd[i] == ':')//(send command directly)
		{
			LED_Switch(LED_1, -1);
			i ++;//(skip ":")
			while (cmd[i] != '\r' && cmd[i] != '\n' && cmd[i] != '>')
			{
				if (cmd[i + 1] == '&')//send checksum value.
				{
					if (cmd[i] == '0')//"0&;", reset check sum value.
					{
						csum = 0;
					}
					else//"&&;", send check sum value.
					{
						len = 1;
						tbuf[0] = csum & 0xFF;
						USER_CAN_Transmit(CAN_1, ID, len, tbuf);
					}
					i += 3;//(skip "0&;" or "&&;")
				}
				/*else if (cmd[i + 1] == 'S')//send size info (do not support this now).
				{
					//send file size.
					tbuf[2] = BSIZE >> 16 & 0xFF;
					csum += tbuf[2];
					tbuf[1] = BSIZE >> 8 & 0xFF;
					csum += tbuf[1];
					tbuf[0] = BSIZE & 0xFF;
					csum += tbuf[0];
					len = 0;
					while (cmd[i + len * 2] == 'S')
						len ++;
					USER_CAN_Transmit(CAN_1, ID, len, tbuf);
					i += len * 2;//(skip "SS")
				}*/
				else//send hex string.
				{
					//(use data to enable receive check)
					len = 0;
					while (i + len * 2 < maxl && len < 8 && cmd[i + len * 2] != ';')
					{
						temp[0] = cmd[i + len * 2];
						temp[1] = cmd[i + len * 2 + 1];
						data[len] = String2Hex(temp);
						csum += data[len];
						len ++;
					}
					//start transmission.
					USER_CAN_Transmit(CAN_1, ID, len, data);
					i += len * 2 + 1;//(skip "CCMMDD;")
				}
			}//while (cmd[i] != '\r' && cmd[i] != '\n' && cmd[i] != '>')
			//receive message & check.
			if (cmd[i] == '>')
			{
				i += 2;//(skip ">>")
				//get receive id.
				ID = String2Hex(cmd + i);
				//skip "ID:".
				while (i < maxl && cmd[i] != ':')
					i ++;
				i ++;
				//wait for receiving.
				if (USER_CAN_Receive(CAN_1, ID, rmsg, 3000) <= 0)
					return NULL;
				//if not echo data, prepare data content to check.
				if (cmd[i] != '~')
				{
					if (cmd[i + 1] == 'N')//(next index)
					{
						len = 1;
						if (cmd[i] == 'N')
							data[0] = fnum;
						else//cmd[i] == 5
							data[0] = (0x0F & fnum) | (String2Hex(cmd + i) & 0x0F) << 4;
						i += 3;//(skip "5N;")
					}
					else//(other commands)
					{
						//convert hex string to value.
						len = 0;
						while (i + len * 2 < maxl && len < 8 && cmd[i + len * 2] != ';')
						{
							temp[0] = cmd[i + len * 2];
							temp[1] = cmd[i + len * 2 + 1];
							data[len] = String2Hex(temp);
							len ++;
						}
						i += len * 2 + 1;//(skip "MESSAGE;")
					}
				}
				//check content from tail.
				len --;
				while (len >= 0 && data[len] == rmsg[len])
					len --;
				if (len >= 0)
					return NULL;
			}//if (cmd[i] == '>')
			return cmd + i;
		}//else if (cmd[i] == ':')
		else//nether '=' or ':' after '<'.
		{
			return NULL;
		}
	}//if (cmd[i] == '<')
	//check receive content.
	else if (cmd[i] == '>')
	{
		LED_Switch(LED_1, -1);
		i += 2;//(skip ">>")
		//get receive id.
		ID = String2Hex(cmd + i);
		while (i < maxl && cmd[i] != ':')
			i ++;
		i ++;
		//wait for receiving.
		if (USER_CAN_Receive(CAN_1, ID, rmsg, 2000) <= 0)
			return NULL;
		//if not echo data, prepare data content to check.
		if (cmd[i] != '~')
		{
			//convert hex string to value.
			len = 0;
			while (i + len * 2 < maxl && len < 8 && cmd[i + len * 2] != ';')
			{
				temp[0] = cmd[i + len * 2];
				temp[1] = cmd[i + len * 2 + 1];
				data[len] = String2Hex(temp);
				len ++;
			}
			i += len * 2 + 1;//(skip "MESSAGE;")
		}
		while (cmd[i] != NULL && cmd[i] != '\r' && cmd[i] != '\n')
			i ++;
		//check content from tail.
		len --;
		while (len >= 0 && data[len] == rmsg[len])
			len --;
		if (len >= 0)
			return NULL;
		return cmd + i;
	}//else if (cmd[i] == '>')
	else
	{
		return NULL;
	}
}
BOOL Process(void)
{
	CHAR cmd[SD_BLOCK]; //read buffer.
	FIL FCFG;//config file.
	LONG i;//current char offset of buffer.
	UINT size;//actual byte count read from file.

	//mount sd card.
	disk_setmedia(DISK_SDC);
	//if (disk_initialize(0) != STA_NORMAL)
	//	return FALSE;
	if (f_mount(&m_fatfs, "0", 1) != FR_OK)
		return FALSE;
	//open config file & load config data.
	if (f_open(&FCFG, "FLASH.cfg", FA_OPEN_EXISTING | FA_READ) != FR_OK)
		return FALSE;
	while (1)
	{
		//read info from new line.
		if (f_read(&FCFG, cmd, 60, &size) != FR_OK)
			return FALSE;
		if (size == 0)
			break;
		//skip \r\n or other illegal chars.
		i = 0;
		while (i < size)
		{
			if (cmd[i] == '<' || cmd[i] == '>')
			{
				break;
			}
			else if (cmd[i] == '@')
			{
				i ++;
				Sleep(String2Dec(cmd + i));
			}
			else if (cmd[i] == '/')
			{
				while (i < size && cmd[i] != '\r' && cmd [i] != '\n')
					i ++;
			}
			else//'\r', '\n'
			{
				i ++;
			}
		}
		if (i != size && TRUE/*SendMsg(cmd + i, size - i) == NULL*/)
			break;
		//move to the next line.
		while (i < size && cmd[i] != '\r' && cmd[i] != '\n')
			i ++;
		//move file pointer back according to the actual send count.
		if (f_lseek(&FCFG, f_tell(&FCFG) - (size - i)) != FR_OK)
			return FALSE;
	}
	LED_Switch(LED_1, 0);
	f_close(&FCFG);
	f_mount(NULL, "0", 1);
	if (size != 0)
		return FALSE;
	else
		return TRUE;
}

