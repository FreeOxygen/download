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
	open_xunlei(g_config.xunlei);//启动迅雷

	while (1) //检测文件是否下载完成
	{
		if (open_Mysql())
		{
			url_info info;
			update_state();//更新数据库状态
			int sumTask = getSumTask();//获得已经建立多少任务
			int ErrSumTask = getErrSumTask();//获得不能下载的的任务
			if ((g_config.MaxTask * 3 / 4) >= ErrSumTask)//当错误的任务数量大于总任务数的一半
			{
				cout << "********迅雷现在的任务数：" << sumTask << endl;
				if ((-1 != sumTask) && (sumTask < g_config.MaxTask))
				{
					if (read_url(info))
					{
						if (is_url_valid(info))//判断url是否有效
						{
							cout << info.url << endl;
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
								Sleep(1000);
							}
						}
					}
				}
			}
			else
			{
				//错误任务数量过多
				if (sumTask == ErrSumTask)
				{
					//没有正在下载的任务
					if (close_xunlei())
					{
						open_xunlei(g_config.xunlei);
						update_errSatk_state();
					}
				}
			}
			close_Mysql();//关闭数据库连接
		}
		Sleep(g_config.WaitTime);
	}
	return 0;
}



