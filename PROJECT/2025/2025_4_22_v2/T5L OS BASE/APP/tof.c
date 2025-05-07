#include "tof.h"
#include "sys.h"
#include "dgus.h"
//������ ����λ��USART3����isr��

//u8 test_error[9]={1,2,3,4,5,6,7,8,9};

//״̬��   =0  �ȴ���һ��5A   =1  �ȴ��ڶ���5A =>3 �ڼ�������
  //У���
uint8_t GY53_Data[16];//���ܵ�����������
uint32_t L_int;	//����
//bit L_change=0;
uint8_t MODE;
/***************************������********************/
void UART5_isr()
{
	uint8_t DATA15=00; 
	static uint8_t UART5_FLAG =0;	//ֻ��ֵһ��
	uint8_t Dat;
	 Dat = SBUF3_RX;    						//������ε�����	
	//Uart2SendByte((uint8_t *)&L);
	//DAT_W_EEPROM = EEPROM_Read(0);
	//UART_Send_byte(1,KEY_FLAG);						//�ô���1�Ѵ���2�����ݷ�����
	switch(UART5_FLAG)
	{
		case 0:
		{
			if(Dat == 0x57)     						//�жϵ�һ��5A
			{
				GY53_Data[UART5_FLAG] = Dat;   //������
				UART5_FLAG=1;      					 //״̬��1
			}
			break;
		}
		case 1:
		{
			if(Dat == 0x00)    							//�жϵڶ���5A
			{
				GY53_Data[UART5_FLAG] = Dat;   //������
				UART5_FLAG=2;      						 //״̬��2
			}
			else
			{
				UART5_FLAG = 0;       				//״̬��0
			}
			break;
		}
		default:
		{
					GY53_Data[UART5_FLAG] = Dat;
					UART5_FLAG++;
					if(UART5_FLAG>0x0F) 									//00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 
			{

					UART5_FLAG = 0;       								//״̬��0				
					DATA15 = GY53_Data[0X00]+GY53_Data[0X01]+GY53_Data[0X02]+GY53_Data[0X03]+GY53_Data[0X04]+GY53_Data[0X05]+GY53_Data[0X06]+GY53_Data[0X07]+GY53_Data[0X08]+GY53_Data[0X09]+GY53_Data[0X0A]+GY53_Data[0X0B]+GY53_Data[0X0C]+GY53_Data[0X0D]+GY53_Data[0X0E]; //����У���

				
				if(DATA15 == GY53_Data[0X0F])  					//����ʱ��ɹ�
				{
				//L[0] = ((GY53_Data[0X0A]<<16 | GY53_Data[0X09]<<8) | GY53_Data[0X08]); 
					
					/*************���״̬λ�Ƿ�Ϊ0********************/
					if(GY53_Data[0X0b]==0)
					{

					L_int=((GY53_Data[0X0A]<<16 | GY53_Data[0X09]<<8) | GY53_Data[0X08]);

//						if(L_change==1)
//						{
//							L_change=0;
//							if(MODE==1)WriteDGUS(0x4913,(uint8_t *)&(colour_w),sizeof(colour_w));
//								else
//							WriteDGUS(L_sp_ad+0x3,(uint8_t *)&(colour_w),sizeof(colour_w));
//						}
						
					}
//					else 	{L_change=1;
//					if(MODE==1)WriteDGUS(0x4913,(uint8_t *)&(colour_y),sizeof(colour_y));
//					else WriteDGUS(L_sp_ad+0x3,(uint8_t *)&(colour_y),sizeof(colour_y));}	
//					
					
					//(GY53_Data[0X0A]     GY53_Data[0X09]      GY53_Data[0X08]   //1000=xxx.m

				}
			}
			break;
		}
	}
}
