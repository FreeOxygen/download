//---------------- include ---------------
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <regex>
#include "urlStart.h"
#include "urlFile.h"
#include "config.h"
#include "urlMysql.h"
using namespace std;
using std::vector;
///״̬��0Ϊ��ɣ�1Ϊ�������أ�2Ϊ���ش���3Ϊ��Ч����

//------------------- function -------------
int main()
{
	int i = 1;
	init_config();//��ʼ�����ò���
	open_xunlei();//����Ѹ��
	cout << "Ѹ�������ɹ�:" << endl;
	initMysql();
	vector<char*> vec = UrlRead(config.urlPath);
	cout << "�����أ�" << vec.size() << endl;//����жϳ�������������
	vector<char*>::iterator v = vec.begin();//�����õ����п��õ�����
	while (v != vec.end())
	{
		cout << *v << endl;
		urlStart(i, *v);
		delete *v;
		v++;
		i++;
	}


	while (1) //����ļ��Ƿ��������
	{
		fileState(config.savePath);
		Sleep(20000);
	}
	system("pause");
	return 0;
}



