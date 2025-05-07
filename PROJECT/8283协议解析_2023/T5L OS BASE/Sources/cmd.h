#ifndef __CMD_H_
#define __CMD_H_
#include "T5LOS8051.H"
#define ARRAY_SIZE(x)	(sizeof(x) / (sizeof((x)[0])))        //��������ṹ�������У�����ĸ���������=�ṹ���ܳ���/�������鳤��
#define		MAX_CMD_LENGTH		15	                // �������������
#define		MAX_CMDS_COUNT		20	                // ���������
typedef void ( *functions )( void );  	                        // �����������ָ������
//����ṹ������ ���ڴ洢�ַ�������Ͷ�Ӧ����
typedef struct
{
    char cmd_name[MAX_CMD_LENGTH + 1];                          // ������ �ַ���ĩβϵͳ���Զ���ӽ�����'/0'       sizeof("name")��СΪ 10
    functions cmd_functions;			  	        // �����������     sizeof(func) ��СΪ 2
}CMD_Name_Func;
// �����б�ṹ������  ���ڴ洢�ַ�����������
typedef struct
{
    CMD_Name_Func cmdNames[MAX_CMDS_COUNT];                     // �洢�ַ������� �Ͷ�Ӧ����
    int num;	                                                // �����������
}CMD_LIST;
 
 
void register_cmds( CMD_Name_Func reg_cmds[], int num );
void match_cmd( char *str );
 
#endif
 
 