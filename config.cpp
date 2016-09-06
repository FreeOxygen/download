//------------------- include ------------
#include "config.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace std;

Config config;

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
			strtok(temp, "=");
			strcpy(temp2, strtok(NULL, "="));
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
	init("url_mold_1", config.url_mold_1);
	init("url_mold_2", config.url_mold_2);
	init("url_mold_3", config.url_mold_3);
	init("url_mold_4", config.url_mold_4);
}