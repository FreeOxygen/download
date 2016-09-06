#ifndef _CONFIG_H_INCLUDED_
#define _CONFIG_H_INCLUDED_

#include <stdlib.h>
#include <string.h>
#include <iostream>
using  namespace std;
void init(char* name,char * path);

void init_config();

struct Config
{
	char xunlei[256];
	char urlPath[256];
	char savePath[256];
	char user[256];
	char passwd[256];
	char host[256];
	char dbName[256];
	char tableName[256];
	char url_mold_1[256];
	char url_mold_2[256];
	char url_mold_3[256];
	char url_mold_4[256];
};
extern Config config;

#endif
