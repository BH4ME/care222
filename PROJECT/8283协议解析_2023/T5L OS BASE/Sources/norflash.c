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
void sys_write_vp_nor(u16 addr,u8* buf,u16 len)
{   
	u8 i;  
	EA=0;
	i = (u8)(addr&0x01);
	addr >>= 1;
	ADR_H = 0x00;
	ADR_M = (u8)(addr>>8);
	ADR_L = (u8)addr;    
	ADR_INC = 0x01;
	RAMMODE = 0x8F;
	while(APP_ACK==0);
	if(i && len>0)
	{	
		RAMMODE = 0x83;	
		DATA1 = *buf++;		
		DATA0 = *buf++;	
		APP_EN = 1;		
		len--;
	}
	RAMMODE = 0x8F;
	while(len>=2)
	{	
		DATA3 = *buf++;		
		DATA2 = *buf++;
		DATA1 = *buf++;		
		DATA0 = *buf++;
		APP_EN = 1;		
		len -= 2;
	}
	if(len)
	{	
		RAMMODE = 0x8C;
		DATA3 = *buf++;		
		DATA2 = *buf++;
		APP_EN = 1;
	}
	RAMMODE = 0x00;
	EA=1;
} 
//��DGUS�е�VP��������
//addr:����ֱ�Ӵ���DGUS�еĵ�ַ
//buf:������
//len:��ȡ������,һ���ֵ���2���ֽ�
void sys_read_vp_nor(u16 addr,u8* buf,u16 len)
{   
	u8 i; 
	EA=0;
	i = (u8)(addr&0x01);
	addr >>= 1;
	ADR_H = 0x00;
	ADR_M = (u8)(addr>>8);
	ADR_L = (u8)addr;
	ADR_INC = 0x01;
	RAMMODE = 0xAF;
	while(APP_ACK==0);
	while(len>0)
	{   
		APP_EN=1;
		while(APP_EN==1);
		if((i==0)&&(len>0))   
		{   
			*buf++ = DATA3;
			*buf++ = DATA2;                      
			i = 1;
			len--;	
		}
		if((i==1)&&(len>0))   
		{   
			*buf++ = DATA1;
			*buf++ = DATA0;                      
			i = 0;
			len--;	
		}
	}
	RAMMODE = 0x00;
	EA=1;
}

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
	EA=0;
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
	EA=1;
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
	EA=0;
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
	EA=1;
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
	sys_write_vp_nor(CACHE_ADDR,buff,Len);
  
	temp[0] = 0xA5;
	temp[1] = (uint8_t)(NorAddr >> 16);
	temp[2] = (uint8_t)(NorAddr >> 8);
	temp[3] = (uint8_t)(NorAddr);
	temp[4] = (CACHE_ADDR>>8)&0xff;/**dgusii���ݵ�ַ**/
	temp[5] = CACHE_ADDR&0xff;
	temp[6] = (uint8_t)(Len >> 8);
	temp[7] = (uint8_t)(Len);
  sys_write_vp_nor(NOR_FLASH_RW_CMD,temp,4);
	do
	{
    DelayMs(5);
		sys_read_vp_nor(NOR_FLASH_RW_CMD,temp,2);
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
  	sys_write_vp_nor(NOR_FLASH_RW_CMD,temp,4);
	do
	{
    DelayMs(5);
sys_read_vp_nor(NOR_FLASH_RW_CMD,temp,2);
	}while(temp[0]!=0);
	sys_read_vp_nor(CACHE_ADDR,buff,Len);
	EA=1;
}
