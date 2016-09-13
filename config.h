#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

#include <stdlib.h>
#include <string.h>
#include <iostream>
using  namespace std;
//---------- struct ----------
struct Config
{
	char xunlei[256];
	char urlPath[256];
	char savePath[256];
	char user[256];
	char passwd[256];
	char host[256];
	char dbName[256];
	char tableName[256];
	int timeout_time;//判断下载文件超时时间，单位秒
	char http_mold[256];
	char https_mold[256];
	char magnet_mold[256];
	char ED2K_mold[256];
	char ftp_mold[256];
	int MaxTask;//最大的任务量
	int MaxPetryCount;
};
//---------- global ---------
extern Config config;

//---------- function ---------
void init_config();

#endif
