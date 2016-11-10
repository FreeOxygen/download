/////----------  include -------------------------
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include "urlMysql.h"
#include "include\mysql.h"

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")
using namespace std;

/////---------------- gloable variable ----------
MYSQL con;
MYSQL_RES * res;//返回的结果集

//------------------- functions ---------------
int open_Mysql()
{
	mysql_init(&con);

	if (NULL != &con && mysql_real_connect(&con, g_config.dbhost, g_config.user, g_config.passwd, g_config.dbName, 3306, NULL, 0))
	{
		cout << "连接成功" << endl;
		return 1;
	}
	else
	{
		cout << "连接错误:" << mysql_error(&con) << endl;
		return 0;
	}
}
//断开数据库的连接
void close_Mysql()
{
	if (NULL != &con)
	{
		mysql_close(&con);
		cout << "数据库连接断开" << endl;
	}
}
//获得文件创建时间
void getDirTime(char* dir, time_t& dir_time)
{
	tm tm_;
	char tmpDir[256];
	char tmp[256];
	strcpy(tmpDir, dir);
	strtok(tmpDir, "_");
	strcpy(tmp, tmpDir);//年月日
	strcat(tmp, "-");
	strcat(tmp, strtok(NULL, "_"));//时
	strcat(tmp, "-");
	strcat(tmp, strtok(NULL, "_"));//分
	strcat(tmp, "-");
	strcat(tmp, strtok(NULL, "_"));//秒
	int year, month, day, hour, minute, second;
	sscanf(tmp, "%d-%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute, &second);

	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	dir_time = mktime(&tm_);
}
//将状态转化为字符串
char * getState(DL_state & dl_state)
{
	switch (dl_state)
	{
	case DL_WAIT:
		return "DL_WAIT";
	case DL_RUN:
		return "DL_RUN";
	case DL_FAIL:
		return "DL_FAIL";
	case DL_SUCCESS:
		return "DL_SUCCESS";
	}
}
//获得下载协议
char * getProtocol(DL_protocol & dl_protocol)
{
	switch (dl_protocol)
	{
	case http:
		return "http";
	case https:
		return "https";
	case magnet:
		return "magnet";
	case ED2K:
		return "ED2K";
	case ftp:
		return "ftp";
	case Err:
		return "Error";
	}
}
//设置下载协议
DL_protocol setProtocol(char * Protocol)
{
	if (strcmp(Protocol, "http") == 0)
		return http;
	if (strcmp(Protocol, "https") == 0)
		return https;
	if (strcmp(Protocol, "magnet") == 0)
		return magnet;
	if (strcmp(Protocol, "ED2K") == 0)
		return ED2K;
	if (strcmp(Protocol, "ftp") == 0)
		return ftp;
	if (strcmp(Protocol, "thunder") == 0)
		return thunder;
	if (strcmp(Protocol, "Error") == 0)
		return Err;
}
//获得remark
char * getRemark(DL_remark & dl_remark)
{
	switch (dl_remark)
	{
	case RE_Run://下载创建
		return "Run";
	case RE_OK://下载成功
		return "OK";
	case RE_xunlei_error://迅雷错误
		return "xunlei_error";
	case RE_Parse_error://解析错误
		return "Parse_error";
	case RE_Link_timeout://链接超时
		return "Link_timeout";
	case RE_Download_timeout://下载超时
		return "Download_timeout";
	case RE_Link_error://链接不能下载
		return "Link_error";
	case RE_URL_error://链接不符合要求
		return "URL_error";
	case RE_Download_repeat://重复下载
		return "Download_repeat";
	case RE_xunlei_save_error:
		return "xunlei_save_error";
	}
}
//更新创建成功任务的URL
void update_Success_url(url_info & info)
{
	//info.filepath;//文件保存路径
	//info.protocol;//下载协议
	//info.remark;//错误内容
	//info.start_time;//开始时间
	//info.state;//下载状态
	//info.tool;//下载工具
	//info.url;//下载URL
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	sprintf(sql, "insert into %s (JID,URL,FILEPATH,PROTOCOL,STATE,REMARK,START_TIME,TOOL) values(%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\")", g_config.tableName, info.JID, info.url, info.filepath,
		getProtocol(info.protocol), getState(info.state), getRemark(info.remark), info.start_time, info.tool);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}
//更新错误的URL
void update_Error_url(url_info & info)
{
	//info.protocol;//下载协议
	//info.remark;//错误内容
	//info.state;//下载状态
	//info.tool;//下载工具
	//info.url;//下载URL
	//设置中文字符的支持
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	sprintf(sql, "insert into %s (JID,URL,PROTOCOL,STATE,REMARK,TOOL) values(%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\")", g_config.tableName, info.JID, info.url,
		getProtocol(info.protocol), getState(info.state), getRemark(info.remark), info.tool);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}
//更新再次尝试建立成功任务的URL
void update_AgainSuccess_url(url_info & info)
{
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	//更新状态，同时增加重新尝试的次数
	sprintf(sql, "update %s set FILEPATH = '%s',STATE = '%s',REMARK = '%s',START_TIME = '%s',RETRY_COUNT = RETRY_COUNT+1 where DJID = %d", g_config.tableName, info.filepath,
		getState(info.state), getRemark(info.remark), info.start_time, info.DJID);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}
//更新错误的URL
void update_AgainError_url(url_info & info)
{
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	//更新尝试次数
	sprintf(sql, "update %s set STATE = '%s',REMARK = '%s',RETRY_COUNT = RETRY_COUNT+1 where DJID = %d", g_config.tableName,
		getState(info.state), getRemark(info.remark), info.DJID);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}
//更新下载状态
void updata_FAIL_xunlei_err(url_info& info)
{
	//设置中文字符的支持
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	sprintf(sql, "update %s set FILEPATH = \"%s\",STATE = \"%s\",REMARK = \"%s\",END_TIME = \"%s\"where DJID =%d", g_config.tableName, info.filepath, getState(info.state), getRemark(info.remark), info.end_time, info.DJID);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}
//更新下载状态
void updata_FAIL_err(url_info& info)
{
	//设置中文字符的支持
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	sprintf(sql, "update %s set STATE = \"%s\",REMARK = \"%s\",END_TIME = \"%s\"where DJID =%d", g_config.tableName, getState(info.state), getRemark(info.remark), info.end_time, info.DJID);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}

//获得数据库一行的信息
void get_data(url_info &info)
{
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	sprintf(sql, "select FILENAME,FILESIZE from %s where DJID =%d AND MACHINE_ID = %d", g_config.tableName, info.DJID,g_config.machine_id);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		//printf("%s executed!!!\n", sql);
		MYSQL_RES * TempRes;
		MYSQL_ROW TempRow;
		TempRes = mysql_store_result(&con);
		TempRow = mysql_fetch_row(TempRes);
		if (NULL != TempRow[0])
		{
			strcat(info.filename, ";");
			strcat(info.filename, TempRow[0]);
		}
		if (NULL != TempRow[1])
		{
			int size = 0;
			sscanf(TempRow[1], "%d", &size);
			info.filesize += size;
		}
		mysql_free_result(TempRes);
	}
}
//更新完成状态
void updata_success_file(url_info & info)
{
	//设置中文字符的支持
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	get_data(info);
	sprintf(sql, "update %s set FILENAME = \"%s\",FILESIZE = %d,STATE = \"%s\",REMARK = \"%s\",END_TIME = \"%s\"where DJID =%d", g_config.tableName, info.filename, info.filesize, getState(info.state), getRemark(info.remark), info.end_time, info.DJID);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}

