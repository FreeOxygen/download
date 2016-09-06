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
��ȡ�ļ��е�URL
fileName ��ȡ�ļ���·��
*/
vector<char*> UrlRead(char* fileName)
{
	char* buffer;
	vector<char*> ver;//�洢����������URl
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open())
	{
		cout << "***************" << "����URL�ļ�����" << "**************" << endl;
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
�ж�url�Ƿ�����Ч��
url��Ҫ��֤��URL��ַ
*/
bool is_url_valid(char* url)
{
	regex http_Mold(config.url_mold_1);//"^(http://).*(\\.\\w+)+");//httpЭ������
	regex ftp_Mold(config.url_mold_2);//"ftp://[^\\s]*");//ftpЭ������
	regex magnet_Mold(config.url_mold_3);//"magnet:\\?xt=urn:btih:[^\\s]*");//������������
	regex ED2K_Mold(config.url_mold_4);//"(ed2k://\\|file\\|){1}.*");//��¿��ʽ����
	switch (1)//�����ж�url�Ƿ����������Э��
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
