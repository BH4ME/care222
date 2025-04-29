ORG 1000H
GOTO MAIN
NOP
NOP
NOP
ORG 1080H

MAIN:
LDWR       R0,1500H        ;读取1500H的数据到R10
MOVXR      R10,1,1         ;
LDWR       R20,100         ;将100写入R20
LDWR       R30,500         ;将500写入R30
CALL       比较             ;调出子程序比较
GOTO       MAIN            ;强制跳转到MAIN(8行)
;====================
比较:
JS         R10,R30,赋值200  ;比较R10与R30,大于则执行下一条,小于则跳转到赋值200(26行)
LDWR       R0,1520H        ;向1520H中写入300
LDWR       R40,300         ;
MOVXR      R40,0,1         ;
LDWR       R0,1510H        ;将1510H清零
MOVXR      R10,1,1;
IJNE       R11,0,清零       ;
GOTO       结束             ;

赋值200:
JS         R10,R20,中止     ;比较R10与R20,大于则执行下一条,小于则跳转到中止(44行)
LDWR       R0,1510H        ;向1510H中写入200
LDWR       R40,200         ;
MOVXR      R40,0,1         ;
LDWR       R0,1520H        ;将1520H清零
MOVXR      R10,1,1         ;
IJNE       R11,0,清零       ;
GOTO       结束             ;

清零:                       ;将1510H及1520H数据清零
LDWR       R0,1510H        ;
LDWR       R20,0           ;
MOVXR      R20,0,1         ;
LDWR       R0,1520H        ;
LDWR       R30,0           ;
MOVXR      R30,0,1         ;

中止:                       ;将1510H及1520H数据清零并跳转到结束
LDWR       R0,1510H;
LDWR       R20,0           ;
MOVXR      R20,0,1         ;
LDWR       R0,1520H        ;
LDWR       R30,0           ;
MOVXR      R30,0,1         ;
GOTO       结束             ;

结束:
RET                        ;跳转到第14行
