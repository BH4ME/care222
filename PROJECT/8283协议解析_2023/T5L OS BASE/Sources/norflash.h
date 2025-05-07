/******************************************************************************
�� �� ��   : norflash.h
�� �� ��   : V1.0
��    ��   : pinot
��������   : 2019��11��18��
��������   : ��дNorFlash
�޸���ʷ   :
��    ��   :
��    ��   :
�޸�����   :
******************************************************************************/

#ifndef _NORFLASH_H_
#define _NORFLASH_H_

/*****************************************************************************
ϵͳ��*/

/*****************************************************************************
�Զ���ͷ�ļ�*/
#include "T5LOS8051.H"
#include "sys.h"
#include "dgus.h"

/*****************************************************************************
�궨�����*/
#define NOR_FLASH_RW_CMD 0x0008
#define CACHE_ADDR 0xf000			/**Dgus������***/
/*****************************************************************************
���⺯������*/
void ReadNorFlash_Dgus(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len);
void WriteNorFlash_Dgus(uint32_t NorAddr, uint32_t DgusAddr, uint16_t Len);
void ReadNorFlash_Str(uint32_t NorAddr, uint8_t* buff, uint16_t Len);
void WriteNorFlash_Str(uint32_t NorAddr, uint8_t* buff, uint16_t Len);
#endif