/////----------  include -------------------------
#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <io.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string>
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

	if (NULL != &con && mysql_real_connect(&con, config.host, config.user , config.passwd, config.dbName, 3306, NULL, 0))
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
//更新数据库中的文件
void updata_file(files& file)
{
	char sql[1024];
	int rt;
	sprintf(sql, "update test set file_name = \"%s\",file_time = \"%s\",file_hash = \"%s\",file_state = \"%s\" where dir = \"%s\"", file.file_name, +file.file_time, +file.file_hash, file.file_state, file.dir);
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
	strcat(findPath, "\\");
	strcat(findPath, find_rule);//设置查找全部文件

	_finddata_t findFile;//获取得到的文件信息

	intptr_t hFile;//查找到的文件句柄

	if ((hFile = _findfirst(findPath, &findFile)) == -1L)
	{
		cout << "没有该文件" << endl;
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
					strcat(findPath, "\\");
					strcat(findPath, findFile.name);
					//判断是否有.cfg文件，有正在下载的文件
					if (-1 == fileState(findPath, "*.cfg"))//文件没有开始下载或下载完成
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
						gitDirTime(findFile.name, dir_time);//获得创建下载的时间
						double a = difftime(now_time, dir_time);
						if (a > 86400)//文件下载超过24小时（86400），判断为下载失败
						{
							files file;
							strcpy(file.dir, findFile.name);
							strcpy(file.file_state, "2");
							updata_file(file);
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
						files file;
						strcpy(file.dir, (strrchr(Path, '\\') + 1));//文件夹名称
						strcpy(file.file_name, findFile.name);//下载文件名称
						tm * t = localtime(&findFile.time_write);//获取文件最后修改时间
						strftime(file.file_time, 64, "%Y-%m-%d-%H-%M-%S", t);
						strcpy(file.file_state, "0");
						updata_file(file);
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
					strcpy(dir, (strrchr(Path, '\\') + 1));
					gitDirTime(dir, dir_time);//获得创建下载的时间
					double a = difftime(now_time, dir_time);
					if (a > 86400)//文件下载超过24小时（86400），判断为下载失败
					{
						files file;
						strcpy(file.dir, dir);
						strcpy(file.file_state, "2");
						updata_file(file);
						cout << "下载超时！！" << endl;
					}
					cout << dir << "--->文件夹中没有文件" << endl;
				}
			}
		}
		_findclose(hFile);
	}
	return rt;
}
//获得文件创建时间
void gitDirTime(char* dir, time_t& dir_time)
{
	tm tm_;
	string tmp((strchr(dir, '_') + 1), (strrchr(dir, '_')));
	int year, month, day, hour, minute, second;
	sscanf(tmp.c_str(), "%d-%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute, &second);

	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	dir_time = mktime(&tm_);
}