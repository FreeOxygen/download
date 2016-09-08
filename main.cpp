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
///状态中0为完成，1为正在下载，2为下载错误，3为无效链接

//------------------- function -------------
int main()
{
	init_config();//初始化配置参数
	open_xunlei();//启动迅雷
	cout << "迅雷启动成功:" << endl;
	initMysql();
	find_new_file(config.urlPath);

	int i = 0;
	while (1) //检测文件是否下载完成
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



