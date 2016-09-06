#ifndef _URLFILE_H_INCLUDED_
#define _URLFILE_H_INCLUDED_
#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

using std::vector;
vector<char*> UrlRead(char* fileName);
bool is_url_valid(char* url);

#endif


