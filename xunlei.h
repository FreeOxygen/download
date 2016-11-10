#ifndef _XUNLEI_H_
#define _XUNLEI_H_
//------------ include ------------
#include "stdafx.h"

//------------ functions -------
int open_xunlei(char * xunleiPath);//打开迅雷
int xunlei_add_url(url_info & info);//迅雷中添加URL任务
int close_xunlei();//关闭迅雷删除 

#endif
