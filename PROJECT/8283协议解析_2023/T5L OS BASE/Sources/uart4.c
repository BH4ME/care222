#include "uart4.h"
#include "T5LUart4.h"

 	// uint16_t TIME_Height[3]={0};
void led1_on( void )
{  
//uint16_t x=0x1234;
//    LED1 = 1;

					printf("\n	接收正常 	\n\n\n\n");
}


void led1_off( void )
{
uint i;
	uint8_t dat[16]={0};
	u16 dat_Ad[2];
	uint16_t Ad4=0x0028;
//	uint Adress;
	uint beishu;
	//uint16_t =ad_0x8;
	DelayMs(5);
//	printf("\n		正在读取数据中.....");
	DelayMs(5);
	

	ReadNorFlash_Str(0x0,(uint8_t*)&dat_Ad,2);
	printf("\n	当前地址 store adress	%hx",dat_Ad[1]);DelayMs(5);
	//ReadNorFlash_Str(Adress,(uint8_t*)&dat,8);

	//beishu=(Adress/8)-5;
	beishu=0x4000-0x28;


	for(i=0;i<beishu;i++)
	{
	ReadNorFlash_Str(Ad4,(uint8_t*)&dat,8);
		DelayMs(5);
		
		if(dat[0]==0xaa)
		{
			if(dat[7]==0xff){printf("\n	状态：关机");}
			else if(dat[7]==0xbb){printf("\n	状态：开机");}
			else if(dat[7]==0xcc){printf("\n	状态：开启UV");

					switch(dat[1])
		{
			//case 0:{JumpPage(Page_home);	break;}
			
			case 1:{printf("\n	模式：气溶胶模式");	break;	}
				
			case 2:	{printf("\n	模式：中心消毒模式");	break;}
			
			case 3:	{printf("\n	模式：区域消毒模式");	break;}

			case 4:	{printf("\n	模式：程序化消毒模式");	break;}

			case 5:	{printf("\n	模式：治疗模式");	break;}
			
			case 6:	{printf("\n	模式：手动模式");	break;}
			
			case 10:{printf("\n	模式：程序化消毒模式");break;}
						case 11:	{printf("\n	模式：治疗模式");	break;}
			default :break;
			
			}
		}
			else if(dat[7]==0xee){printf("\n	状态：关闭UV");}
			else if(dat[7]==0xdd){printf("\n	状态：暂停UV");}
			
					DelayMs(5);
	printf("\n	当前地址：%hx			时间： 20%2bd年	%2bd月	%2bd日		%2bd：%2bd：%2bd	\n\n\n\n",Ad4,dat[8],dat[9],dat[10],dat[12],dat[13],dat[14]);

			
		}
		else 
		{
					DelayMs(5);
					printf("\n	当前地址：%hx			未收到有效值	\n\n\n\n");
		}

	DelayMs(15);
		if(Ad4>=0x4000)
		{
			//Ad4=0x28;
			break;
		}
		else {Ad4=0x8+Ad4;}
	}
	
		printf("\r\n 	读取完成	\n\n\n\n");
   // LED1 = 0;
}
void led2_on( void )
{
	uint i;
	uchar dat_txst[2]={0x0,0x28};
	uint clean_0[8]={0};
	WriteNorFlash_Str(0x0,(uint8_t*)&dat_txst,2);
	printf("\r\n CLEAN NORFLASH (0x0~0x1),0x01 WRITE 0x28....NEXT DATA WILL WRITE IN 0x28	\n\n\n\n");
	for(i=0;i<=1022;i++)
	{
		WriteNorFlash_Str(0x28+(0x10*i),(uint8_t*)&clean_0,10);
		printf("\r\n CLEAN NORFLASH ADRESS	%hx \n\n\n\n",0x28+(0x10*i));
	}
	
		printf("\r\n CLEAN FINISH	!!!!!!!	");
}
 

	
void led2_off( void )
{
	uint Adress_Flag=1;	u16 dat_Ad[2];
		ReadNorFlash_Str(0x0,(uint8_t*)&dat_Ad,2);

	
	DelayMs(5);
	//printf("正在读取数据中....\n\n\n\n");
	DelayMs(5);
	
	printf("\n	当前地址 store adress	%hx\n\n\n\n",dat_Ad[1]);DelayMs(5);
//find_day();
}

 /***********************/
void led3_off( void )
{
   // LED3 = 0;

	
}
 
//注册led命令
void led_register( void )
{
	
    //初始化 字符串命令和对应函数
    CMD_Name_Func led_cmds[] =
    {
        {"UV TEST", led1_on},                       // 一个结构体变量大小为 12 (字符串大小10 + 函数名大小2)
        {"UV READ", led1_off},                     // 一个结构体变量大小为 12
        {"UV CLEAN", led2_on},
        {"UV ADRESS", led2_off},
//				{"UV ", led3_on},
//        {"led3 off", led3_off}
    };
 //printf("\n任务注册成功");
    //将命令添加到列表中
    register_cmds( led_cmds, ARRAY_SIZE( led_cmds ) );	// ARRAY_SIZE 用来计算结构体数组中，数组的个数。个数=结构体总长度/单个数组长度

}