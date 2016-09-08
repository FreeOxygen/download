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
//�������ݿ��е��ļ�
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
	strcat(findPath, "\\");
	strcat(findPath, find_rule);//���ò���ȫ���ļ�

	_finddata_t findFile;//��ȡ�õ����ļ���Ϣ

	intptr_t hFile;//���ҵ����ļ����

	if ((hFile = _findfirst(findPath, &findFile)) == -1L)
	{
		cout << "û�и��ļ�" << endl;
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
					strcat(findPath, "\\");
					strcat(findPath, findFile.name);
					//�ж��Ƿ���.cfg�ļ������������ص��ļ�
					if (-1 == fileState(findPath, "*.cfg"))//�ļ�û�п�ʼ���ػ��������
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
						gitDirTime(findFile.name, dir_time);//��ô������ص�ʱ��
						double a = difftime(now_time, dir_time);
						if (a > 86400)//�ļ����س���24Сʱ��86400�����ж�Ϊ����ʧ��
						{
							files file;
							strcpy(file.dir, findFile.name);
							strcpy(file.file_state, "2");
							updata_file(file);
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
						files file;
						strcpy(file.dir, (strrchr(Path, '\\') + 1));//�ļ�������
						strcpy(file.file_name, findFile.name);//�����ļ�����
						tm * t = localtime(&findFile.time_write);//��ȡ�ļ�����޸�ʱ��
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
					now_time = time(NULL);//������ڵ�ʱ��
					strcpy(dir, (strrchr(Path, '\\') + 1));
					gitDirTime(dir, dir_time);//��ô������ص�ʱ��
					double a = difftime(now_time, dir_time);
					if (a > 86400)//�ļ����س���24Сʱ��86400�����ж�Ϊ����ʧ��
					{
						files file;
						strcpy(file.dir, dir);
						strcpy(file.file_state, "2");
						updata_file(file);
						cout << "���س�ʱ����" << endl;
					}
					cout << dir << "--->�ļ�����û���ļ�" << endl;
				}
			}
		}
		_findclose(hFile);
	}
	return rt;
}
//����ļ�����ʱ��
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