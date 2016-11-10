#ifndef _URLMYSQL_H_INCLUDED_
#define _URLMYSQL_H_INCLUDED_
//------------ include ------------
#include "stdafx.h"
//---------- functions ------------------------
int open_Mysql();//初始化Mysql连接
void close_Mysql();//关闭MySql的连接
void update_state();//更新状态
int getSumTask();//获得已经建立任务的总数
int getErrSumTask();//获得不能完成的下载任务总数
int get_againURL();//建立获得可以从新下载的url的连接
int get_again_URL(url_info & info);//得到一条下载的url
void update_errSatk_state();//更新错误的任务状态
void update_Success_url(url_info & info);//更新建立成功的任务信息
void update_Error_url(url_info & info);//更新建立错误的任务信息
void update_AgainSuccess_url(url_info & info);//更新再次尝试建立成功任务的URL
void update_AgainError_url(url_info & info);//更新再次尝试建立错误任务的状态


#endif