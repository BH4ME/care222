#ifndef __UART4_H
#define __UART4_H
#include "T5LOS8051.H"
#include "string.h"
#include "cmd.h"
 #include <string.h>
#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include "norflash.h"
#include "sys_params.h"
extern uint16_t TIME_Height[3];
void led1_on(void);
void led1_off(void);
void led2_on(void);
void led2_off(void);
void led3_on(void);
void led3_off(void);
void led_register(void);
void Read_Month_To_Draw();
#endif