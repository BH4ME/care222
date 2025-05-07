#include "adc.h"
#include "dgus.h"


#define ADC_START_ADDR		0x32

u8 adc_channel=0x01;



//读取通道的ad值,不需要启动命令,屏幕上电之后,直接读取就行
//channel:0-7
u16 adc_read(u16 channel)
{
	ReadDGUS(ADC_START_ADDR+channel,(u8*)&channel,sizeof(channel));
	
	return channel;
}



u32 ADC_100ms=0;
//100ms调用一次
void T_100MS_ADC_isr()
{
	u16 ad;
	static float adc_08;
	static float adc_09;
	static float adc_10;
	static float I;	
	ad = adc_read(adc_channel);

	switch(adc_channel)
	{
		case 0x01:
		{
			adc_channel=0x03;
			adc_10=ad*(3300.0f/4095)*11/1000.0f*1.02;
			
			WriteDGUS(ADC_10_AD,(uint8_t *)&adc_10,sizeof(adc_10));
			break;
		}
		case 0x03:
		{
			adc_channel=0x05;
			adc_09=ad*(3300.0f/4095)*2/1000.0f*1.01;
					
			WriteDGUS(ADC_09_AD,(uint8_t *)&adc_09,sizeof(adc_09));
			break;
		}
		case 0x05:
		{
			adc_channel=0x01;
			adc_08=ad*(3300.0f/4095)*11/1000.0f*1.02;
			
			I=(adc_10-adc_08)/0.5*1.02;
			WriteDGUS(ADC_08_AD,(uint8_t *)&I,sizeof(I));
			
			break;
		}
		
	}
}