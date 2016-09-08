#ifndef _URLFILE_H_INCLUDED_
#define _URLFILE_H_INCLUDED_
#include <iostream>
#include <fstream>
#include <regex>

using std::vector;
int open_urlFile(char* fileName);
int read_url(char * readUrl);
bool is_url_valid(char* url);

#endif


