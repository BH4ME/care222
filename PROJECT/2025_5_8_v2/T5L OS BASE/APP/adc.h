#ifndef __ADC_H__
#define __ADC_H__
#include "sys.h"

#define ADC_08_AD 0X4300
#define ADC_09_AD 0X4400
#define ADC_10_AD 0X4500

extern u32 ADC_100ms;
extern void T_100MS_ADC_isr();
#endif