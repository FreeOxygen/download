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

enum DL_state { DL_WAIT, DL_RUN, DL_FAIL, DL_SUCCESS };//下载的状态
enum DL_protocol { http_https, magnet, ED2K, ftp };//下载协议

struct url_info
{
	int DJID ;//下载ID
	int JID ;//下载链接ID
	char url[1024];//url
	int RETRY_COUNT;//下载次数
	char filename[1024];//下载完成文件的名称
	int filesize;//下载完成文件大小
	char filepath[1024];//下载存放的路径
	int progress;//下载进度
	DL_protocol protocol ;//下载使用的协议
	DL_state state ;//下载状态
	char remark[256] = "";//下载失败说明
	char start_time[64] = "";//下载开始时间
	char end_time[64] = "";//下载完成时间
	char update_time[64] = "";//更新时间
	char tool[256] = { "thunder" };//使用的下载工具
};
////---------- functions ------------------------

void initMysql();//初始化Mysql连接
int fileState(const char * Paht, char* find_rule = "*");//遍历文件夹
void insert_file(files& file);
//int findFileForSql(char* dir, files & file);
void getDirTime(char* dir, time_t& dir_time);

void add_DL(url_info & info);
char * getState(DL_state & dl_state);
char * getProtocol(DL_protocol & dl_protocol);
void updata_success(url_info & info);
void updata_FAIL(url_info& info);

#endif