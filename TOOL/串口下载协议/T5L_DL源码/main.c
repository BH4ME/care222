/*

设计: GD
开始日期: 2021.6.2
版本日志：

      2021.6.2 V0.1 完成基础ModBusRTU Slave 功能码3 6 16 测试通过
			   完成和Dgus 内存的数据交互指令,实现在线烧写资源文件和程序。
      2021.6.5 V0.2 与PC程序中联调，测试通过
      2021.6.20 V1.0 整理代码方便移植

*/
#include ".\T5LDriver\T5LDriver.h"
#include ".\T5LDriver\T5LUart4.h"

void InitSystem();

//定时器2中断 5ms中断一次
void T5L_Time2() interrupt 5 using 2
{
    TF2=0; //清除中断
	//通讯接收超时
	if (HmiSlvCmd.Status==1){
		HmiSlvCmd.Delay++; 
		if (HmiSlvCmd.Delay>1) HmiSlvCmd.Status=2; 
	}
	if (HmiSlvCmd.Step>0 || HmiSlvCmd.Status>2) //ModBus 接收数据超时判断
	{
		HmiSlvCmd.TxTimeOut++; //超时接收发送时间>100ms
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
		T5L_HMIslvSub(); //功能：在线更新触摸屏程序
    }
}

void InitSystem()
{
    T5L_InitCPU();
	T5L_HMIslvInit(); //初始化通讯参数
    EA=1;
}