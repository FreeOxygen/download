/////----------  include -------------------------
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include "config.h"
#include "urlMysql.h"
#include "include\mysql.h"

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")
using namespace std;

/////---------------- gloable variable ----------
MYSQL con;
//extern Config config;
void initMysql()
{
	mysql_init(&con);

	if (NULL != &con && mysql_real_connect(&con, config.host, config.user, config.passwd, config.dbName, 3306, NULL, 0))
	{
		cout << "连接成功" << endl;
	}
}

//数据库中添加文件
void insert_file(files &file)
{
	string sql = "insert into test(dir, file_name, file_time, file_hash, file_state) values(\"";
	int rt;
	sql = sql + file.dir + "\",\"" + file.file_name + "\",\"" + file.file_time + "\",\"" + file.file_hash + "\",\'" + file.file_state + "\')";
	rt = mysql_real_query(&con, sql.c_str(), strlen(sql.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql.c_str());
	}
}

/*
将添加的下载任务，添加到数据库
*/
void add_DL(url_info & info)
{
	char sql[1024] = "";
	int rt;
	sprintf(sql, "insert into ecd_dl_job (DJID,JID,URL,FILEPATH,PROTOCOL,STATE,REMARK,START_TIME,TOOL) values(%d,%d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\")", info.DJID, info.JID, info.url, info.filepath,
		getProtocol(info.protocol), getState(info.state), info.remark, info.start_time, info.tool);
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
//更新数据库中的文件
void updata_FAIL(url_info& info)
{
	char sql[1024] = "update ecd_dl_job set STATE = \"%s\",REMARK=\"%s\" where FILEPATH = \"%s\"";
	int rt;
	sprintf(sql, "update ecd_dl_job set STATE = \"%s\",REMARK=\"%s\" where FILEPATH = \"%s\"", getState(info.state),info.remark,info.filepath);
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
//更新完成状态
void updata_success(url_info & info)
{
	char sql[1024] = "update ecd_dl_job set FILENAME = \"%s\",FILESIZE = %d,STATE = \"%s\",END_TIME = \"%s\"where FILEPATH =\"123\"";
	int rt;
	sprintf(sql, "update ecd_dl_job set FILENAME = \"%s\",FILESIZE = %d,STATE = \"%s\",END_TIME = \"%s\"where FILEPATH =\"%s\"", info.filename, info.filesize, getState(info.state), info.end_time, info.filepath);
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

int findFileForSql(string dir, files & file)
{
	MYSQL_RES * res;
	MYSQL_ROW row;
	int rows = 0;
	string sql = "select * from test where dir = \"";
	sql += dir + "\"";
	int rt = mysql_real_query(&con, sql.c_str(), strlen(sql.c_str()));
	if (rt)
	{
		printf("Error making query: %s !!!\n", mysql_error(&con));
	}
	else
	{
		printf("%s executed!!!\n", sql.c_str());
	}
	res = mysql_store_result(&con);//取得查询结果
	while (row = mysql_fetch_row(res))
	{
		strcpy(file.dir, row[1]);
		strcpy(file.file_name, row[2]);
		strcpy(file.file_time, row[3]);
		strcpy(file.file_hash, row[4]);
		strcpy(file.file_state, row[5]);
		row++;
	}
	if (0 == rows)
	{
		cout << "没有找到行" << endl;
		mysql_free_result(res);//释放res资源
		return 0;
	}
	else
	{
		cout << file.dir << "|" << file.file_name << "|" << file.file_state << endl;
		mysql_free_result(res);//释放res资源
		return rows;
	}
}

/*
递归查询文件
Path 查找的路径
find_rule 查找的规则,默认为*
return 0 没有文件，1 存在文件,-1 文件不存在
*/
int fileState(const char * Path, char * find_rule)
{
	char findPath[1024];
	int rt = 0;
	strcpy(findPath, Path);
	strcat(findPath, "/");
	strcat(findPath, find_rule);//设置查找全部文件

	_finddata_t findFile;//获取得到的文件信息

	intptr_t hFile;//查找到的文件句柄

	if ((hFile = _findfirst(findPath, &findFile)) == -1L)
	{
		cout << findPath << "没有该文件" << endl;
		rt = -1;
	}
	else {

		do {
			if (findFile.attrib & _A_SUBDIR) //查询到的为文件
			{
				if (strcmp(findFile.name, ".") != 0 && strcmp(findFile.name, "..") != 0)
				{
					//查询子目录
					strcpy(findPath, Path);
					strcat(findPath, "/");
					strcat(findPath, findFile.name);
					//判断是否有.cfg文件，有正在下载的文件
					if (-1 == fileState(findPath, "*.xltd"))//文件没有开始下载或下载完成
					{
						fileState(findPath);//是否有文件
					}
					else
					{
						//正在下载文件处理
						//判断是否下载超时
						time_t dir_time;
						time_t	now_time;
						now_time = time(NULL);//获得现在的时间
						char tmp[24];
						strncpy(tmp, findPath + (strlen(config.savePath)+1), 23);//
						getDirTime(tmp, dir_time);//获得创建下载的时间
						double a = difftime(now_time, dir_time);
						if (a > 240)//文件下载超过24小时（86400），判断为下载失败
						{

							url_info info;
							strncpy(info.filepath, findPath, strlen(config.savePath) + strlen(tmp) + 1);
							info.state = DL_FAIL;
							strcpy(info.remark, "Download timeout !");
							updata_FAIL(info);
							cout << "下载超时！！" << endl;
						}
					}
				}
				else
				{
					continue;
				}
			}
			else
			{
				rt++;
				//查询到文件
				if (strcmp(find_rule, "*") == 0)//是否查找所有
				{
					if (1)//可以判断文件后缀
					{
						url_info info;
						strncpy(info.filepath, Path, strlen(config.savePath) + 24);//文件存储路径
						strcpy(info.filename, findFile.name);
						info.filesize = findFile.size;//文件的大小
						tm * t = localtime(&findFile.time_write);//获取文件最后修改时间
						strftime(info.end_time, 64, "%Y-%m-%d %H:%M:%S", t);
						info.state = DL_SUCCESS;//更新状态
						strcpy(info.remark, "OK!");
						updata_success(info);
					}
				}
			}
		} while (_findnext(hFile, &findFile) == 0);
		if (0 == rt)
		{
			if (strcmp(findFile.name, ".") == 0 || strcmp(findFile.name, "..") == 0)
			{
				if (strcmp(Path, config.savePath) != 0)
				{
					char dir[255];
					time_t dir_time;
					time_t now_time;
					now_time = time(NULL);//获得现在的时间
					strcpy(dir, (strrchr(Path, '/') + 1));
					getDirTime(dir, dir_time);//获得创建下载的时间
					double a = difftime(now_time, dir_time);
					if (a > 240)//文件下载超过24小时（86400），判断为下载失败
					{
						url_info info;
						strcpy(info.filepath, Path);
						info.state = DL_FAIL;
						strcpy(info.remark, "Link timeout !");
						updata_FAIL(info);
						cout << "链接超时超时！！" << endl;
					}
					cout << dir << "--->文件夹中没有文件" << endl;
				}
				cout << "find file***********没有下载文件********" << endl;
			}
		}
		_findclose(hFile);
	}
	return rt;
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
	case http_https:
		return "http_https";
	case magnet:
		return "magnet";
	case ED2K:
		return "ED2K";
	case ftp:
		return "ftp";
	}
}