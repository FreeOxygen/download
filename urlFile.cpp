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
��һ��url�ļ�
fileName ��ȡ�ļ���·��
*/
int open_urlFile(char* fileName)
{
	fin.open(fileName);
	if (!fin.is_open())
	{
		cout << "***************" << "�ļ��򿪴���" << "**************" << endl;
		return 0;
	}
	else
	{
		strcpy(filename, fileName);
		cout << "url�ļ�:" << fileName << "--�򿪳ɹ�!" << endl;
		return 1;
	}
}

int read_url(char * readUrl)
{
	char buffer[1024];
	if (fin.is_open())//�ļ��Ѿ���
	{
		if (!fin.eof())
		{
			fin.getline(buffer, 1024);
			while (!is_url_valid(buffer))
			{
				if (fin.eof()) //�ļ��Ѿ�����
				{
					//ɾ���ļ�
					fin.close();
					remove(filename);//ɾ���Ѿ���ȡ��ɵ��ļ�
					if (find_new_file(config.urlPath))//�����µ��ļ�
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
			//ɾ���ļ�
			fin.close();
			remove(filename);//ɾ���Ѿ���ȡ��ɵ��ļ�
			if (find_new_file(config.urlPath))//�����µ��ļ�
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
		if (find_new_file(config.urlPath))//�����µ��ļ�
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
