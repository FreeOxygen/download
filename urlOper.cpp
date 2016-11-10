//----------- include -------------
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "urlOper.h"
using namespace std;

//----------- gloable variable ----

ifstream fin;
char filename[1024];

//----------- functions -----------

/*
打开一个url文件
fileName 读取文件的路径
*/
int open_urlFile(char* fileName)
{
	fin.open(fileName);
	if (!fin.is_open())
	{
		cout << "***************" << "文件打开错误" << "**************" << endl;
		return 0;
	}
	else
	{
		strcpy(filename, fileName);
		cout << "url文件:" << fileName << "-->打开成功!" << endl;
		return 1;
	}
}

/*
查找URLPATH路径下的URL文件

	路径打来正确返回
*/
int find_new_file(const char * Path)
{
	char findPath[1024];
	strcpy(findPath, Path);
	strcat(findPath, "/*");

	_finddata_t findFile;//获取得到的文件信息

	intptr_t hFile;//查找到的文件句柄

	if ((hFile = _findfirst(findPath, &findFile)) == -1L)
	{
		_findclose(hFile);
		cout << "***** urlPath 路径错误 *******" << endl;
		getchar();
		exit(0);
		return 0;
	}
	else {
		do {
			if (findFile.attrib & _A_SUBDIR) //查询到的为文件夹
			{
				continue;
			}
			else
			{
				char tmp[1024];
				strcpy(tmp, Path);
				strcat(tmp, "/");
				strcat(tmp, findFile.name);
				if (open_urlFile(tmp))//文件打开成功
				{
					cout << "打开文件：" << findFile.name << endl;
					_findclose(hFile);
					return 1;
				}
			}
		} while (_findnext(hFile, &findFile) == 0);
		_findclose(hFile);
		cout << "********文件夹中没有新的URL文件*******" << endl;
		return 0;
	}
}

/*
读取一条URL的信息
*/
int i = 1;
int read_url(url_info & readUrl_info)
{
	char buffer[1024];
	if (fin.is_open())//文件是否打开
	{
		if (!fin.eof())
		{
			fin.getline(buffer, 1024);
			readUrl_info.JID = i++;
			strcpy(readUrl_info.url, buffer);
			return 1;
		}
		else
		{
			//删除文件
			fin.close();
			remove(filename);//删除已经读取完成的文件
			if (find_new_file(g_config.urlPath))//查找新的文件
			{
				url_info tmpinfo;
				if (read_url(tmpinfo))
				{
					readUrl_info = tmpinfo;
					return 1;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
	}
	else
	{
		if (find_new_file(g_config.urlPath))//查找新的文件
		{
			url_info tmpinfo;
			if (read_url(tmpinfo))
			{
				//strcpy(readUrl, tmpBuf);
				readUrl_info = tmpinfo;
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
}
