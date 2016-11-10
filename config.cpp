//------------------- include ------------
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;

//----------------- function -------------
/*
加载配置文件
name 需要加载的名称
*/
void  init(char* name, char* path)
{
	FILE *fp;
	char temp[126] = { 0 };
	char temp2[126] = { 0 };
	fp = fopen("config.txt", "r");
	if (fp == NULL)
	{
		cout << "***************" << "加载config.txt文件错误" << "**************" << endl;
		system("pause");
		exit(0);
	}

	fgets(temp, 126, fp);
	while (!feof(fp))
	{
		if (strstr(temp, name))
		{
			strcpy(temp2, (strchr(temp, '=') + 1));
			int i;
			for (i = 0; i < 126; i++) {

				if (temp2[i] == '\n')
				{
					temp2[i] = '\0';
				}
			}
			strcpy(path, temp2);
			cout << name << "初始化成功" << endl;
			break;
		}
		fgets(temp, 126, fp);
	}
	fclose(fp);
}

//初始化变量
void init_config()
{
	init("xunlei", g_config.xunlei);
	init("urlPath", g_config.urlPath);
	init("savePath", g_config.save_Path);
	init("relativePath", g_config.relativePath);
	if (g_config.save_Path[0] == '\\')
	{
		cout << g_config.save_Path << endl;
	}
	strcpy(g_config.savePath, g_config.save_Path);
	strcat(g_config.savePath, g_config.relativePath);//得到保存文件目录的全路径
	init("dbuser", g_config.user);
	init("dbpasswd", g_config.passwd);
	init("dbhost", g_config.dbhost);
	init("dbName", g_config.dbName);
	init("tableName", g_config.tableName);
	init("tool", g_config.tool);
	char temp[255];
	init("timeout_time", temp);
	sscanf(temp, "%d", &(g_config.timeout_time));
	init("MaxPetryCount", temp);
	sscanf(temp, "%d", &(g_config.MaxPetryCount));
	init("MaxTask", temp);
	sscanf(temp, "%d", &(g_config.MaxTask));
	init("MACHINE_ID", temp);
	sscanf(temp, "%d", &(g_config.machine_id));
	init("WaitTime", temp);
	sscanf(temp, "%d", &(g_config.WaitTime));
	g_config.WaitTime *= 1000;
	init("http_mold", g_config.http_mold);
	init("https_mold", g_config.https_mold);
	init("magnet_mold", g_config.magnet_mold);
	init("ED2K_mold", g_config.ED2K_mold);
	init("thunder_mold", g_config.thunder_mold);
	init("ftp_mold", g_config.ftp_mold);
	init("file_rule", g_config.file_rule);
}