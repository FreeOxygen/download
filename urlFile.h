#ifndef _URLFILE_H_INCLUDED_
#define _URLFILE_H_INCLUDED_
#include <iostream>
#include <fstream>
#include <regex>
#include "urlMysql.h"

using std::vector;
int open_urlFile(char* fileName);
int read_url(url_info & readUrl_info);
int read_url(char * readUrl_info);
bool is_url_valid(char* url);
bool is_url_valid(char* url, url_info & readurl_info);

#endif


