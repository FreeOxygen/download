//---------------- include ------------
#include <iostream>
#include <io.h>
#include <time.h>
#include "urlFile.h"
using namespace std;

//---------------- function -----------
/*
	�ж��Ƿ������ļ�
	
*/

int find_new_file(const char * Path)
{
	char findPath[1024];
	strcpy(findPath, Path);
	strcat(findPath, "\\*");

	_finddata_t findFile;//��ȡ�õ����ļ���Ϣ

	intptr_t hFile;//���ҵ����ļ����

	if ((hFile = _findfirst(findPath, &findFile)) == -1L)
	{
		_findclose(hFile);
		cout << "����·������" << endl;
		return 0;
	}
	else {

		do {
			if (findFile.attrib & _A_SUBDIR) //��ѯ����Ϊ�ļ���
			{
					continue;
			}
			else
			{
				char tmp[1024];
				strcpy(tmp, Path);
				strcat(tmp, "\\");
				strcat(tmp, findFile.name);
				if (open_urlFile(tmp))//�ļ��򿪳ɹ�
				{
					cout << "���ļ���" << findFile.name << endl;
					_findclose(hFile);
					return 1;
				}
			}
		} while (_findnext(hFile, &findFile) == 0);
		_findclose(hFile);
		cout << "û�����ļ�" << endl;
		return 0;
	}
}