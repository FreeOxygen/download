#ifndef _URLSTART_H_INCLUDED_
#define _URLSTART_H_INCLUDED_

#include "urlMysql.h"
void keyboardInput(char* value);
//void getInfo();
int urlStart(int urlNum,char* url);
int urlStart(int urlNum, url_info & url);
void open_xunlei();

#endif
