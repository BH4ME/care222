ORG 1000H ;意义：接下来的指令放在0x1000地址
GOTO MAIN ;强制跳转到MAIN
NOP       ;无操作
NOP
NOP
ORG 1080H

MAIN:
LDWR R0,密码   ;将0x2000变量地址写入R0，“密码”会自动编译为0x2000
MOVXR R10,1,1 ;将R0所代表变量地址中 1 word长的数值写入R10
LDWR R20,1314 ;把1314（不带H即为十进制数，后同）写入R20
CALL 比较
GOTO MAIN

比较:
TESTS R10,R20,2  ;比较R10与R20中的值，如果相等返回0x00到R0，如果不等则返回R10中的地址到R0（此地址不为0）
IJNE R0,0,密码错误  ;比较R0的值跟0，如果不相等则执行错误密码子程序，如果相等执行下一条程序
GOTO 开机

开机:
IJNE R255,0,结束 ;比较R255的值跟0，如果不相等就执行结束，相等则执行下一条
LDWR R0,0084H
LDWR R30,5A01H
LDWR R32,0001H  
MOVXR R30,0,2  ;执行后将向0x0084地址写入0x5A01 0001，效果是切换页面到01页
LDBR R255,1,1    ;向R255中写入1

密码错误:
IJNE R255,0,结束 ;比较R255的值跟0，如果不相等就执行结束，相等则执行下一条
LDWR R0,0084H
LDWR R30,5A01H
LDWR R32,0003H 
MOVXR R30,0,2  ;执行后将向0x0084地址写入0x5A01 0003，效果是切换页面到03页
LDBR R255,1,1    ;向R255中写入1

结束:
END ;注意，END并不是程序不再执行，而是从第一行开始重新执行



















