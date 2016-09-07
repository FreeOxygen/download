//-------------------- include --------------
#include "config.h"
#include "urlStart.h"
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <time.h>
#include <tchar.h>
#include "urlMysql.h"
using namespace std;
HWND hwnd;
//-------------------- function ------------


/*
获取活动窗口左上角相对于屏幕的坐标
x 横坐标
y 纵坐标
*/
void getPos(HWND h, int* x, int* y)
{
	int x1, x2, y1, y2;

	// 获取屏幕鼠标坐标
	POINT pt;
	GetCursorPos(&pt);
	// printf("%d %d\n",pt.x,pt.y);
	x1 = pt.x;
	y1 = pt.y;
	// 获取窗口鼠标坐标
	ScreenToClient(h, &pt);
	//  printf("%d %d\n",pt.x,pt.y);
	x2 = pt.x;
	y2 = pt.y;

	//获得窗口左上角坐标
	*x = x1 - x2;
	*y = y1 - y2;
}

/*
获取得到焦点窗口的名称
title 窗口的名称
*/
void getText(HWND h, char* title)
{

	// 获取窗口标题
	char text[200];
	GetWindowText(h, text, 200);
	//    printf("%s\n",text);
	strcpy(title, text);

}

/*
	操作迅雷，创建下载任务
	urlNum 读取到的第几条URL
	url URL

	return 创建成功返回1，错误返回0
*/
int urlStart(int urlNum, char* url)
{
	HINSTANCE hInstance;
	HWND newHwnd;
	int x, y;

	if (!IsWindowVisible(hwnd)) //判断窗口是否显示
	{
		ShowWindow(hwnd, SW_SHOW);//让窗口显示
	}
	
	char title1[200] = { 0 };
	getText(hwnd, title1);//获取主窗口标题

	SetForegroundWindow(hwnd);
	getPos(hwnd,&x, &y);//获取主窗口左上角坐标
	SetFocus(hwnd);//主窗口得到焦点
	SetCursorPos(x + 60, y + 83);//移动鼠标点击新建任务
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(1000);

	//SetFocus(hwnd);
	newHwnd = GetForegroundWindow();//获得新建任务的句柄
	keyboardInput(url);//输入URL
	Sleep(1000);

	getPos(newHwnd,&x, &y);//获得新建文件窗口的坐标
	//SetFocus(hwnd);
	SetCursorPos(x + 200, y + 250);//移动鼠标点击存储地址
	Sleep(1000);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//点击存储地址编辑框

	Sleep(500);

	keybd_event(17, 0, 0, 0);//将默认路径全选
	keybd_event(65, 0, 0, 0);

	keybd_event(65, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(17, 0, KEYEVENTF_KEYUP, 0);//删除默认存储路径

	keybd_event(17, 0, 0, 0);
	keybd_event(88, 0, 0, 0);

	keybd_event(88, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(17, 0, KEYEVENTF_KEYUP, 0);

	Sleep(500);
	char path[255];
	strcpy(path, config.savePath);//获得保存的文件地址


	strcat(path, "\\");
	keyboardInput(path);


	//获得当前系统的时间，用于命名文件夹
	SYSTEMTIME systim;
	GetLocalTime(&systim);
	int sptk = 0;
	char savepath[126] = { 0 };

	sptk += sprintf(savepath, "%d_", urlNum);
	sptk += sprintf(savepath + sptk, "%4d-", systim.wYear);
	sptk += sprintf(savepath + sptk, "%02d-", systim.wMonth);
	sptk += sprintf(savepath + sptk, "%02d-", systim.wDay);
	sptk += sprintf(savepath + sptk, "%02d-", systim.wHour);
	sptk += sprintf(savepath + sptk, "%02d-", systim.wMinute);//获取时间
	sptk += sprintf(savepath + sptk, "%02d_", systim.wSecond);
	sptk += sprintf(savepath + sptk, "%03d", systim.wMilliseconds);
	printf("this is important: %s",savepath);
	keyboardInput(savepath);//输入文件夹名称


	/*mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(1000);*/
	//SetFocus(newHwnd);
	SetCursorPos(x + 200, y + 300);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//点击下载
	Sleep(2000);

	char title2[200] = { 0 };
	HWND h = GetForegroundWindow();
	getText(h,title2);//获取当前窗口的标题
	cout << title1 << endl << title2 << endl;

	Sleep(1000);
	int result = strcmp(title2, title1);
	if (result != 0) {
		//连接地址不正确
		//SetFocus(hwnd);
		//SetForegroundWindow(hwnd);
		keybd_event(27, 0, 0, 0);//按下esc键退出新建下载
		keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
		files file;//保存错误文件
		strcpy(file.dir, savepath);
		strcpy(file.file_state, "3");
		insert_file(file);
		printf("download failed\n");
		return 0;
	}
	files file;//可以下载的文件
	sprintf(file.dir,"%s", savepath);
	strcpy(file.file_state, "1");
	insert_file(file);

	return 1;
}


/*
	模拟键盘输入
	value 输入的内容
*/
void keyboardInput(char* value)
{

	int i = 0;
	int keyValue = 0;
	while (value[i] != '\0')
	{
		if (value[i] >= 97 && value[i] <= 122)
		{
			keyValue = value[i] - 32;
			keybd_event(keyValue, (BYTE)0, 0, 0);

			keybd_event(keyValue, (BYTE)0, KEYEVENTF_KEYUP, 0);

		}
		else if (value[i] >= 65 && value[i] <= 90)
		{

			keyValue = value[i];
			keybd_event(16, 0, 0, 0);
			keybd_event(keyValue, (BYTE)0, 0, 0);

			keybd_event(keyValue, (BYTE)0, KEYEVENTF_KEYUP, 0);
			keybd_event(16, 0, KEYEVENTF_KEYUP, 0);

		}
		else
		{


			switch (value[i])
			{
			case ';':

				keybd_event(186, 0, 0, 0);
				keybd_event(186, 0, KEYEVENTF_KEYUP, 0);

				break;
			case ':':
				keybd_event(16, 0, 0, 0);
				keybd_event(186, 0, 0, 0);
				keybd_event(186, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '/':
				keybd_event(191, 0, 0, 0);
				keybd_event(191, 0, KEYEVENTF_KEYUP, 0);
				break;


			case '?':
				keybd_event(16, 0, 0, 0);
				keybd_event(191, 0, 0, 0);
				keybd_event(191, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '.':

				keybd_event(190, 0, 0, 0);
				keybd_event(190, 0, KEYEVENTF_KEYUP, 0);

				break;

			case '>':
				keybd_event(16, 0, 0, 0);
				keybd_event(190, 0, 0, 0);
				keybd_event(190, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case ',':

				keybd_event(188, 0, 0, 0);
				keybd_event(188, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '<':
				keybd_event(16, 0, 0, 0);
				keybd_event(188, 0, 0, 0);
				keybd_event(188, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '\'':

				keybd_event(222, 0, 0, 0);
				keybd_event(222, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '"':
				keybd_event(16, 0, 0, 0);
				keybd_event(222, 0, 0, 0);
				keybd_event(222, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '`':

				keybd_event(222, 0, 0, 0);
				keybd_event(222, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '~':
				keybd_event(16, 0, 0, 0);
				keybd_event(192, 0, 0, 0);
				keybd_event(192, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;


			case '-':

				keybd_event(189, 0, 0, 0);
				keybd_event(189, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '_':
				keybd_event(16, 0, 0, 0);
				keybd_event(189, 0, 0, 0);
				keybd_event(189, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;



			case '[':

				keybd_event(219, 0, 0, 0);
				keybd_event(219, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '{':
				keybd_event(16, 0, 0, 0);
				keybd_event(219, 0, 0, 0);
				keybd_event(219, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case ']':

				keybd_event(221, 0, 0, 0);
				keybd_event(221, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '}':
				keybd_event(16, 0, 0, 0);
				keybd_event(221, 0, 0, 0);
				keybd_event(221, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;



			case '\\':

				keybd_event(220, 0, 0, 0);
				keybd_event(220, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '|':
				keybd_event(16, 0, 0, 0);
				keybd_event(220, 0, 0, 0);
				keybd_event(220, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '=':

				keybd_event(220, 0, 0, 0);
				keybd_event(220, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '+':
				keybd_event(16, 0, 0, 0);
				keybd_event(187, 0, 0, 0);
				keybd_event(187, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;


			case '0':

				keybd_event(48, 0, 0, 0);
				keybd_event(48, 0, KEYEVENTF_KEYUP, 0);

				break;


			case ')':
				keybd_event(16, 0, 0, 0);
				keybd_event(48, 0, 0, 0);
				keybd_event(48, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '1':

				keybd_event(49, 0, 0, 0);
				keybd_event(49, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '!':
				keybd_event(16, 0, 0, 0);
				keybd_event(49, 0, 0, 0);
				keybd_event(49, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '2':

				keybd_event(50, 0, 0, 0);
				keybd_event(50, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '@':
				keybd_event(16, 0, 0, 0);
				keybd_event(50, 0, 0, 0);
				keybd_event(50, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '3':

				keybd_event(51, 0, 0, 0);
				keybd_event(51, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '#':
				keybd_event(16, 0, 0, 0);
				keybd_event(51, 0, 0, 0);
				keybd_event(51, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;



			case '4':

				keybd_event(52, 0, 0, 0);
				keybd_event(52, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '$':
				keybd_event(16, 0, 0, 0);
				keybd_event(52, 0, 0, 0);
				keybd_event(52, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '5':

				keybd_event(53, 0, 0, 0);
				keybd_event(53, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '%':
				keybd_event(16, 0, 0, 0);
				keybd_event(53, 0, 0, 0);
				keybd_event(53, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '6':

				keybd_event(54, 0, 0, 0);
				keybd_event(54, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '^':
				keybd_event(16, 0, 0, 0);
				keybd_event(54, 0, 0, 0);
				keybd_event(54, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '7':

				keybd_event(55, 0, 0, 0);
				keybd_event(55, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '&':
				keybd_event(16, 0, 0, 0);
				keybd_event(55, 0, 0, 0);
				keybd_event(55, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;


			case '8':

				keybd_event(56, 0, 0, 0);
				keybd_event(56, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '*':
				keybd_event(16, 0, 0, 0);
				keybd_event(56, 0, 0, 0);
				keybd_event(56, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;

			case '9':

				keybd_event(57, 0, 0, 0);
				keybd_event(57, 0, KEYEVENTF_KEYUP, 0);

				break;


			case '(':
				keybd_event(16, 0, 0, 0);
				keybd_event(57, 0, 0, 0);
				keybd_event(57, 0, KEYEVENTF_KEYUP, 0);
				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
				break;



			case '\n':

				keybd_event(13, 0, 0, 0);
				keybd_event(13, 0, KEYEVENTF_KEYUP, 0);

				break;
			}
		}


		i++;
	}

}

/*
	获取窗口的信息，
*/
void getInfo()
{
	int x1, x2, y1, y2;

	// 获取活动窗口
	HWND h = GetForegroundWindow();
	//printf("0x%X\n",h);
	// 获取窗口标题
	char text[200];
	GetWindowText(h, text, 200);
	printf("%s\n", text);
	// 获取屏幕鼠标坐标
	POINT pt;
	GetCursorPos(&pt);
	printf("%d %d\n", pt.x, pt.y);
	x1 = pt.x;
	y1 = pt.y;
	// 获取窗口鼠标坐标
	ScreenToClient(h, &pt);
	printf("%d %d\n", pt.x, pt.y);
	x2 = pt.x;
	y2 = pt.y;

	SetCursorPos(x1 - x2, y1 - y2);
	printf("xiangduizuobiao  :%d %d\n", x1 - x2, y1 - y2);
	Sleep(10000);

}

struct ProcessWindow
{
	DWORD dwProcessId;
	HWND hwndWindow;
}; 

BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{
	ProcessWindow *pProcessWindow = (ProcessWindow *)lParam;

	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd, &dwProcessId);

	// 判断是否是指定进程的主窗口   
	if (pProcessWindow->dwProcessId == dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL)
	{
		pProcessWindow->hwndWindow = hWnd;

		return FALSE;
	}

	return TRUE;
}
void open_xunlei()
{
	// 进程启动信息   
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	// 进程信息   
	PROCESS_INFORMATION pi;

	// 创建进程   
	// 第二个参数是要启动的应用程序路径名，比如：C:\Test.exe   
	cout << config.xunlei << endl;
	if (CreateProcess(config.xunlei, NULL/* config.xunlei*/, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	{
		ProcessWindow procwin;
		procwin.dwProcessId = pi.dwProcessId;
		procwin.hwndWindow = NULL;
		// 等待新进程初始化完毕   
		//WaitForInputIdle(pi.hProcess, 5000);
		Sleep(5000);
		// 查找主窗口   
		EnumWindows(EnumWindowCallBack, (LPARAM)&procwin);
		hwnd = procwin.hwndWindow;
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//让窗口置于顶层
		cout << hex <<pi.dwThreadId << endl;
		cout << hex <<pi.dwProcessId << endl;
		char buf[200];
		GetWindowText(hwnd, buf, 200);
		cout << "open exe:"<<buf << endl;
	}
	else 
	{
		cout << GetLastError() << endl;
	}
}