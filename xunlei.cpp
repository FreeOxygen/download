//-------------- include ----------------
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <time.h>
#include "xunlei.h"
using namespace std;

//-------------- gloable variable -------
HWND hwnd;//打开迅雷的窗口句柄


//-------------- functions --------------
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

/*
打开迅雷
	xunleiPath 迅雷.exe文件的路径
*/
int open_xunlei(char * xunleiPath)
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
	if (CreateProcess(xunleiPath, NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	{
		ProcessWindow procwin;
		procwin.dwProcessId = pi.dwProcessId;
		procwin.hwndWindow = NULL;
		// 等待新进程初始化完毕   
		Sleep(5000);
		// 查找主窗口   
		EnumWindows(EnumWindowCallBack, (LPARAM)&procwin);
		hwnd = procwin.hwndWindow;
		//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//让窗口置于顶层
		char buf[200];
		GetWindowText(hwnd, buf, 200);
		cout << "open exe:" << buf << endl;
		return 1;//打开成功
	}
	else
	{
		cout << GetLastError() << endl;
		return 0;//迅雷打开错误
	}
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

				keybd_event(187, 0, 0, 0);
				keybd_event(187, 0, KEYEVENTF_KEYUP, 0);

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
	x1 = pt.x;
	y1 = pt.y;
	ScreenToClient(h, &pt);
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
	strcpy(title, text);

}
/*
添加下载任务
	info 读取到的URL地址的信息
return 0添加错误 1 添加成功
*/
int xunlei_add_url(url_info & info)
{
	HWND newHwnd = NULL;
	int x, y;

	if (!IsWindowVisible(hwnd)) //判断窗口是否显示
	{
		ShowWindow(hwnd, SW_SHOW);//让窗口显示
	}

	char title1[200] = { 0 };
	getText(hwnd, title1);//获取主窗口标题
	
	//磁力链接
	if (info.protocol == magnet)
	{
		SetForegroundWindow(hwnd);
		PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//点击新建按键
		PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
		int tmp = 0;
		//创建下载任务错误
		do
		{
			tmp++;
			newHwnd = FindWindow(NULL, "新建下载");//得到新建下载的句柄
			if (tmp > 10000)
			{
				info.remark = RE_xunlei_error;//迅雷创建任务错误
				info.state = DL_FAIL;
				cout << "*********新建任务错误**********" << endl;
				return 0;
			}
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));

		char titleTmp[200];
		GetWindowText(newHwnd, titleTmp, 200);
		cout << "打开的窗口:" << tmp << endl;

		int cont = 0;
		//窗口错误
		while (!IsWindowVisible(newHwnd))
		{
			cont++;
			if (cont > 1000)
			{
				info.remark = RE_xunlei_error;//迅雷创建任务错误
				info.state = DL_FAIL;
				cout << "*************新建窗口错误***********" << endl;
				return 0;
			}
		}

		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(info.url);//输入URL
		Sleep(1000);
		getPos(newHwnd, &x, &y);//获得新建文件窗口的坐标
		SetCursorPos(x + 100, y + 230);//移动鼠标点击下载
		Sleep(1000);

		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//点击下载
		Sleep(1000);
		HWND hwnd_Megnet;
		HWND hwnd_NewBT = NULL;
		hwnd_Megnet = FindWindow(NULL, "磁力链解析");//得到磁力链解析的句柄
		SetForegroundWindow(hwnd_Megnet);
		int i;
		for (i = 0; hwnd_NewBT == NULL&&i < 500; i++) {
			hwnd_NewBT = FindWindow(NULL, "新建BT任务");

			Sleep(1000);
		}

		//资源解析错误
		if (i == 1000) {
			info.state = DL_FAIL;//文件下载失败
			info.remark = RE_Parse_error;//链接解析错误
			cout << "种子下载失败" << endl;
			return 0;
		}

		else {
			SetForegroundWindow(hwnd_NewBT);
			//获得当前系统的时间，用于命名文件夹
			SYSTEMTIME systim;
			GetLocalTime(&systim);
			int sptk = 0;
			char savepath[126] = { 0 };

			sptk += sprintf(savepath + sptk, "%4d-", systim.wYear);
			sptk += sprintf(savepath + sptk, "%02d-", systim.wMonth);
			sptk += sprintf(savepath + sptk, "%02d_", systim.wDay);
			sptk += sprintf(savepath + sptk, "%02d_", systim.wHour);
			sptk += sprintf(savepath + sptk, "%02d_", systim.wMinute);//获取时间
			sptk += sprintf(savepath + sptk, "%02d_", systim.wSecond);
			sptk += sprintf(savepath + sptk, "%03d", systim.wMilliseconds);
			cout << "this is important" << savepath << endl;

			char path[255];
			strcpy(path, config.savePath);//获得保存的文件地址

			strcat(path, "/");
			strcat(path, savepath);//存储的地址
			strtok(savepath, "_");
			strcpy(info.start_time, savepath);//年月日
			strcat(info.start_time, " ");
			strcat(info.start_time, strtok(NULL, "_"));//时
			strcat(info.start_time, ":");
			strcat(info.start_time, strtok(NULL, "_"));//分
			strcat(info.start_time, ":");
			strcat(info.start_time, strtok(NULL, "_"));//秒


			getPos(hwnd_NewBT, &x, &y);//获得新建文件窗口的坐标
			Sleep(500);
			SetCursorPos(x + 150, y + 500);//移动鼠标点击存储地址
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//点击存储地址编辑框
			Sleep(1000);

			keybd_event(17, 0, 0, 0);//将默认路径全选
			keybd_event(65, 0, 0, 0);

			keybd_event(65, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(17, 0, KEYEVENTF_KEYUP, 0);//删除默认存储路径

			keybd_event(17, 0, 0, 0);
			keybd_event(88, 0, 0, 0);

			keybd_event(88, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(17, 0, KEYEVENTF_KEYUP, 0);
			Sleep(1000);
			SetForegroundWindow(hwnd_NewBT);
			keyboardInput(path);//输入存储路径
			Sleep(1000);


			SetCursorPos(x + 200, y + 530);

			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//点击下载
			Sleep(1000);

			char title2[200] = { 0 };
			HWND h = GetForegroundWindow();
			getText(h, title2);//获取当前窗口的标题
			cout << title1 << "-----" << title2 << endl;
			
			//下载链接错误
			if (IsWindowVisible(hwnd_NewBT) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
			{
				//连接地址不正确
				if (IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
				{
					info.state = DL_FAIL;//文件下载失败
					info.remark = RE_Download_repeat;
				}
				else
				{
					info.state = DL_FAIL;//文件下载失败
					info.remark = RE_Link_error;
				}
				keybd_event(27, 0, 0, 0);//按下esc键退出新建下载
				keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
				printf("download failed\n");
				return 0;
			}

			info.state = DL_RUN;//状态正在下载
			strcpy(info.filepath, path);
			info.remark = RE_Run;
			return 1;

		}
	}
	//普通连接
	else
	{
		SetForegroundWindow(hwnd);
		PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//点击新建按键
		PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
		int tmp = 0;
		//得到新建下载的句柄

		//打开新建错误
		do
		{
			tmp++;
			newHwnd = FindWindow(NULL, "新建下载");
			if (tmp > 10000)
			{
				info.state = DL_FAIL;
				info.remark = RE_xunlei_error;
				cout << "*********新建任务错误**********" << endl;
				return 0;
			}
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));

		char titleTmp[200];
		GetWindowText(newHwnd, titleTmp, 200);
		cout << "打开的窗口:" << tmp << endl;

		int cont = 0;
		//判断新建窗口是否打开

		//新建窗口没有打开
		while (!IsWindowVisible(newHwnd))
		{
			cont++;
			if (cont > 1000)
			{
				info.state = DL_FAIL;
				info.remark = RE_xunlei_error;
				cout << "*************新建窗口错误***********" << endl;
				return 0;
			}
		}


		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(info.url);//输入URL
		Sleep(1000);

		//获得当前系统的时间，用于命名文件夹
		SYSTEMTIME systim;
		GetLocalTime(&systim);
		int sptk = 0;
		char savepath[126] = { 0 };

		/*sptk += sprintf(savepath, "%d_", urlNum);*/
		sptk += sprintf(savepath + sptk, "%4d-", systim.wYear);
		sptk += sprintf(savepath + sptk, "%02d-", systim.wMonth);
		sptk += sprintf(savepath + sptk, "%02d_", systim.wDay);
		sptk += sprintf(savepath + sptk, "%02d_", systim.wHour);
		sptk += sprintf(savepath + sptk, "%02d_", systim.wMinute);//获取时间
		sptk += sprintf(savepath + sptk, "%02d_", systim.wSecond);
		sptk += sprintf(savepath + sptk, "%03d", systim.wMilliseconds);
		cout << "this is important" << savepath << endl;

		char path[255];
		strcpy(path, config.savePath);//获得保存的文件地址

		strcat(path, "/");
		strcat(path, savepath);//存储的地址
		strtok(savepath, "_");
		strcpy(info.start_time, savepath);//年月日
		strcat(info.start_time, " ");
		strcat(info.start_time, strtok(NULL, "_"));//时
		strcat(info.start_time, ":");
		strcat(info.start_time, strtok(NULL, "_"));//分
		strcat(info.start_time, ":");
		strcat(info.start_time, strtok(NULL, "_"));//秒

		getPos(newHwnd, &x, &y);//获得新建文件窗口的坐标
		SetCursorPos(x + 200, y + 250);//移动鼠标点击存储地址
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//点击存储地址编辑框
		Sleep(1000);
		//Sleep(500);
		keybd_event(17, 0, 0, 0);//将默认路径全选
		keybd_event(65, 0, 0, 0);

		keybd_event(65, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(17, 0, KEYEVENTF_KEYUP, 0);//删除默认存储路径

		keybd_event(17, 0, 0, 0);
		keybd_event(88, 0, 0, 0);

		keybd_event(88, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(17, 0, KEYEVENTF_KEYUP, 0);
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(path);//输入存储路径
		Sleep(1000);

		SetCursorPos(x + 200, y + 300);
		SetForegroundWindow(newHwnd);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//点击下载
		Sleep(1000);

		char title2[200] = { 0 };
		HWND h = GetForegroundWindow();
		getText(h, title2);//获取当前窗口的标题
		cout << title1 << "-----" << title2 << endl;

		if (IsWindowVisible(newHwnd) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
		{
			//连接地址不正确
			if (IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
			{
				info.state = DL_FAIL;//文件下载失败
				info.remark = RE_Download_repeat;
			}
			else
			{
				info.state = DL_FAIL;//文件下载失败
				info.remark = RE_Link_error;
			}
			keybd_event(27, 0, 0, 0);//按下esc键退出新建下载
			keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
			printf("download failed\n");
			return 0;
		}
		info.state = DL_RUN;//状态正在下载
		strcpy(info.filepath, path);
		info.remark = RE_Run;

		return 1;
	}
}
