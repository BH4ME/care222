#include  "home.h"
#include "dgus.h"
#include "qrj.h"
uint8_t CurrentPageNum[2]={0};	//CurrentPageNum[1]�ǵ�ǰҳ��

//������������ѭ��ִ�е�ǰҳ��ĳ���
void ReadCurrentPage_run()		
{

	ReadDGUS(0x14,(uint8_t *)&CurrentPageNum,sizeof(CurrentPageNum));
	switch(CurrentPageNum[1])
	{
		case Page_home:	
		{
			QRJ_run();
			break;
		}
		
		case Page_qirongjiao:
		{
			
			break;
		}
			
		case Page_zhongxin:
		{

			break;
		}
		
		case Page_quyu:
		{
			break;
		}
		
				
		case Page_chengxuhua:
		{

			break;
		}
		
				
		case Page_zhiliao:
		{
			break;
		}
		
				
		case Page_shoudong:
		{		

			break;
		}
					
		case Page_lianji:
		{
	
			break;
		}
		
				
		case Page_shezhi:
		{

			break;
		}
		
		case Page_chengxuhua_JUMP:
		{
			
			break;
		}
				
			case Page_zhiliao_JUMP:
		{
			break;
		}
		default:
		{
			
			break;
		}
		
	}
	
}

