#include "adc.h"

#define ADC_START_ADDR		0x32
//uint16_t ADC_FLAG=0;
//uint16_t ADC_channel=0;
//uint16_t cnt=0;
	u16 ad;
//	u16 max_ad = 0;
//	u16 min_ad = 0xFFFF;
	//float ad_sum = 0;
	
	float adc_08;
		float adc_09;
			float adc_10;
	uchar adc_channel=0x01;

	
	void bianliangchongzhi_adc();
//读取通道的ad值,不需要启动命令,屏幕上电之后,直接读取就行
//channel:0-7
u16 adc_read(u16 channel)
{
	sys_read_vp(ADC_START_ADDR+channel,(u8*)&channel,2);
	
	return channel;
}


void T_100MS_ADC_isr()
{
	
		ad = adc_read(adc_channel);

//			if(adc_channel<=0x01)adc_channel++;
//			else adc_channel=0x00;
	
	
	switch(adc_channel)
	{
		case 0x01:
		{
					adc_channel=0x03;
				adc_10=ad*(3300.0f/4095)*11/1000.0f*1.02;
			break;
		}
				case 0x03:
		{
							adc_channel=0x05;
				adc_09=ad*(3300.0f/4095)*2/1000.0f*1.01;
			break;
		}
				case 0x05:
		{
							adc_channel=0x01;
				adc_08=ad*(3300.0f/4095)*11/1000.0f*1.02;
			break;
		}
	}
}