void updata_success_file_null(url_info & info)
{
	//设置中文字符的支持
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	sprintf(sql, "update %s set STATE = \"%s\",REMARK = \"%s\",END_TIME = \"%s\"where DJID =%d", g_config.tableName, getState(info.state), getRemark(info.remark), info.end_time, info.DJID);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
	}
}
//得到状态为DL_RUN的行
int get_stateDL_RUN()
{
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	//得到状态为下载的文件路径
	sprintf(sql, "select DJID,FILEPATH from %s where TOOL in (%s) AND STATE = 'DL_RUN' AND MACHINE_ID = %d", g_config.tableName, g_config.tool,g_config.machine_id);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
		return 0;
	}
	else
	{
		cout << "下载状态集合获取成功" << endl;
		res = mysql_store_result(&con);//取得查询结果
	}
}
//得到查询的行
int get_RUN_row(url_info & info)
{
	MYSQL_ROW row;//行
	if (row = mysql_fetch_row(res))
	{
		char tmp[1024];
		strcpy(tmp, row[0]);
		sscanf(row[0], "%d", &(info.DJID));
		if (NULL != row[1])
		{
			strcpy(info.filepath, g_config.save_Path);
			strcat(info.filepath, row[1]);
		}
		else
		{
			cout << "DJID[" << row[0] << "]文件地址错误" << endl;
			sscanf("1", "%s", info.filepath);
		}
		//strcpy(info.filepath, row[1]);
		cout << info.DJID << "****查询==============>" << info.filepath << endl;
		return 1;
	}
	else
	{
		mysql_free_result(res);
		return 0;
	}
}
//得到现在总的任务数量
int getSumTask()
{
	char sql[1024] = "";
	int rt;
	sprintf(sql, "select count(DJID) from %s where TOOL in (%s) AND MACHINE_ID = %d AND REMARK in ('Run','Link_timeout','Download_timeout','xunlei_save_error')", g_config.tableName, g_config.tool,g_config.machine_id);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
		return -1;
	}
	else
	{
		//printf("%s executed!!!\n", sql);
		MYSQL_RES * TempRes;
		MYSQL_ROW TempRow;
		int rt;
		TempRes = mysql_store_result(&con);
		TempRow = mysql_fetch_row(TempRes);
		sscanf(TempRow[0], "%d", &rt);
		mysql_free_result(TempRes);
		return rt;
	}
}
//得到现在总的任务数量
int getErrSumTask()
{
	char sql[1024] = "";
	int rt;
	sprintf(sql, "select count(DJID) from %s where TOOL in (%s) AND MACHINE_ID = %d AND REMARK in ('Link_timeout','Download_timeout','xunlei_save_error')", g_config.tableName, g_config.tool,g_config.machine_id);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
		return -1;
	}
	else
	{
		//printf("%s executed!!!\n", sql);
		MYSQL_RES * TempRes;
		MYSQL_ROW TempRow;
		int rt;
		TempRes = mysql_store_result(&con);
		TempRow = mysql_fetch_row(TempRes);
		sscanf(TempRow[0], "%d", &rt);
		mysql_free_result(TempRes);
		return rt;
	}
}
//获得可以再次下载URL的数据库连接
int get_againURL()
{
	char sql[1024] = "set names \'GBK\'";
	int rt;
	mysql_real_query(&con, sql, strlen(sql));
	int sumtask = getSumTask();
	sprintf(sql, "select DJID,URL,PROTOCOL from %s where TOOL in (%s) AND MACHINE_ID = %d AND (STATE ='DL_WAIT'OR REMARK in( 'xunlei_error','Parse_error','Link_error')) AND RETRY_COUNT < %d limit %d", g_config.tableName, g_config.tool, g_config.machine_id, g_config.MaxPetryCount, (g_config.MaxTask - sumtask - 1));
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
		return 0;
	}
	else
	{
		//cout << "下载状态集合获取成功" << endl;
		res = mysql_store_result(&con);//取得查询结果
	}
}
//获得再次下载的url行
int get_again_URL(url_info & info)
{
	MYSQL_ROW row;//行
	if (row = mysql_fetch_row(res))
	{
		char tmp[256];
		strcpy(tmp, row[0]);
		sscanf(row[0], "%d", &(info.DJID));//得到url的主键
		sscanf(row[1], "%s", info.url);//得到url地址
		if (NULL != row[2] && 0 != (strcmp(row[2], "Error")))
		{
			sscanf(row[2], "%s", tmp);//转换下载协议
			info.protocol = setProtocol(tmp);
		}
		else
		{
			is_url_valid(info);
			char sql[1024] = "set names \'GBK\'";
			sprintf(sql, "update %s set PROTOCOL = \"%s\" where DJID =%d", g_config.tableName, getProtocol(info.protocol), info.DJID);
			mysql_real_query(&con, sql, strlen(sql));
		}
		return 1;
	}
	else
	{
		mysql_free_result(res);
		return 0;
	}
}
//更新错误任务的状态
void update_errSatk_state()
{
	char sql[1024] = "";
	int rt;
	sprintf(sql, " select DJID,REMARK,RETRY_COUNT from %s where TOOL in (%s) AND MACHINE_ID = %d AND REMARK in ('Link_timeout','Download_timeout','xunlei_save_error')", g_config.tableName, g_config.tool,g_config.machine_id);
	rt = mysql_real_query(&con, sql, strlen(sql));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql);
		MYSQL_RES * TempRes;
		MYSQL_ROW TempRow;
		TempRes = mysql_store_result(&con);
		while (TempRow = mysql_fetch_row(TempRes))
		{
			int cont = 0;
			int Djid = 0;
			sscanf(TempRow[2], "%d", &cont);//获得尝试的次数
			sscanf(TempRow[0], "%d", &Djid);//获得尝试的次数
			cout << TempRow[0] << "|" << TempRow[1] << "|" << TempRow[2] << endl;
			if ((cont <= g_config.MaxPetryCount) && ((strcmp(TempRow[1], "xunlei_save_error") == 0) || (strcmp(TempRow[1], "Link_timeout") == 0)))
			{
				//超时链接允许重新尝试
				sprintf(sql, "update %s set REMARK = 'Link_error' where DJID =%d", g_config.tableName, Djid);
			}
			else
			{
				if (strcmp(TempRow[1], "Download_timeout") && cont <= 2)
				{
					//超时链接允许重新尝试
					sprintf(sql, "update %s set REMARK = 'Link_error' where DJID =%d", g_config.tableName, Djid);
				}
				else
				{
					sprintf(sql, "update %s set REMARK = 'Link_error' where DJID =%d", g_config.tableName, Djid);
				}
			}
			//执行sql语句
			rt = mysql_real_query(&con, sql, strlen(sql));
			if (rt)
			{
				printf("Error making query: %s !!!\n", mysql_error(&con));
			}
			else
			{
				printf("%s executed!!!\n", sql);
			}
		}
		mysql_free_result(TempRes);
	}
}

