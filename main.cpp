//---------------- include ---------------
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <regex>
#include "urlStart.h"
#include "config.h"
#include "urlMysql.h"
#include "urlFile.h"
#include "urlSource.h"
using namespace std;
using std::vector;
///״̬��0Ϊ��ɣ�1Ϊ�������أ�2Ϊ���ش���3Ϊ��Ч����

//------------------- function -------------
int main()
{
	init_config();//��ʼ�����ò���
	open_xunlei();//����Ѹ��
	cout << "Ѹ�������ɹ�:" << endl;
	initMysql();
	find_new_file(config.urlPath);

	int i = 0;
	while (1) //����ļ��Ƿ��������
	{
		char url[1024];
		if (read_url(url))
		{
			i++;
			cout << url << endl;
			urlStart(i, url);
		}
		fileState(config.savePath);
		Sleep(2000);
	}
	system("pause");
	return 0;
}



