#include "buttom.h"

#define  BUTTON_MS  2000
#define  BUTTON  29
 u16 g_button_state[4]={0};
//����1ms ��ʱ  ���㰴ť�ó��̰�
//�2S ���س���
 
 	  static u16 time_ms[1]={0};
      static u16 time_ms_flag[1]={0}; //0����  1��ʼ��ʱ 2�̰�   3---2S����
	  static u16 time_lock[1]={0}; //���̰�����-----1����
		
		  uint  button_state[2]={0};
	  u16 i=0;	
void  button_long_short_cala(void)
{  


	  //��ʼ��ʱ
//	  for(i=0;i<BUTTON;i++)
//	  {
		 if(time_ms_flag[i]==1)
		 {
          time_ms[i]++;
		 }
		 if(time_ms[i]>=BUTTON_MS) //2S ǿ�Ƴ��� ��̰�����
		 {
			 time_lock[i]=1;
			 g_button_state[i]=2;
			 time_ms_flag[i]=0;
			 time_ms[i]=0;		 	 		 
		 } 
//	  }	
	  //�̰�����---��ʱʹ�� --��ռ�ʱ
	//  for(i=0;i<BUTTON;i++)
	//  {
		 ReadDGUS(0x5100+i,(u8*)&button_state[0],sizeof(button_state[0]));
		 if(button_state[0]==0x5A)
		 {
		    time_ms_flag[i]=1;
		    time_ms[i]=0;
			button_state[0]=0;
			WriteDGUS(0x5100+i,(u8*)&button_state[0],sizeof(button_state[0]));
		 }
	//  }	
		//��������---��ʱ����---ͳ��ʱ��
//	  for(i=0;i<BUTTON;i++)
	//  {
		 ReadDGUS(0x5200+i,(u8*)&button_state[0],sizeof(button_state[0]));
		 if(button_state[0]==0x5A)
		 {		 
			if(time_lock[i]==0) //����û�д����̰�---
			{
				g_button_state[i]=1;
			}		 
		    time_ms_flag[i]=0;
			time_ms[i]=0;		
			time_lock[i]=0; //����
			button_state[0]=0;
			WriteDGUS(0x5200+i,(u8*)&button_state[0],sizeof(button_state[0]));
		 }
	//  }
		
}

