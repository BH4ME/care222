/******************************************************************************
文 件 名   : uart.h
版 本 号   : V1.0
作    者   : pinot
生成日期   : 2019年10月21日
功能描述   : UART2345串口读写
修改历史   :
日    期   :
作    者   :
修改内容   :
******************************************************************************/

#ifndef _UART_H_
#define _UART_H_

/*****************************************************************************
系统库*/
#include "string.h"

/*****************************************************************************
自定义头文件*/
#include "sys.h"

#include <stdio.h>
/*****************************************************************************
宏定义变量*/
#define UART2_MAX_LEN  8096
extern uint16_t uart2_rx_count;
extern uint8_t xdata  Uart2_Rx[UART2_MAX_LEN];

#define UART3_MAX_LEN  128
extern uint16_t uart3_rx_count;
extern uint8_t xdata  Uart3_Rx[UART3_MAX_LEN];

#define UART4_MAX_LEN 128
extern uint16_t uart4_rx_count;
extern uint8_t xdata Uart4_Rx[UART4_MAX_LEN];

#define UART5_MAX_LEN 128
extern uint16_t uart5_rx_count;
extern uint8_t xdata Uart5_Rx[UART5_MAX_LEN];

/*****************************************************************************
对外函数声明*/
void InitUart(void);

void Uart2Init(void);
void Uart2SendByte(uint8_t dat);
void Uart2SendStr(uint8_t *pstr,uint8_t strlen);
void Uart3Init(void);
void Uart3SendByte(uint8_t dat);
void Uart3SendStr(uint8_t *pstr,uint8_t strlen);
extern void UART5_isr(void);		//串口中断3响应函数
void Uart4Init(void);
void Uart4SendByte(uint8_t dat);
void Uart4SendStr(uint8_t *pstr,uint8_t strlen);
void Uart5Init(void);
void Uart5Init(void);
void Uart5SendStr(uint8_t *pstr,uint8_t strlen);

#endif
