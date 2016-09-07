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
///状态中0为完成，1为正在下载，2为下载错误，3为无效链接

//------------------- function -------------
int main()
{
	int i = 1;
	init_config();//初始化配置参数
	open_xunlei();//启动迅雷
	cout << "迅雷启动成功:" << endl;
	initMysql();
	vector<char*> vec = UrlRead(config.urlPath);
	cout << "共加载：" << vec.size() << endl;//获得判断出来的有用条数
	vector<char*>::iterator v = vec.begin();//迭代得到所有可用的连接
	while (v != vec.end())
	{
		cout << *v << endl;
		urlStart(i, *v);
		delete *v;
		v++;
		i++;
	}


	while (1) //检测文件是否下载完成
	{
		fileState(config.savePath);
		Sleep(20000);
	}
	system("pause");
	return 0;
}



