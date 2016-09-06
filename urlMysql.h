#ifndef _URLMYSQL_H_INCLUDED_
#define _URLMYSQL_H_INCLUDED_
#include "include\mysql.h"
////---------------- struct ----------------------

struct files
{
	char dir[256] = { "null" };
	char file_name[1024] = { "null" };
	char file_time[256] = { "null" };
	char file_hash[1024] = { "null" };
	char file_state[2] = "";
};
enum state { file_empty };
////---------- functions ------------------------

void initMysql();//��ʼ��Mysql����
int fileState(const char * Paht, char* find_rule = "*");//�����ļ���
void insert_file(files& file);
void updata_file(files& file);
int findFileForSql(char* dir,files & file);
void gitDirTime(char* dir, time_t& dir_time);

#endif