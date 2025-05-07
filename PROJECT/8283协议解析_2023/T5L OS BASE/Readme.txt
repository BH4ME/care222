MODE =1(气溶胶模式、区域照射、中心照射)，=2（治疗模式），=3（程序化模式），=4（手动模式）

dgus.c 
mode_p.c		治疗模式处理文件		
mode_s.c		气溶胶模式、区域照射、中心照射处理文件
suanfa.c
i2c.c		RTC处理文件
rtc_sys.c		RTC操作DGUS寄存器文件
sys_params.c	FlashROM处理函数
uart4.c		串口4连接上位机反馈函数
cmd.c		串口4连接上位机指令代码函数
start_win.c	画功率图处理函数
mode_3.c		程序化模式处理函数
mode_4.c		手动模式处理函数
T5LUart4.c	串口4下载工程文件协议函数