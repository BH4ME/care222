#include "buttom.h"

#define  BUTTON_MS  2000
#define  BUTTON  29
 u16 g_button_state[4]={0};
//放入1ms 计时  计算按钮得长短按
//最长2S 返回长按
 
 	  static u16 time_ms[1]={0};
      static u16 time_ms_flag[1]={0}; //0结束  1开始计时 2短按   3---2S长按
	  static u16 time_lock[1]={0}; //长短按的锁-----1上锁
		
		  uint  button_state[2]={0};
	  u16 i=0;	
void  button_long_short_cala(void)
{  


	  //开始计时
//	  for(i=0;i<BUTTON;i++)
//	  {
		 if(time_ms_flag[i]==1)
		 {
          time_ms[i]++;
		 }
		 if(time_ms[i]>=BUTTON_MS) //2S 强制长按 与短按互锁
		 {
			 time_lock[i]=1;
			 g_button_state[i]=2;
			 time_ms_flag[i]=0;
			 time_ms[i]=0;		 	 		 
		 } 
//	  }	
	  //短按按下---计时使能 --清空计时
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
		//长按按下---计时结束---统计时常
//	  for(i=0;i<BUTTON;i++)
	//  {
		 ReadDGUS(0x5200+i,(u8*)&button_state[0],sizeof(button_state[0]));
		 if(button_state[0]==0x5A)
		 {		 
			if(time_lock[i]==0) //长按没有触发短按---
			{
				g_button_state[i]=1;
			}		 
		    time_ms_flag[i]=0;
			time_ms[i]=0;		
			time_lock[i]=0; //解锁
			button_state[0]=0;
			WriteDGUS(0x5200+i,(u8*)&button_state[0],sizeof(button_state[0]));
		 }
	//  }
		
}

