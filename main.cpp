//---------------- include ---------------
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include "config.h"
#include "urlMysql.h"
#include "xunlei.h"
#include "urlOper.h"
using namespace std;

//------------------- function -------------
int main()
{
	init_config();//初始化配置参数
	open_xunlei(config.xunlei);
	cout << "迅雷启动成功" << endl;
	initMysql();

	int i = 0;
	while (1) //检测文件是否下载完成
	{
		url_info info;
		int sumTask = getSumTask();//获得已经建立多少任务
		cout << "********任务数：" << sumTask << endl;
		if ((-1 != sumTask) && (sumTask < config.MaxTask))
		{
			if (read_url(info))
			{
				if (is_url_valid(info))//判断url是否有效
				{
					cout << info.url << endl;
					Sleep(2000);
					if (xunlei_add_url(info))
					{
						//成功建立下载
						//filepath;protocol;remark;start_time;state;tool;url;
						update_Success_url(info);
					}
					else
					{
						//建立连接错误
						//protocol;remark;state;tool;url;
						update_Error_url(info);
					}
				}
				//URL是不符合要求的
				else
				{
					info.state = DL_FAIL;
					info.remark = RE_URL_error;
					update_Error_url(info);
				}
			}
			else
			{
				//读取数据库中可以再次尝试的连接
				if (get_againURL())
				{
					//得到一条url
					while (get_again_URL(info))
					{
						Sleep(2000);
						if (xunlei_add_url(info))
						{
							//成功建立下载
							//filepath;protocol;remark;start_time;state;tool;url;
							update_AgainSuccess_url(info);
						}
						else
						{
							//建立连接错误
							//protocol;remark;state;tool;url;
							update_AgainError_url(info);
						}
					}
				}
			}
		}
		update_state();
		Sleep(2000);
	}
	return 0;
}



