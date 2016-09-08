//---------------------- include --------------
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include "config.h"
#include "urlFile.h"
#include "urlSource.h"
using namespace std;
using std::vector;

ifstream fin;
char filename[1024];
//------------------- function --------------
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
		cout << "url文件:" << fileName << "--打开成功!" << endl;
		return 1;
	}
}

int read_url(char * readUrl)
{
	char buffer[1024];
	if (fin.is_open())//文件已经打开
	{
		if (!fin.eof())
		{
			fin.getline(buffer, 1024);
			while (!is_url_valid(buffer))
			{
				if (fin.eof()) //文件已经读完
				{
					//删除文件
					fin.close();
					remove(filename);//删除已经读取完成的文件
					if (find_new_file(config.urlPath))//查找新的文件
					{
						char tmpBuf[1024];
						if (read_url(tmpBuf))
						{
							strcpy(readUrl, tmpBuf);
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
				fin.getline(buffer, 1024);
			}
			strcpy(readUrl, buffer);
			return 1;
		}
		else
		{
			//删除文件
			fin.close();
			remove(filename);//删除已经读取完成的文件
			if (find_new_file(config.urlPath))//查找新的文件
			{
				char tmpBuf[1024];
				if (read_url(tmpBuf))
				{
					strcpy(readUrl, tmpBuf);
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
		if (find_new_file(config.urlPath))//查找新的文件
		{
			char tmpBuf[1024];
			if (read_url(tmpBuf))
			{
				strcpy(readUrl, tmpBuf);
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
/*
判断url是否是有效的
url需要验证的URL地址
*/
bool is_url_valid(char* url)
{
	regex http_Mold(config.url_mold_1);//"^(http://).*(\\.\\w+)+");//http协议下载
	regex ftp_Mold(config.url_mold_2);//"ftp://[^\\s]*");//ftp协议下载
	regex magnet_Mold(config.url_mold_3);//"magnet:\\?xt=urn:btih:[^\\s]*");//磁力链接下载
	regex ED2K_Mold(config.url_mold_4);//"(ed2k://\\|file\\|){1}.*");//电驴格式下载
	switch (1)//进行判断url是否满足允许的协议
	{
	case 1:
		if (regex_match(url, http_Mold))
		{
			return true;
		}
	case 2:
		if (regex_match(url, ftp_Mold))
		{
			return true;
		}
	case 3:
		if (regex_match(url, magnet_Mold))
		{
			return true;
		}
	case 4:
		if (regex_match(url, ED2K_Mold))
		{
			return true;
		}
	default:
		return false;
	}
}
