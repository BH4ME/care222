/*
	HMI ��λ��ͨѶ  ModBusSlave 115200,n,8,1  ��ͨѶ��ַ 1 ���������߸����س���͸�����Դ�ļ�
	��������д�� HmiSlvBuff ��������
*/
#include "dgus.h"
#include "T5LUart4.h"
#include "crc16table.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "handle.h"
//��������
struct TX_SLVCMD_STRUCT data HmiSlvCmd;
union TX_SLVBUFF_STRUCT xdata HmiSlvBuff;
unsigned char xdata HmiSlvSendBuff[250]; //���ͱ��ĳ���


char uartRecStr[20] = {0};             //���ڽ����ַ����洢
unsigned char uartRecCnt = 0; 
uint rec_ok = 0; 
/*

//ʹ��˵�� ��������ʱ���� 5ms �����ͨѶ�ĳ�ʱ�쳣����
	if (HmiSlvCmd.Status==1){
		HmiSlvCmd.Delay++; 
		if (HmiSlvCmd.Delay>0) HmiSlvCmd.Status=2; 
	}
	if (HmiSlvCmd.Step>0 || HmiSlvCmd.Status>2) //ModBus �������ݳ�ʱ�ж�
	{
		HmiSlvCmd.TxTimeOut++; //��ʱ���շ���ʱ�� 100ms
		if (HmiSlvCmd.TxTimeOut>20)
		{
			HmiSlvCmd.TxTimeOut=0;
			HmiSlvCmd.Step=0;
			HmiSlvCmd.Status=0;
		}
	}
	else HmiSlvCmd.TxTimeOut=0;


*/
void Uart4SendByte(uint8_t dat)
{
  SBUF2_TX = dat;    
  while((SCON2T&0x01) == 0); 
  SCON2T &= 0xFE;  
}

char putchar(char c)
{
    Uart4SendByte(c);
    return c;
}



//����4�����ж�
void T5L_Uart4_TX() interrupt 10 using 2
{
	EA=0;
	SCON2T&=0xfe;

	if (HmiSlvCmd.Status==3)
	{
		HmiSlvCmd.TxTimeOut=0;
		if (HmiSlvCmd.Length>=HmiSlvCmd.SendLength)
		{
			HmiSlvCmd.Status=0;
			HmiSlvCmd.Step=0;
		}
		else SBUF2_TX=HmiSlvSendBuff[HmiSlvCmd.Length++];
	}
	EA=1;
}
//�����ж�
void T5L_Uart4_RX() interrupt 11 using 2
{
	unsigned int data rData,tmp; 
	uint8_t res=0;
	EA=0;
	rData=SBUF2_RX;
	res=SBUF2_RX;
	
		SCON2R&=0xfe;
//	  if((SCON2R&0x01)==0x01)
//  {

//		if( ( rData == '\r' ) && ( rData == '\n' ) )    //�ַ����Իس����з�����
//    {
//       rec_ok = 1; 
//    }
//    else
//    {
    //                                //�ý�����ɱ�־		
   // }
//	}
	//uartRecStr[uartRecCnt++] = res;
	
	if(uart4_open_buff[1])
	{
						
	    if( ( res != '\r' ) && ( res != '\n' ) )    //�ַ����Իس����з�����
    {
        uartRecStr[uartRecCnt++] = res;
    }
    else
    {
        rec_ok = 1;                             //�ý�����ɱ�־
    }
	}
	else
	{
	if (HmiSlvCmd.Status>0) 	
		return;
	HmiSlvCmd.TxTimeOut=0;
	switch (HmiSlvCmd.Step){
	case 0://�жϵ�ַ
		if (rData==HmiSlvCmd.Addr)
		{
			HmiSlvBuff.Value[0]=rData;
			HmiSlvCmd.Length=1;
			HmiSlvCmd.Step=1;
			HmiSlvCmd.JsCrc.Data=0xffff;
			tmp=CRC16_TABLE[(HmiSlvCmd.JsCrc.Data^rData)&0x00ff];
			HmiSlvCmd.JsCrc.Data=(HmiSlvCmd.JsCrc.Data>>8)^tmp;
		}
//		else if(rData==0x6c)
//		{
//			//printf("32");	
//			HmiSlvCmd.Step=3;
//			//uartRecCnt[0]=rData;
//		}
		break;
	case 1://��ȡ���İ�ͷ��
		HmiSlvBuff.Value[HmiSlvCmd.Length++]=rData;			
		if (HmiSlvCmd.Length<7)
		{
			tmp=CRC16_TABLE[(HmiSlvCmd.JsCrc.Data^rData)&0x00ff];
			HmiSlvCmd.JsCrc.Data=(HmiSlvCmd.JsCrc.Data>>8)^tmp;
		}
		if(HmiSlvCmd.Length==7 && HmiSlvBuff.Item.Command==0x10)
		{
			tmp=CRC16_TABLE[(HmiSlvCmd.JsCrc.Data^rData)&0x00ff];
			HmiSlvCmd.JsCrc.Data=(HmiSlvCmd.JsCrc.Data>>8)^tmp;				
			HmiSlvCmd.RxLength=9+HmiSlvBuff.ItemX.ByteCount;
			if(HmiSlvCmd.RxLength>252) 
			{
				HmiSlvCmd.Step=0;
				break;
			}
			HmiSlvCmd.Step=2;
			break;
		}			
		if (HmiSlvCmd.Length>=8){ //������3 �� 6
			if (HmiSlvCmd.JsCrc.Value[0]==HmiSlvBuff.Item.LowCrc && HmiSlvCmd.JsCrc.Value[1]==HmiSlvBuff.Item.HiCrc)
			{//������ȷ					
				HmiSlvCmd.Delay=0;
				HmiSlvCmd.Status=2; //���ݽ������   ͨѶת����Ҫ�ӳ� Status=1
			}
			HmiSlvCmd.Step=0;
		}
		break;
	case 2: //0x10
		HmiSlvBuff.Value[HmiSlvCmd.Length++]=rData;			
		if (HmiSlvCmd.Length<(HmiSlvCmd.RxLength-1))
		{
			tmp=CRC16_TABLE[(HmiSlvCmd.JsCrc.Data^rData)&0x00ff];
			HmiSlvCmd.JsCrc.Data=(HmiSlvCmd.JsCrc.Data>>8)^tmp;
		}
		if (HmiSlvCmd.Length>=HmiSlvCmd.RxLength){
			if (HmiSlvCmd.JsCrc.Value[0]==HmiSlvBuff.Value[HmiSlvCmd.RxLength-1] && HmiSlvCmd.JsCrc.Value[1]==HmiSlvBuff.Value[HmiSlvCmd.RxLength-2])
			{//������ȷ
				
				HmiSlvCmd.Delay=0;
				HmiSlvCmd.Status=2; //���ݽ������  ͨѶת����Ҫ�ӳ� Status=1
			}
			HmiSlvCmd.Step=0;
		}			
		break;
//		
//	case 3:
//	{
//		
//		//	printf("32");	
//			HmiSlvCmd.Step=4;
//			uartRecCnt[1]=rData;
//		break;
//	}
//		case 4:
//	{
//		
//			printf("0d");	
//			//HmiSlvCmd.Step=5;
//			uartRecCnt[2]=rData;
//			rec_ok = 1; 
//			HmiSlvCmd.Step=0;
//		break;
//	}
	
	default:
		HmiSlvCmd.Step=0;
		break;
	}	
}

	EA=1;
}

