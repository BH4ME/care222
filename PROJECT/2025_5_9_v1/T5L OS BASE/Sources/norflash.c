/******************************************************************************
* �� �� ��   : norflash.c
* �� �� ��   : V1.0
* ��    ��   : pinot
* ��������   : 2019��11��18��
* ��������   : ��дNorFlash
* �޸���ʷ   :
* ��    ��   :
* ��    ��   :
* �޸�����   :
******************************************************************************/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "norflash.h"


/*****************************************************************************
��NorFlash*/
/*****************************************************************************
 �� �� ��  : ReadNorFlash
 ��������  : ��NorFlash
 �������  : uint32_t NorAddr    NorFlash�׵�ַ
             uint32_t DgusAddr  �洢���ݵ�DGUS��ַ
             uint16_t Len       ��ȡ�����ֳ���(������ż��)
 �������  : ��
 �޸���ʷ  :
 ��    ��  : 2019��11��04��
 ��    ��  :
 �޸�����  : ����
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
дNorFlash*/
/*****************************************************************************
 �� �� ��  : WriteNorFlash
 ��������  : дNorFlash
 �������  : uint32_t NorAddr    NorFlash�׵�ַ
             uint32_t DgusAddr  �洢���ݵ�DGUS��ַ
             uint16_t Len       д�������ֳ���(������ż��)
 �������  : ��
 �޸���ʷ  :
 ��    ��  : 2019��11��04��
 ��    ��  :
 �޸�����  : ����
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
 �� �� ��  : WriteNorFlash
 ��������  : дNorFlash
 �������  : uint32_t NorAddr    NorFlash�׵�ַ
             uint32_t DgusAddr  �洢���ݵ�DGUS��ַ
             uint16_t Len       д�������ֳ���(������ż��)
 �������  : ��
 �޸���ʷ  :
 ��    ��  : 2019��11��04��
 ��    ��  :
 �޸�����  : ����
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
	temp[4] = (CACHE_ADDR>>8)&0xff;/**dgusii���ݵ�ַ**/
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
	temp[4] = (CACHE_ADDR>>8)&0xff;//dgusii���ݵ�ַ
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
