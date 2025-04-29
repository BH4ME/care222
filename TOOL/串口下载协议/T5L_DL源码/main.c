/*

���: GD
��ʼ����: 2021.6.2
�汾��־��

      2021.6.2 V0.1 ��ɻ���ModBusRTU Slave ������3 6 16 ����ͨ��
			   ��ɺ�Dgus �ڴ�����ݽ���ָ��,ʵ��������д��Դ�ļ��ͳ���
      2021.6.5 V0.2 ��PC����������������ͨ��
      2021.6.20 V1.0 ������뷽����ֲ

*/
#include ".\T5LDriver\T5LDriver.h"
#include ".\T5LDriver\T5LUart4.h"

void InitSystem();

//��ʱ��2�ж� 5ms�ж�һ��
void T5L_Time2() interrupt 5 using 2
{
    TF2=0; //����ж�
	//ͨѶ���ճ�ʱ
	if (HmiSlvCmd.Status==1){
		HmiSlvCmd.Delay++; 
		if (HmiSlvCmd.Delay>1) HmiSlvCmd.Status=2; 
	}
	if (HmiSlvCmd.Step>0 || HmiSlvCmd.Status>2) //ModBus �������ݳ�ʱ�ж�
	{
		HmiSlvCmd.TxTimeOut++; //��ʱ���շ���ʱ��>100ms
		if (HmiSlvCmd.TxTimeOut>20)
		{
			HmiSlvCmd.TxTimeOut=0;
			HmiSlvCmd.Step=0;
			HmiSlvCmd.Status=0;
		}
	}
	else HmiSlvCmd.TxTimeOut=0;	
}

void main()
{
    InitSystem();
    while(1)
    {
		T5L_HMIslvSub(); //���ܣ����߸��´���������
    }
}

void InitSystem()
{
    T5L_InitCPU();
	T5L_HMIslvInit(); //��ʼ��ͨѶ����
    EA=1;
}