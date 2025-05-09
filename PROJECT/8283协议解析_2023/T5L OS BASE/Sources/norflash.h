/******************************************************************************
文 件 名   : norflash.h
版 本 号   : V1.0
作    者   : pinot
生成日期   : 2019年11月18日
功能描述   : 读写NorFlash
修改历史   :
日    期   :
作    者   :
修改内容   :
******************************************************************************/

#ifndef _NORFLASH_H_
#define _NORFLASH_H_

/*****************************************************************************
系统库*/

/*****************************************************************************
自定义头文件*/
#include "T5LOS8051.H"
#include "sys.h"
#include "dgus.h"

/*****************************************************************************
宏定义变量*/
#define NOR_FLASH_RW_CMD 0x0008
#define CACHE_ADDR 0xf000			/**Dgus缓冲区***/
/*****************************************************************************
对外函数声明*/
void ReadNorFlash_Dgus(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len);
void WriteNorFlash_Dgus(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len);
void ReadNorFlash_Str(uint32_t NorAddr, uint8_t* buff, uint16_t Len);
void WriteNorFlash_Str(uint32_t NorAddr, uint8_t* buff, uint16_t Len);
#endif