int find_file(url_info & info, char * find_rule = "*")
{
	_finddata_t findFile;//获取得到的文件信息
	intptr_t hFile;//查找到的文件句柄
	char findPath[1024];
	int rt = 0;

	//判断路径下面是否有.xltd文件
	strcpy(findPath, info.filepath);
	strcat(findPath, "/*.xltd");
	if (-1L == (hFile = _findfirst(findPath, &findFile)))
	{
		//文件中不存在.xltd文件
		//判断文件中所有文件
		strcpy(findPath, info.filepath);
		strcat(findPath, "/");
		strcat(findPath, find_rule);//设置查找全部文件

		if ((hFile = _findfirst(findPath, &findFile)) == -1L)
		{
			//没有文件或文件夹
			cout << findPath << "--->文件不存在" << "****迅雷存储出问题" << endl;
			time_t now_time;
			now_time = time(NULL);//获得现在的时间
			tm * t = localtime(&now_time);
			strftime(info.end_time, 64, "%Y-%m-%d %H:%M:%S", t);
			info.state = DL_FAIL;
			info.remark = RE_xunlei_save_error;
			strcpy(info.filepath, "");

			updata_FAIL_xunlei_err(info);
			return 0;
		}
		else {
			do {
				if (findFile.attrib & _A_SUBDIR) //查询到的为文件夹
				{
					if (strcmp(findFile.name, ".") != 0 && strcmp(findFile.name, "..") != 0)
					{
						//查询子目录
						char temp_filepath[1024];
						strcpy(temp_filepath, info.filepath);
						strcpy(findPath, info.filepath);
						strcat(findPath, "/");
						strcat(findPath, findFile.name);
						strcpy(info.filepath, findPath);
						find_file(info);//查找子文件夹
						strcpy(info.filepath, temp_filepath);
						rt = -1000;//后面判断空
					}
					else
					{
						continue;
					}
				}
				else
				{
					rt++;//文件夹中有文件
					//查询到文件
					char * temp_char;
					temp_char = strrchr(findFile.name, '.');

					if ((NULL == temp_char) || is_file_valid(temp_char))//可以判断文件后缀.torrent
					{
						char temp[1024] = "";
						if (strlen(info.filepath) > (strlen(g_config.savePath) + strlen("/2016-09-23_13_01_15_128")))
						{
							strcpy(temp, (info.filepath + strlen(g_config.savePath) + strlen("/2016-09-23_13_01_15_128/")));
							strcat(temp, "/");
						}
						strcat(temp, findFile.name);
						strcpy(info.filename, temp);//下载完成文件的名称
						info.filesize = findFile.size / 1024;//文件的大小
						tm * t = localtime(&findFile.time_write);//获取文件最后修改时间
						strftime(info.end_time, 64, "%Y-%m-%d %H:%M:%S", t);
						info.state = DL_SUCCESS;//更新状态
						info.remark = RE_OK;
						updata_success_file(info);
					}
					else
					{
						tm * t = localtime(&findFile.time_write);//获取文件最后修改时间
						strftime(info.end_time, 64, "%Y-%m-%d %H:%M:%S", t);
						info.state = DL_SUCCESS;//更新状态
						info.remark = RE_OK;
						updata_success_file_null(info);
					}
				}
			} while (_findnext(hFile, &findFile) == 0);
			//没有文件
			if (0 == rt)//文件夹中没有文件
			{
				if (strcmp(findFile.name, ".") == 0 || strcmp(findFile.name, "..") == 0)
				{
					char dir[255];
					time_t dir_time;
					time_t now_time;
					now_time = time(NULL);//获得现在的时间
					strncpy(dir, findPath + (strlen(g_config.savePath) + 1), 23);//取得文件夹时间

					tm * t = localtime(&now_time);//修改的时间
					strftime(info.end_time, 64, "%Y-%m-%d %H:%M:%S", t);

					getDirTime(dir, dir_time);//获得创建下载的时间
					double a = difftime(now_time, dir_time);
					if (a > g_config.timeout_time)//文件下载超过24小时（86400），判断为下载失败
					{
						info.state = DL_FAIL;
						info.remark = RE_Link_timeout;
						updata_FAIL_err(info);
						cout << "链接超时超时！！" << endl;
					}
				}
			}
		}
		_findclose(hFile);
	}
	else
	{
		//存在.xltd文件
		//正在下载文件处理
		//判断是否下载超时
		time_t dir_time;
		time_t	now_time;
		now_time = time(NULL);//获得现在的时间
		char tmp[24];
		strncpy(tmp, findPath + (strlen(g_config.savePath) + 1), 23);//取得文件夹中的时间

		tm * t = localtime(&now_time);//修改的时间
		strftime(info.end_time, 64, "%Y-%m-%d %H:%M:%S", t);

		getDirTime(tmp, dir_time);//获得创建下载的时间
		double a = difftime(now_time, dir_time);
		if (a > g_config.timeout_time)//文件下载超过24小时（86400），判断为下载失败
		{
			info.state = DL_FAIL;
			info.remark = RE_Download_timeout;
			updata_FAIL_err(info);
			cout << "下载超时！！" << endl;
		}
		_findclose(hFile);
	}
	return rt;
}

void update_state()
{
	if (get_stateDL_RUN())
	{
		url_info info;
		while (get_RUN_row(info))
		{
			find_file(info);
		}
	}
}