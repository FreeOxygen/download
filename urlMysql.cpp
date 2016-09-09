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
		cout << "���ӳɹ�" << endl;
	}
}

//���ݿ�������ļ�
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
����ӵ�����������ӵ����ݿ�
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
//�������ݿ��е��ļ�
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
//�������״̬
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
	res = mysql_store_result(&con);//ȡ�ò�ѯ���
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
		cout << "û���ҵ���" << endl;
		mysql_free_result(res);//�ͷ�res��Դ
		return 0;
	}
	else
	{
		cout << file.dir << "|" << file.file_name << "|" << file.file_state << endl;
		mysql_free_result(res);//�ͷ�res��Դ
		return rows;
	}
}

/*
�ݹ��ѯ�ļ�
Path ���ҵ�·��
find_rule ���ҵĹ���,Ĭ��Ϊ*
return 0 û���ļ���1 �����ļ�,-1 �ļ�������
*/
int fileState(const char * Path, char * find_rule)
{
	char findPath[1024];
	int rt = 0;
	strcpy(findPath, Path);
	strcat(findPath, "/");
	strcat(findPath, find_rule);//���ò���ȫ���ļ�

	_finddata_t findFile;//��ȡ�õ����ļ���Ϣ

	intptr_t hFile;//���ҵ����ļ����

	if ((hFile = _findfirst(findPath, &findFile)) == -1L)
	{
		cout << findPath << "û�и��ļ�" << endl;
		rt = -1;
	}
	else {

		do {
			if (findFile.attrib & _A_SUBDIR) //��ѯ����Ϊ�ļ�
			{
				if (strcmp(findFile.name, ".") != 0 && strcmp(findFile.name, "..") != 0)
				{
					//��ѯ��Ŀ¼
					strcpy(findPath, Path);
					strcat(findPath, "/");
					strcat(findPath, findFile.name);
					//�ж��Ƿ���.cfg�ļ������������ص��ļ�
					if (-1 == fileState(findPath, "*.xltd"))//�ļ�û�п�ʼ���ػ��������
					{
						fileState(findPath);//�Ƿ����ļ�
					}
					else
					{
						//���������ļ�����
						//�ж��Ƿ����س�ʱ
						time_t dir_time;
						time_t	now_time;
						now_time = time(NULL);//������ڵ�ʱ��
						char tmp[24];
						strncpy(tmp, findPath + (strlen(config.savePath)+1), 23);//
						getDirTime(tmp, dir_time);//��ô������ص�ʱ��
						double a = difftime(now_time, dir_time);
						if (a > 240)//�ļ����س���24Сʱ��86400�����ж�Ϊ����ʧ��
						{

							url_info info;
							strncpy(info.filepath, findPath, strlen(config.savePath) + strlen(tmp) + 1);
							info.state = DL_FAIL;
							strcpy(info.remark, "Download timeout !");
							updata_FAIL(info);
							cout << "���س�ʱ����" << endl;
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
				//��ѯ���ļ�
				if (strcmp(find_rule, "*") == 0)//�Ƿ��������
				{
					if (1)//�����ж��ļ���׺
					{
						url_info info;
						strncpy(info.filepath, Path, strlen(config.savePath) + 24);//�ļ��洢·��
						strcpy(info.filename, findFile.name);
						info.filesize = findFile.size;//�ļ��Ĵ�С
						tm * t = localtime(&findFile.time_write);//��ȡ�ļ�����޸�ʱ��
						strftime(info.end_time, 64, "%Y-%m-%d %H:%M:%S", t);
						info.state = DL_SUCCESS;//����״̬
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
					now_time = time(NULL);//������ڵ�ʱ��
					strcpy(dir, (strrchr(Path, '/') + 1));
					getDirTime(dir, dir_time);//��ô������ص�ʱ��
					double a = difftime(now_time, dir_time);
					if (a > 240)//�ļ����س���24Сʱ��86400�����ж�Ϊ����ʧ��
					{
						url_info info;
						strcpy(info.filepath, Path);
						info.state = DL_FAIL;
						strcpy(info.remark, "Link timeout !");
						updata_FAIL(info);
						cout << "���ӳ�ʱ��ʱ����" << endl;
					}
					cout << dir << "--->�ļ�����û���ļ�" << endl;
				}
				cout << "find file***********û�������ļ�********" << endl;
			}
		}
		_findclose(hFile);
	}
	return rt;
}
//����ļ�����ʱ��
void getDirTime(char* dir, time_t& dir_time)
{
	tm tm_;
	char tmpDir[256];
	char tmp[256];
	strcpy(tmpDir, dir);
	strtok(tmpDir, "_");
	strcpy(tmp, tmpDir);//������
	strcat(tmp, "-");
	strcat(tmp, strtok(NULL, "_"));//ʱ
	strcat(tmp, "-");
	strcat(tmp, strtok(NULL, "_"));//��
	strcat(tmp, "-");
	strcat(tmp, strtok(NULL, "_"));//��
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

//��״̬ת��Ϊ�ַ���
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
//�������Э��
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