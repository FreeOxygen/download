//---------------- include ------------
#include <iostream>
#include <io.h>
#include <time.h>
#include "urlFile.h"
using namespace std;

//---------------- function -----------
/*
	判断是否有性文件
	
*/

int find_new_file(const char * Path)
{
	char findPath[1024];
	strcpy(findPath, Path);
	strcat(findPath, "\\*");

	_finddata_t findFile;//获取得到的文件信息

	intptr_t hFile;//查找到的文件句柄

	if ((hFile = _findfirst(findPath, &findFile)) == -1L)
	{
		_findclose(hFile);
		cout << "输入路径错误" << endl;
		return 0;
	}
	else {

		do {
			if (findFile.attrib & _A_SUBDIR) //查询到的为文件夹
			{
					continue;
			}
			else
			{
				char tmp[1024];
				strcpy(tmp, Path);
				strcat(tmp, "\\");
				strcat(tmp, findFile.name);
				if (open_urlFile(tmp))//文件打开成功
				{
					cout << "打开文件：" << findFile.name << endl;
					_findclose(hFile);
					return 1;
				}
			}
		} while (_findnext(hFile, &findFile) == 0);
		_findclose(hFile);
		cout << "没有新文件" << endl;
		return 0;
	}
}