#ifndef _T5L_UART4_H_
#define _T5L_UART4_H_

union TX_DREVCRC{
	unsigned char Value[2];
	unsigned int Data;
};

//�������ݽ����ṹ 3  6
struct TX_SLV_UNIONHEAD{
	unsigned char Address; //������ַ
	unsigned char Command;//������
	unsigned int  RegisterAddr ;//�Ĵ�����ַ
	unsigned int  DataValue; // ����/��ֵ
	unsigned char HiCrc; //
	unsigned char LowCrc;
};

struct TX_SLV_UNIONHEAD16{
	unsigned char Address; //������ַ
	unsigned char Command;//������
	unsigned int  RegisterAddr ;//�Ĵ�����ַ
	unsigned int  DataValue; // ����/��ֵ
	unsigned char ByteCount; //�ж��ٸ��ֽ�����
	unsigned char uint8[250];
};

union TX_SLVBUFF_STRUCT{
	struct TX_SLV_UNIONHEAD Item;
	struct TX_SLV_UNIONHEAD16 ItemX;
	unsigned char Value[300];
};

//ͨѶ����Э���ṹ
struct TX_SLVCMD_STRUCT{
	unsigned char Addr; //������ַ
	unsigned char Step;
	unsigned char Length;
	unsigned char RxLength;
	unsigned char SendLength;
	union TX_DREVCRC JsCrc;
	unsigned char Delay;
	unsigned char Status; //=0 ����״̬, =1 ���û�ͨѶת����Ҫ�ӳ�, =2 ��������
	unsigned char TxTimeOut; //�������ݼ�ϳ�ʱ
};

extern struct TX_SLVCMD_STRUCT data HmiSlvCmd;
extern union TX_SLVBUFF_STRUCT xdata HmiSlvBuff;

void T5L_HMIslvInit();
unsigned char T5L_HMIslvSub();
#endif