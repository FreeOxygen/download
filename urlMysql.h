#ifndef _URLMYSQL_H_INCLUDED_
#define _URLMYSQL_H_INCLUDED_
#include <string.h>
#include "stdafx.h"

//---------- functions ------------------------
void update_Success_url(url_info & info);//更新建立成功的任务信息
void update_Error_url(url_info & info);//更新建立错误的任务信息
void update_AgainSuccess_url(url_info & info);//更新再次尝试建立成功任务的URL
void update_AgainError_url(url_info & info);//更新再次尝试建立错误任务的URLhttp://dl.google.com/dl/android/studio/install/2.1.3.0/android-studio-bundle-143.3101438-windows.exe	
void initMysql();//初始化Mysql连接
void update_state();//更新状态
int getSumTask();//获得已经建立任务的总数
int get_againURL();//建立获得可以从新下载的url的连接
int get_again_URL(url_info & info);//得到一条下载的url

#endif