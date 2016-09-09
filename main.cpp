//---------------- include ---------------
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include "urlStart.h"
#include "config.h"
#include "urlMysql.h"
#include "urlFile.h"
#include "urlSource.h"
using namespace std;

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
		url_info info;
		if (read_url(info))
		{
			i++;
			cout << info.url << endl;
			urlStart(i, info);
			info.DJID = i;
			info.JID = i;
			add_DL(info);
		}
		fileState(config.savePath);
		Sleep(2000);
	}
	system("pause");
	return 0;
}



