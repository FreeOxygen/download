//---------------------- include --------------
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include "config.h"
#include "urlFile.h"
using namespace std;
using std::vector;


//------------------- function --------------
/*
读取文件中的URL
fileName 读取文件的路径
*/
vector<char*> UrlRead(char* fileName)
{
	char* buffer;
	vector<char*> ver;//存储满足条件的URl
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open())
	{
		cout << "***************" << "加载URL文件错误" << "**************" << endl;
		system("pause");
		exit(0);
	}
	else
	{
		while (!fin.eof())
		{
			buffer = new char[1024];
			fin.getline(buffer, 1024);
			if(is_url_valid(buffer))
			{
				//cout << buffer << endl;
				ver.push_back(buffer);
			}
		}
	}
	return ver;
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
	case 1 :
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
