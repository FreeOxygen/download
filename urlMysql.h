#ifndef _URLMYSQL_H_INCLUDED_
#define _URLMYSQL_H_INCLUDED_
#include <string.h>
////---------------- struct ----------------------

struct files
{
	char dir[256] = { "null" };
	char file_name[1024] = { "null" };
	char file_time[256] = { "null" };
	char file_hash[1024] = { "null" };
	char file_state[2] = "";
};

enum DL_state { DL_WAIT, DL_RUN, DL_FAIL, DL_SUCCESS };//���ص�״̬
enum DL_protocol { http_https, magnet, ED2K, ftp };//����Э��

struct url_info
{
	int DJID ;//����ID
	int JID ;//��������ID
	char url[1024];//url
	int RETRY_COUNT;//���ش���
	char filename[1024];//��������ļ�������
	int filesize;//��������ļ���С
	char filepath[1024];//���ش�ŵ�·��
	int progress;//���ؽ���
	DL_protocol protocol ;//����ʹ�õ�Э��
	DL_state state ;//����״̬
	char remark[256] = "";//����ʧ��˵��
	char start_time[64] = "";//���ؿ�ʼʱ��
	char end_time[64] = "";//�������ʱ��
	char update_time[64] = "";//����ʱ��
	char tool[256] = { "thunder" };//ʹ�õ����ع���
};
////---------- functions ------------------------

void initMysql();//��ʼ��Mysql����
int fileState(const char * Paht, char* find_rule = "*");//�����ļ���
void insert_file(files& file);
//int findFileForSql(char* dir, files & file);
void getDirTime(char* dir, time_t& dir_time);

void add_DL(url_info & info);
char * getState(DL_state & dl_state);
char * getProtocol(DL_protocol & dl_protocol);
void updata_success(url_info & info);
void updata_FAIL(url_info& info);

#endif