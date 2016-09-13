//------------------- include ------------
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;
//------------------ global ---------------
Config config;//全局配置信息

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
			strcpy(temp2, (strchr(temp,'=') + 1));
			int i;
			for (i = 0; i < 126; i++) {

				if (temp2[i] == '\n')
				{
					temp2[i] = '\0';
				}
			}
			strcpy(path, temp2);
			break;
		}
		fgets(temp, 126, fp);
	}
	fclose(fp);
}

//初始化变量
void init_config()
{
	init("xunlei", config.xunlei);
	init("urlPath", config.urlPath);
	init("savePath", config.savePath);
	init("user", config.user);
	init("passwd", config.passwd);
	init("host", config.host);
	init("dbName", config.dbName);
	init("tableName", config.tableName);
	char temp[255];
	init("timeout_time", temp); 
	sscanf(temp, "%d",&(config.timeout_time));
	init("MaxPetryCount", temp);
	sscanf(temp, "%d", &(config.MaxPetryCount));
	init("MaxTask", temp);
	sscanf(temp, "%d", &(config.MaxTask));
	init("http_mold", config.http_mold);
	init("https_mold", config.https_mold);
	init("magnet_mold", config.magnet_mold);
	init("ED2K_mold", config.ED2K_mold);
	init("ftp_mold", config.ftp_mold);
}