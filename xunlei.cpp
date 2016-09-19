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

// constant definition
const int MAX_WAIT_ROUND = 3;

//-------------- gloable variable -------
HWND g_hwnd;//打开迅雷的窗口句柄

int write_Clip(char * buf);
int waitForInput(HWND hwnd);
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
	int find_ROUND = MAX_WAIT_ROUND;
	do
	{
		g_hwnd = FindWindow("XLUEFrameHostWnd", "迅雷");
		if (GetParent(g_hwnd) == NULL)
		{
			break;
		}
		Sleep(1000);
	} while (find_ROUND--);
	
	if (NULL == g_hwnd)
	{
		//迅雷没有启动
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
			g_hwnd = procwin.hwndWindow;
			//SetWindowPos(g_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//让窗口置于顶层
			char buf[200];
			GetWindowText(g_hwnd, buf, 200);
			cout << "open exe:" << buf << endl;
			return 1;//打开成功
		}
		else
		{
			cout << GetLastError() << endl;
			return 0;//迅雷打开错误
		}
	}
	else
	{
		char buf[255];
		GetWindowText(g_hwnd, buf, 255);
		cout << buf << "：打开成功" << endl;
		return 0;
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
	HWND newHwnd = NULL;//新建窗口的句柄

	int x, y;

	if (!IsWindowVisible(g_hwnd)) //判断窗口是否显示
	{
		ShowWindow(g_hwnd, SW_SHOW);//让窗口显示
	}

	//磁力链接
	if (info.protocol == magnet)
	{
		SetForegroundWindow(g_hwnd);
		PostMessage(g_hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//点击新建按键
		PostMessage(g_hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
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
			Sleep(1000);
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));


		SetForegroundWindow(newHwnd);
		write_Clip(info.url);//将url写入剪贴板
		//ctrl + V
		keybd_event(VK_CONTROL, 0, 0, 0);
		keybd_event('V', 0, 0, 0);
		keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		Sleep(1000);//等待输入完成
		PostMessage(newHwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(183, 219));//点击下载按键
		PostMessage(newHwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(183, 219));

		HWND hwnd_NewBT = NULL;
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

			//tab键
			keybd_event(VK_TAB, 0, 0, 0);
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

			keybd_event(VK_CONTROL, 0, 0, 0);//将默认路径全选
			keybd_event('A', 0, 0, 0);

			keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);//删除默认存储路径

			write_Clip(path);
			//ctrl + V
			keybd_event(VK_CONTROL, 0, 0, 0);
			keybd_event('V', 0, 0, 0);
			keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
			keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

			Sleep(1000);//等待输入完成
			PostMessage(hwnd_NewBT, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(155, 531));//点击下载按键
			PostMessage(hwnd_NewBT, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(155, 531));

			//下载链接错误
			int wait_cont = MAX_WAIT_ROUND;
			while (IsWindowVisible(hwnd_NewBT) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
			{
				//连接地址不正确
				if (wait_cont < 0)
				{
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
					keybd_event(VK_ESCAPE, 0, 0, 0);//按下esc键退出新建下载
					keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
					Sleep(1000);
					printf("download failed\n");
					return 0;
				}
				Sleep(1000);
				wait_cont--;
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
		SetForegroundWindow(g_hwnd);
		PostMessage(g_hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//点击新建按键
		PostMessage(g_hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
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

			Sleep(1000);
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));

		char titleTmp[200];
		GetWindowText(newHwnd, titleTmp, 200);
		cout << "打开的窗口:" << titleTmp << endl;

		SetForegroundWindow(newHwnd);
		write_Clip(info.url);//将url写入剪贴板
		//ctrl + V
		keybd_event(VK_CONTROL, 0, 0, 0);
		keybd_event('V', 0, 0, 0);
		keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
		Sleep(1000);

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

		for (int n = MAX_WAIT_ROUND; n < -1; n--)
		{
			if (waitForInput(newHwnd))
			{
				break;
			}
			if (0 == n)
			{
				//url 解析失败
				info.state = DL_FAIL;//文件下载失败
				info.remark = RE_Link_error;
				cout << "url 解析失败" << endl;
				return 0;
			}
			Sleep(1000);
		}

		SetForegroundWindow(newHwnd);
		//tab键
		keybd_event(VK_TAB, 0, 0, 0);
		keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);

		keybd_event(VK_CONTROL, 0, 0, 0);//将默认路径全选
		keybd_event('A', 0, 0, 0);

		keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);//删除默认存储路径

		write_Clip(path);
		//ctrl + V
		keybd_event(VK_CONTROL, 0, 0, 0);
		keybd_event('V', 0, 0, 0);
		keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

		//keyboardInput(path);//输入存储路径
		Sleep(1000);
		PostMessage(newHwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(203, 296));//点击下载按键
		PostMessage(newHwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(203, 296));

		int wait_cont = MAX_WAIT_ROUND;
		while (IsWindowVisible(newHwnd) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
		{
			//连接地址不正确
			if (wait_cont < 0)
			{
				if (IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
				{
					info.state = DL_FAIL;//文件下载失败
					info.remark = RE_Download_repeat;
				}
				keybd_event(VK_ESCAPE, 0, 0, 0);//按下esc键退出新建下载
				keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
				Sleep(1000);
				printf("download failed\n");
				return 0;
			}
			Sleep(1000);
			wait_cont--;
		}
		info.state = DL_RUN;//状态正在下载
		strcpy(info.filepath, path);
		info.remark = RE_Run;

		return 1;
	}
}

/*等待窗口准备好输入存储路径
  即：判断窗口中是否有文件夹开启的button*/
int waitForInput(HWND hwnd)
{

	RECT rect;
	GetWindowRect(hwnd, (LPRECT)&rect);
	HDC wndDC = GetDC(NULL);

	COLORREF color = GetPixel(wndDC, rect.left + 365, rect.top + 250);
	ReleaseDC(NULL, wndDC);

	if (0x1b4ff == color)//检测到橘黄色
	{
		return 0;
	}
	return 1;

}
//写剪贴板
int write_Clip(char * buf)
{
	if (OpenClipboard(NULL))//打开剪贴板  
	{
		HGLOBAL hClip;
		char * pBuf;
		EmptyClipboard();//清空剪贴板

		hClip = GlobalAlloc(GPTR, (strlen(buf) + 1));//创建一个win32的内存块
		pBuf = (char*)GlobalLock(hClip);//锁定内存块
		strcpy(pBuf, buf);
		GlobalUnlock(hClip);//释放类存快所有权 
		SetClipboardData(CF_TEXT, hClip);//将文本放入剪贴板  

		CloseClipboard();//关闭剪贴板 
		return 1;
	}
	else
	{
		return 0;
	}
}
