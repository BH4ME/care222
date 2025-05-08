/******************************************************************************
* 文 件 名   : norflash.c
* 版 本 号   : V1.0
* 作    者   : pinot
* 生成日期   : 2019年11月18日
* 功能描述   : 读写NorFlash
* 修改历史   :
* 日    期   :
* 作    者   :
* 修改内容   :
******************************************************************************/

/*****************************************************************************
自定义头文件*/
#include "norflash.h"


/*****************************************************************************
读NorFlash*/
/*****************************************************************************
 函 数 名  : ReadNorFlash
 功能描述  : 读NorFlash
 输入参数  : uint32_t NorAddr    NorFlash首地址
             uint32_t DgusAddr  存储数据的DGUS地址
             uint16_t Len       读取数据字长度(必须是偶数)
 输出参数  : 无
 修改历史  :
 日    期  : 2019年11月04日
 作    者  :
 修改内容  : 创建
*****************************************************************************/
void ReadNorFlash_Dgus(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len)
{
  uint8_t temp[8]={0};
	temp[0] = 0x5A;
	temp[1] = (uint8_t)(NorAddr >> 16);
	temp[2] = (uint8_t)(NorAddr >> 8);
	temp[3] = (uint8_t)(NorAddr);
	temp[4] = (uint8_t)(DgusAddr >> 8);
	temp[5] = (uint8_t)(DgusAddr);
	temp[6] = (uint8_t)(Len >> 8);
	temp[7] = (uint8_t)(Len);
  WriteDGUS(NOR_FLASH_RW_CMD,temp,sizeof(temp));
	do
	{
    DelayMs(5);
	  ReadDGUS(NOR_FLASH_RW_CMD,temp,1);
	}while(temp[0]!=0);
}


/*****************************************************************************
写NorFlash*/
/*****************************************************************************
 函 数 名  : WriteNorFlash
 功能描述  : 写NorFlash
 输入参数  : uint32_t NorAddr    NorFlash首地址
             uint32_t DgusAddr  存储数据的DGUS地址
             uint16_t Len       写入数据字长度(必须是偶数)
 输出参数  : 无
 修改历史  :
 日    期  : 2019年11月04日
 作    者  :
 修改内容  : 创建
*****************************************************************************/
void WriteNorFlash_Dgus(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len)
{
  uint8_t temp[8]={0};
	temp[0] = 0xA5;
	temp[1] = (uint8_t)(NorAddr >> 16);
	temp[2] = (uint8_t)(NorAddr >> 8);
	temp[3] = (uint8_t)(NorAddr);
	temp[4] = (uint8_t)(DgusAddr >> 8);
	temp[5] = (uint8_t)(DgusAddr);
	temp[6] = (uint8_t)(Len >> 8);
	temp[7] = (uint8_t)(Len);
  WriteDGUS(NOR_FLASH_RW_CMD,temp,sizeof(temp));
	do
	{
    DelayMs(5);
	  ReadDGUS(NOR_FLASH_RW_CMD,temp,1);
	}while(temp[0]!=0);
}

/*****************************************************************************
 函 数 名  : WriteNorFlash
 功能描述  : 写NorFlash
 输入参数  : uint32_t NorAddr    NorFlash首地址
             uint32_t DgusAddr  存储数据的DGUS地址
             uint16_t Len       写入数据字长度(必须是偶数)
 输出参数  : 无
 修改历史  :
 日    期  : 2019年11月04日
 作    者  :
 修改内容  : 创建
*****************************************************************************/
void WriteNorFlash_Str(uint32_t NorAddr, uint8_t* buff, uint16_t Len)
{
	uint8_t temp[8]={0};
	EA=0;
	//sys_write_vp_nor(CACHE_ADDR,buff,Len);
  WriteDGUS(CACHE_ADDR,buff,Len);
	temp[0] = 0xA5;
	temp[1] = (uint8_t)(NorAddr >> 16);
	temp[2] = (uint8_t)(NorAddr >> 8);
	temp[3] = (uint8_t)(NorAddr);
	temp[4] = (CACHE_ADDR>>8)&0xff;/**dgusii数据地址**/
	temp[5] = CACHE_ADDR&0xff;
	temp[6] = (uint8_t)(Len >> 8);
	temp[7] = (uint8_t)(Len);
  WriteDGUS(NOR_FLASH_RW_CMD,temp,4);
	//  sys_write_vp_nor(NOR_FLASH_RW_CMD,temp,4);
	do
	{
    DelayMs(5);
		ReadDGUS(NOR_FLASH_RW_CMD,temp,2);

	}while(temp[0]!=0);
	EA=1;
}

void ReadNorFlash_Str(uint32_t NorAddr, uint8_t* buff, uint16_t Len)
{
  uint8_t temp[8]={0};
	EA=0;
	temp[0] = 0x5A;
	temp[1] = (uint8_t)(NorAddr >> 16);
	temp[2] = (uint8_t)(NorAddr >> 8);
	temp[3] = (uint8_t)(NorAddr);
	temp[4] = (CACHE_ADDR>>8)&0xff;//dgusii数据地址
	temp[5] = CACHE_ADDR&0xff;
	temp[6] = (uint8_t)(Len >> 8);
	temp[7] = (uint8_t)(Len);
  	WriteDGUS(NOR_FLASH_RW_CMD,temp,4);
	do
	{
    DelayMs(5);
		ReadDGUS(NOR_FLASH_RW_CMD,temp,2);
	}while(temp[0]!=0);
	ReadDGUS(CACHE_ADDR,buff,Len);
	EA=1;
}