//��ʼ��ʾͨѶ����
void T5L_HMIslvInit()
{
	HmiSlvCmd.Addr=1; //ModBus ��ַΪ1
	HmiSlvCmd.Status=0;
	HmiSlvCmd.Step=0;	
	HmiSlvCmd.TxTimeOut=0;
}
//������������
unsigned char HMICommand()
{
	unsigned char i;
	unsigned int tmp;
	unsigned char retVal;
// ����0 ���������� ����1��������
	retVal=0;
	switch (HmiSlvBuff.Item.Command)
	{
	case 3: //��ȡ
		if (HmiSlvBuff.Item.DataValue==0 || HmiSlvBuff.Item.DataValue>115)
		{//�쳣���ȡ���ݽ�ֹ����		
			return 0;
		}
		HmiSlvSendBuff[0]=HmiSlvCmd.Addr;
		HmiSlvSendBuff[1]=3;
		HmiSlvSendBuff[2]=HmiSlvBuff.Item.DataValue*2;
		HmiSlvCmd.SendLength=HmiSlvSendBuff[2]+5;
		tmp=HmiSlvBuff.Item.RegisterAddr*2;
		ReadDGUS(HmiSlvBuff.Item.RegisterAddr,(u8*)&HmiSlvSendBuff[3],HmiSlvSendBuff[2]);
		retVal=1;
		break;
	case 6: //д��
		WriteDGUS(HmiSlvBuff.Item.RegisterAddr,&HmiSlvBuff.Value[4],2);
		HmiSlvCmd.SendLength=8;
		for (i=0;i<6;i++)
		{
			HmiSlvSendBuff[i]=HmiSlvBuff.Value[i];
		}
		retVal=1;
		break;
	case 16: //����д��
		if(HmiSlvBuff.ItemX.DataValue==0 || HmiSlvBuff.ItemX.DataValue>125) return 0; //���ܳ���125�� word
		WriteDGUS(HmiSlvBuff.Item.RegisterAddr,HmiSlvBuff.ItemX.uint8,HmiSlvBuff.ItemX.ByteCount);		
		HmiSlvCmd.SendLength=8;
		for (i=0;i<6;i++)
		{
			HmiSlvSendBuff[i]=HmiSlvBuff.Value[i];
		}
		retVal=1;
		break;
	default:
		return 0;
	}
	return retVal;
}
//Զ��ͨѶ��Ӧ�ӳ��� ����2 ��������
unsigned char T5L_HMIslvSub()
{
	unsigned char i;
	unsigned int tmp;
	unsigned char retVal;
	EA=0;
	if (HmiSlvCmd.Status!=2) return 0;
	retVal=HMICommand();
	if (retVal==0)
	{
		HmiSlvCmd.Status=0;//�޷���
		HmiSlvCmd.Step=0;
		return 0;
	}
	
	HmiSlvCmd.JsCrc.Data=0xffff;
	for (i=0;i<HmiSlvCmd.SendLength-2;i++){//�����ͷ��CRC
		tmp=CRC16_TABLE[(HmiSlvCmd.JsCrc.Data^HmiSlvSendBuff[i])&0x00ff];
		HmiSlvCmd.JsCrc.Data=(HmiSlvCmd.JsCrc.Data>>8)^tmp;
	}
	HmiSlvSendBuff[HmiSlvCmd.SendLength-2]=HmiSlvCmd.JsCrc.Value[1];
	HmiSlvSendBuff[HmiSlvCmd.SendLength-1]=HmiSlvCmd.JsCrc.Value[0];

	//��������
	HmiSlvCmd.Step=0;
	HmiSlvCmd.Length=1;
	HmiSlvCmd.Status=3;
	SBUF2_TX=HmiSlvSendBuff[0];
	return retVal;
	EA=1;
}

