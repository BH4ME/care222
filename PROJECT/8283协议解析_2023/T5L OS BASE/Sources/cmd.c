#include <string.h>
#include "cmd.h"
#include "uart.h"
#include "stdio.h"
#include "T5LUart4.h"
static CMD_LIST command_list = {NULL, 0};  // ȫ�������б�������ע�������
/*
* �������ܣ� ����ע�ắ�� ÿ�����һ���������ӵ������б���
* ��������� reg_cmds ��ע������ṹ������
*            length   �������
* ��������� ��
* ����ֵ ��  ��
* ��    ע�� length ���ó��� MAX_CMDS_COUNT  
*/
void register_cmds( CMD_Name_Func reg_cmds[], int length )
{
    int i;
    if ( length > MAX_CMDS_COUNT )
    {
        return;
    }
 
    for ( i = 0; i < length; i++ )
    {
        if ( command_list.num < MAX_CMDS_COUNT ) // �����б�δ��
        {
					//printf("��ʼ��");
            strcpy( command_list.cmdNames[command_list.num].cmd_name, reg_cmds[i].cmd_name );       //���ַ�����������б���
            command_list.cmdNames[command_list.num].cmd_functions = reg_cmds[i].cmd_functions;          //�������Ӧ�ĺ����洢���б���
            command_list.num++;                                                                     // ����ֵĬ��Ϊ0��ÿ���һ�����������1.             
        }
    }
}
 
/*
* �������ܣ� ����ƥ��ִ�к���
* ��������� cmdStr ��ƥ�������ַ���
* ��������� ��
* ����ֵ ��  ��
* ��    ע�� cmdStr ���Ȳ��ó��� MAX_CMD_NAME_LENGTH
*/
void match_cmd( char *cmdStr )
{
    int i;

    if ( strlen( cmdStr ) > MAX_CMD_LENGTH )
    {
        return;
    }
 
    for ( i = 0; i < command_list.num; i++ )	                                                    // ���������б�
    {
        if ( strcmp( command_list.cmdNames[i].cmd_name, cmdStr ) == 0 )                             //�ȽϽ��յ��������ַ��� �� �б��д洢�������ַ����Ƿ���ȣ������Ⱦ͵��������ַ�����Ӧ�ĺ�����
        {
            command_list.cmdNames[i].cmd_functions();
        }
    }

}