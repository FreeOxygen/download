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
using namespace std;
HWND hwnd;

//-------------------- function -------------

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

	// �ж��Ƿ���ָ�����̵�������   
	if (pProcessWindow->dwProcessId == dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL)
	{
		pProcessWindow->hwndWindow = hWnd;

		return FALSE;
	}

	return TRUE;
}

void open_xunlei()
{
	// ����������Ϣ   
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	// ������Ϣ   
	PROCESS_INFORMATION pi;

	// ��������   
	// �ڶ���������Ҫ������Ӧ�ó���·���������磺C:\Test.exe   
	cout << config.xunlei << endl;
	if (CreateProcess(config.xunlei, NULL/* config.xunlei*/, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	{
		ProcessWindow procwin;
		procwin.dwProcessId = pi.dwProcessId;
		procwin.hwndWindow = NULL;
		// �ȴ��½��̳�ʼ�����   
		Sleep(5000);
		// ����������   
		EnumWindows(EnumWindowCallBack, (LPARAM)&procwin);
		hwnd = procwin.hwndWindow;
		//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//�ô������ڶ���
		cout << hex << pi.dwThreadId << endl;
		cout << hex << pi.dwProcessId << endl;
		char buf[200];
		GetWindowText(hwnd, buf, 200);
		cout << "open exe:" << buf << endl;
	}
	else
	{
		cout << GetLastError() << endl;
	}
}

/*
��ȡ��������Ͻ��������Ļ������
x ������
y ������
*/
void getPos(HWND h, int* x, int* y)
{
	int x1, x2, y1, y2;

	// ��ȡ��Ļ�������
	POINT pt;
	GetCursorPos(&pt);
	x1 = pt.x;
	y1 = pt.y;
	ScreenToClient(h, &pt);
	x2 = pt.x;
	y2 = pt.y;

	//��ô������Ͻ�����
	*x = x1 - x2;
	*y = y1 - y2;
}

/*
��ȡ�õ����㴰�ڵ�����
title ���ڵ�����
*/
void getText(HWND h, char* title)
{

	// ��ȡ���ڱ���
	char text[200];
	GetWindowText(h, text, 200);
	strcpy(title, text);

}

/*
����Ѹ�ף�������������
urlNum ��ȡ���ĵڼ���URL
url URL

return �����ɹ�����1�����󷵻�0
*/
int urlStart(int urlNum, char* url)
{
	HINSTANCE hInstance;
	HWND newHwnd = NULL;
	int x, y;

	if (!IsWindowVisible(hwnd)) //�жϴ����Ƿ���ʾ
	{
		ShowWindow(hwnd, SW_SHOW);//�ô�����ʾ
	}

	char title1[200] = { 0 };
	getText(hwnd, title1);//��ȡ�����ڱ���
	if (1) {


		SetForegroundWindow(hwnd);
		PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//����½�����
		PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
		int tmp = 0;
		do
		{
			tmp++;
			newHwnd = FindWindow(NULL, "�½�����");//�õ��½����صľ��
			if (tmp > 10000)
			{
				cout << "*********�½��������**********" << endl;
				return 0;
			}
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));

		char titleTmp[200];
		GetWindowText(newHwnd, titleTmp, 200);
		cout << "�򿪵Ĵ���:" << tmp << endl;

		int cont = 0;
		while (!IsWindowVisible(newHwnd))
		{
			cont++;
			if (cont > 1000)
			{
				cout << "*************�½����ڴ���***********" << endl;
				return 0;
			}
		}
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(url);//����URL
		Sleep(1000);
		getPos(newHwnd, &x, &y);//����½��ļ����ڵ�����
		SetCursorPos(x + 100, y + 230);//�ƶ����������
		Sleep(1000);

		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//�������
		Sleep(1000);
		HWND hwnd_Megnet;
		HWND hwnd_NewBT = NULL;
		hwnd_Megnet = FindWindow(NULL, "����������");//�õ������������ľ��
		SetForegroundWindow(hwnd_Megnet);
		int i;
		for (i = 0; hwnd_NewBT == NULL&&i < 500; i++) {
			hwnd_NewBT = FindWindow(NULL, "�½�BT����");

			Sleep(1000);
		}
		if (i == 1000) {
			cout << "��������ʧ��" << endl;


		}
		else {
			SetForegroundWindow(hwnd_NewBT);
			//BringWindowToTop(hwnd_NewBT);
			//��õ�ǰϵͳ��ʱ�䣬���������ļ���
			SYSTEMTIME systim;
			GetLocalTime(&systim);
			int sptk = 0;
			char savepath[126] = { 0 };

			sptk += sprintf(savepath, "%d_", urlNum);
			sptk += sprintf(savepath + sptk, "%4d-", systim.wYear);
			sptk += sprintf(savepath + sptk, "%02d-", systim.wMonth);
			sptk += sprintf(savepath + sptk, "%02d-", systim.wDay);
			sptk += sprintf(savepath + sptk, "%02d-", systim.wHour);
			sptk += sprintf(savepath + sptk, "%02d-", systim.wMinute);//��ȡʱ��
			sptk += sprintf(savepath + sptk, "%02d_", systim.wSecond);
			sptk += sprintf(savepath + sptk, "%03d", systim.wMilliseconds);
			cout << "this is important" << savepath << endl;

			char path[255];
			strcpy(path, config.savePath);//��ñ�����ļ���ַ

			strcat(path, "\\");
			strcat(path, savepath);//�洢�ĵ�ַ
								   

			getPos(hwnd_NewBT, &x, &y);//����½��ļ����ڵ�����
			SetCursorPos(x + 150, y + 500);//�ƶ�������洢��ַ

			Sleep(1000);
			//SetForegroundWindow(newHwnd);

			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//����洢��ַ�༭��

																			   //Sleep(500);

			keybd_event(17, 0, 0, 0);//��Ĭ��·��ȫѡ
			keybd_event(65, 0, 0, 0);

			keybd_event(65, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(17, 0, KEYEVENTF_KEYUP, 0);//ɾ��Ĭ�ϴ洢·��

			keybd_event(17, 0, 0, 0);
			keybd_event(88, 0, 0, 0);

			keybd_event(88, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(17, 0, KEYEVENTF_KEYUP, 0);

			SetForegroundWindow(hwnd_NewBT);
			keyboardInput(path);//����洢·��


			SetCursorPos(x + 200, y + 530);
			//SetForegroundWindow(hwnd_NewBT);

			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//�������
			Sleep(1000);

			char title2[200] = { 0 };
			HWND h = GetForegroundWindow();
			getText(h, title2);//��ȡ��ǰ���ڵı���
			cout << title1 << "-----" << title2 << endl;

			if (IsWindowVisible(hwnd_NewBT) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
			{
				//���ӵ�ַ����ȷ
				keybd_event(27, 0, 0, 0);//����esc���˳��½�����
				keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
				files file;//��������ļ�
				strcpy(file.dir, savepath);
				strcpy(file.file_state, "3");
				insert_file(file);
				printf("download failed\n");
				return 0;
			}
			files file;//�������ص��ļ�
			sprintf(file.dir, "%s", savepath);
			strcpy(file.file_state, "1");
			insert_file(file);

		}


	}
	else
	{

		SetForegroundWindow(hwnd);
		PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//����½�����
		PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
		int tmp = 0;
		do
		{
			tmp++;
			newHwnd = FindWindow(NULL, "�½�����");//�õ��½����صľ��
			if (tmp > 10000)
			{
				cout << "*********�½��������**********" << endl;
				return 0;
			}
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));

		char titleTmp[200];
		GetWindowText(newHwnd, titleTmp, 200);
		cout << "�򿪵Ĵ���:" << tmp << endl;

		int cont = 0;
		while (!IsWindowVisible(newHwnd))
		{
			cont++;
			if (cont > 1000)
			{
				cout << "*************�½����ڴ���***********" << endl;
				return 0;
			}
		}
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(url);//����URL
		Sleep(1000);

		//��õ�ǰϵͳ��ʱ�䣬���������ļ���
		SYSTEMTIME systim;
		GetLocalTime(&systim);
		int sptk = 0;
		char savepath[126] = { 0 };

		sptk += sprintf(savepath, "%d_", urlNum);
		sptk += sprintf(savepath + sptk, "%4d-", systim.wYear);
		sptk += sprintf(savepath + sptk, "%02d-", systim.wMonth);
		sptk += sprintf(savepath + sptk, "%02d-", systim.wDay);
		sptk += sprintf(savepath + sptk, "%02d-", systim.wHour);
		sptk += sprintf(savepath + sptk, "%02d-", systim.wMinute);//��ȡʱ��
		sptk += sprintf(savepath + sptk, "%02d_", systim.wSecond);
		sptk += sprintf(savepath + sptk, "%03d", systim.wMilliseconds);
		cout << "this is important" << savepath << endl;

		char path[255];
		strcpy(path, config.savePath);//��ñ�����ļ���ַ

		strcat(path, "\\");
		strcat(path, savepath);//�洢�ĵ�ַ
		//keyboardInput(path);


		getPos(newHwnd, &x, &y);//����½��ļ����ڵ�����
		SetCursorPos(x + 200, y + 250);//�ƶ�������洢��ַ
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//����洢��ַ�༭��

		//Sleep(500);

		keybd_event(17, 0, 0, 0);//��Ĭ��·��ȫѡ
		keybd_event(65, 0, 0, 0);

		keybd_event(65, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(17, 0, KEYEVENTF_KEYUP, 0);//ɾ��Ĭ�ϴ洢·��

		keybd_event(17, 0, 0, 0);
		keybd_event(88, 0, 0, 0);

		keybd_event(88, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(17, 0, KEYEVENTF_KEYUP, 0);

		SetForegroundWindow(newHwnd);
		keyboardInput(path);//����洢·��


		SetCursorPos(x + 200, y + 300);
		SetForegroundWindow(newHwnd);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//�������
		Sleep(1000);

		char title2[200] = { 0 };
		HWND h = GetForegroundWindow();
		getText(h, title2);//��ȡ��ǰ���ڵı���
		cout << title1 << "-----" << title2 << endl;

		if (IsWindowVisible(newHwnd) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
		{
			//���ӵ�ַ����ȷ
			keybd_event(27, 0, 0, 0);//����esc���˳��½�����
			keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
			files file;//��������ļ�
			strcpy(file.dir, savepath);
			strcpy(file.file_state, "3");
			insert_file(file);
			printf("download failed\n");
			return 0;
		}
		files file;//�������ص��ļ�
		sprintf(file.dir, "%s", savepath);
		strcpy(file.file_state, "1");
		insert_file(file);

		return 1;

	}
}

int urlStart(int urlNum, url_info & info)
{
	HINSTANCE hInstance;
	HWND newHwnd = NULL;
	int x, y;

	if (!IsWindowVisible(hwnd)) //�жϴ����Ƿ���ʾ
	{
		ShowWindow(hwnd, SW_SHOW);//�ô�����ʾ
	}

	char title1[200] = { 0 };
	getText(hwnd, title1);//��ȡ�����ڱ���
	if (info.protocol == magnet)
	{
		SetForegroundWindow(hwnd);
		PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//����½�����
		PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
		int tmp = 0;
		do
		{
			tmp++;
			newHwnd = FindWindow(NULL, "�½�����");//�õ��½����صľ��
			if (tmp > 10000)
			{
				cout << "*********�½��������**********" << endl;
				return 0;
			}
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));

		char titleTmp[200];
		GetWindowText(newHwnd, titleTmp, 200);
		cout << "�򿪵Ĵ���:" << tmp << endl;

		int cont = 0;
		while (!IsWindowVisible(newHwnd))
		{
			cont++;
			if (cont > 1000)
			{
				cout << "*************�½����ڴ���***********" << endl;
				return 0;
			}
		}
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(info.url);//����URL
		Sleep(1000);
		getPos(newHwnd, &x, &y);//����½��ļ����ڵ�����
		SetCursorPos(x + 100, y + 230);//�ƶ����������
		Sleep(1000);

		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//�������
		Sleep(1000);
		HWND hwnd_Megnet;
		HWND hwnd_NewBT = NULL;
		hwnd_Megnet = FindWindow(NULL, "����������");//�õ������������ľ��
		SetForegroundWindow(hwnd_Megnet);
		int i;
		for (i = 0; hwnd_NewBT == NULL&&i < 500; i++) {
			hwnd_NewBT = FindWindow(NULL, "�½�BT����");

			Sleep(1000);
		}
		//��Դ��������
		if (i == 1000) {
			info.state = DL_FAIL;//�ļ�����ʧ��
			strcpy(info.remark, "Parse error");//���ӽ�������
			cout << "��������ʧ��" << endl;
			return 0;
		}
		else {
			SetForegroundWindow(hwnd_NewBT);
			//��õ�ǰϵͳ��ʱ�䣬���������ļ���
			SYSTEMTIME systim;
			GetLocalTime(&systim);
			int sptk = 0;
			char savepath[126] = { 0 };

			sptk += sprintf(savepath + sptk, "%4d-", systim.wYear);
			sptk += sprintf(savepath + sptk, "%02d-", systim.wMonth);
			sptk += sprintf(savepath + sptk, "%02d_", systim.wDay);
			sptk += sprintf(savepath + sptk, "%02d_", systim.wHour);
			sptk += sprintf(savepath + sptk, "%02d_", systim.wMinute);//��ȡʱ��
			sptk += sprintf(savepath + sptk, "%02d_", systim.wSecond);
			sptk += sprintf(savepath + sptk, "%03d", systim.wMilliseconds);
			cout << "this is important" << savepath << endl;

			char path[255];
			strcpy(path, config.savePath);//��ñ�����ļ���ַ

			strcat(path, "/");
			strcat(path, savepath);//�洢�ĵ�ַ
			strtok(savepath, "_");
			strcpy(info.start_time, savepath);//������
			strcat(info.start_time, " ");
			strcat(info.start_time, strtok(NULL, "_"));//ʱ
			strcat(info.start_time, ":");
			strcat(info.start_time, strtok(NULL, "_"));//��
			strcat(info.start_time, ":");
			strcat(info.start_time, strtok(NULL, "_"));//��


			getPos(hwnd_NewBT, &x, &y);//����½��ļ����ڵ�����
			Sleep(500);
			SetCursorPos(x + 150, y + 500);//�ƶ�������洢��ַ
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//����洢��ַ�༭��
			Sleep(1000);

			keybd_event(17, 0, 0, 0);//��Ĭ��·��ȫѡ
			keybd_event(65, 0, 0, 0);

			keybd_event(65, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(17, 0, KEYEVENTF_KEYUP, 0);//ɾ��Ĭ�ϴ洢·��

			keybd_event(17, 0, 0, 0);
			keybd_event(88, 0, 0, 0);

			keybd_event(88, 0, KEYEVENTF_KEYUP, 0);
			keybd_event(17, 0, KEYEVENTF_KEYUP, 0);
			Sleep(1000);
			SetForegroundWindow(hwnd_NewBT);
			keyboardInput(path);//����洢·��
			Sleep(1000);


			SetCursorPos(x + 200, y + 530);

			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//�������
			Sleep(1000);

			char title2[200] = { 0 };
			HWND h = GetForegroundWindow();
			getText(h, title2);//��ȡ��ǰ���ڵı���
			cout << title1 << "-----" << title2 << endl;

			if (IsWindowVisible(hwnd_NewBT) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
			{
				//���ӵ�ַ����ȷ
				keybd_event(27, 0, 0, 0);//����esc���˳��½�����
				keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
				info.state = DL_FAIL;//�ļ�����ʧ��
				strcpy(info.remark, "Link error!");//���Ӵ���
				printf("download failed\n");
				return 0;
			}
			info.state = DL_RUN;//״̬��������
			strcpy(info.filepath, path);
			strcpy(info.remark, "Run!");
			return 1;

		}
	}
	else
	{


		SetForegroundWindow(hwnd);
		PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(36, 73));//����½�����
		PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(36, 73));
		int tmp = 0;
		//�õ��½����صľ��
		do
		{
			tmp++;
			newHwnd = FindWindow(NULL, "�½�����");
			if (tmp > 10000)
			{
				cout << "*********�½��������**********" << endl;
				return 0;
			}
		} while (newHwnd == NULL && !IsWindowVisible(newHwnd));

		char titleTmp[200];
		GetWindowText(newHwnd, titleTmp, 200);
		cout << "�򿪵Ĵ���:" << tmp << endl;

		int cont = 0;
		//�ж��½������Ƿ��
		while (!IsWindowVisible(newHwnd))
		{
			cont++;
			if (cont > 1000)
			{
				cout << "*************�½����ڴ���***********" << endl;
				return 0;
			}
		}
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(info.url);//����URL
		Sleep(1000);

		//��õ�ǰϵͳ��ʱ�䣬���������ļ���
		SYSTEMTIME systim;
		GetLocalTime(&systim);
		int sptk = 0;
		char savepath[126] = { 0 };

		/*sptk += sprintf(savepath, "%d_", urlNum);*/
		sptk += sprintf(savepath + sptk, "%4d-", systim.wYear);
		sptk += sprintf(savepath + sptk, "%02d-", systim.wMonth);
		sptk += sprintf(savepath + sptk, "%02d_", systim.wDay);
		sptk += sprintf(savepath + sptk, "%02d_", systim.wHour);
		sptk += sprintf(savepath + sptk, "%02d_", systim.wMinute);//��ȡʱ��
		sptk += sprintf(savepath + sptk, "%02d_", systim.wSecond);
		sptk += sprintf(savepath + sptk, "%03d", systim.wMilliseconds);
		cout << "this is important" << savepath << endl;

		char path[255];
		strcpy(path, config.savePath);//��ñ�����ļ���ַ

		strcat(path, "/");
		strcat(path, savepath);//�洢�ĵ�ַ
		strtok(savepath, "_");
		strcpy(info.start_time, savepath);//������
		strcat(info.start_time, " ");
		strcat(info.start_time, strtok(NULL, "_"));//ʱ
		strcat(info.start_time, ":");
		strcat(info.start_time, strtok(NULL, "_"));//��
		strcat(info.start_time, ":");
		strcat(info.start_time, strtok(NULL, "_"));//��

		getPos(newHwnd, &x, &y);//����½��ļ����ڵ�����
		SetCursorPos(x + 200, y + 250);//�ƶ�������洢��ַ
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//����洢��ַ�༭��
		Sleep(1000);
																		   //Sleep(500);
		keybd_event(17, 0, 0, 0);//��Ĭ��·��ȫѡ
		keybd_event(65, 0, 0, 0);

		keybd_event(65, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(17, 0, KEYEVENTF_KEYUP, 0);//ɾ��Ĭ�ϴ洢·��

		keybd_event(17, 0, 0, 0);
		keybd_event(88, 0, 0, 0);

		keybd_event(88, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(17, 0, KEYEVENTF_KEYUP, 0);
		Sleep(1000);
		SetForegroundWindow(newHwnd);
		keyboardInput(path);//����洢·��
		Sleep(1000);

		SetCursorPos(x + 200, y + 300);
		SetForegroundWindow(newHwnd);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);//�������
		Sleep(1000);

		char title2[200] = { 0 };
		HWND h = GetForegroundWindow();
		getText(h, title2);//��ȡ��ǰ���ڵı���
		cout << title1 << "-----" << title2 << endl;

		if (IsWindowVisible(newHwnd) || IsWindowVisible(FindWindow("XLUEModalHostWnd", "MessageBox")))
		{
			//���ӵ�ַ����ȷ
			keybd_event(27, 0, 0, 0);//����esc���˳��½�����
			keybd_event(27, 0, KEYEVENTF_KEYUP, 0);
			info.state = DL_FAIL;//�ļ�����ʧ��
			strcpy(info.remark, "Link error!");
			printf("download failed\n");
			return 0;
		}
		info.state = DL_RUN;//״̬��������
		strcpy(info.filepath, path);
		strcpy(info.remark, "Run!");

		return 1;
	}
}


/*
ģ���������
value ���������
*/
//void keyboardInput(char* value)
//{
//	int i = 0;
//	int keyValue = 0;
//	while (value[i] != '\0')
//	{
//		if (value[i] >= 97 && value[i] <= 122)
//		{
//			keyValue = value[i] - 32;
//			keybd_event(keyValue, (BYTE)0, 0, 0);
//
//			keybd_event(keyValue, (BYTE)0, KEYEVENTF_KEYUP, 0);
//
//		}
//		else if (value[i] >= 65 && value[i] <= 90)
//		{
//
//			keyValue = value[i];
//			keybd_event(16, 0, 0, 0);
//			keybd_event(keyValue, (BYTE)0, 0, 0);
//
//			keybd_event(keyValue, (BYTE)0, KEYEVENTF_KEYUP, 0);
//			keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//
//		}
//		else
//		{
//
//			switch (value[i])
//			{
//			case ';':
//
//				keybd_event(186, 0, 0, 0);
//				keybd_event(186, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//			case ':':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(186, 0, 0, 0);
//				keybd_event(186, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '/':
//				keybd_event(191, 0, 0, 0);
//				keybd_event(191, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//			case '?':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(191, 0, 0, 0);
//				keybd_event(191, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '.':
//
//				keybd_event(190, 0, 0, 0);
//				keybd_event(190, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//			case '>':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(190, 0, 0, 0);
//				keybd_event(190, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case ',':
//
//				keybd_event(188, 0, 0, 0);
//				keybd_event(188, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '<':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(188, 0, 0, 0);
//				keybd_event(188, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '\'':
//
//				keybd_event(222, 0, 0, 0);
//				keybd_event(222, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '"':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(222, 0, 0, 0);
//				keybd_event(222, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '`':
//
//				keybd_event(222, 0, 0, 0);
//				keybd_event(222, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '~':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(192, 0, 0, 0);
//				keybd_event(192, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//			case '-':
//
//				keybd_event(189, 0, 0, 0);
//				keybd_event(189, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '_':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(189, 0, 0, 0);
//				keybd_event(189, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//
//			case '[':
//
//				keybd_event(219, 0, 0, 0);
//				keybd_event(219, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '{':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(219, 0, 0, 0);
//				keybd_event(219, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case ']':
//
//				keybd_event(221, 0, 0, 0);
//				keybd_event(221, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '}':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(221, 0, 0, 0);
//				keybd_event(221, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//
//			case '\\':
//
//				keybd_event(220, 0, 0, 0);
//				keybd_event(220, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '|':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(220, 0, 0, 0);
//				keybd_event(220, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '=':
//
//				keybd_event(220, 0, 0, 0);
//				keybd_event(220, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '+':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(187, 0, 0, 0);
//				keybd_event(187, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//			case '0':
//
//				keybd_event(48, 0, 0, 0);
//				keybd_event(48, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case ')':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(48, 0, 0, 0);
//				keybd_event(48, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '1':
//
//				keybd_event(49, 0, 0, 0);
//				keybd_event(49, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '!':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(49, 0, 0, 0);
//				keybd_event(49, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '2':
//
//				keybd_event(50, 0, 0, 0);
//				keybd_event(50, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '@':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(50, 0, 0, 0);
//				keybd_event(50, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '3':
//
//				keybd_event(51, 0, 0, 0);
//				keybd_event(51, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '#':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(51, 0, 0, 0);
//				keybd_event(51, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//
//			case '4':
//
//				keybd_event(52, 0, 0, 0);
//				keybd_event(52, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '$':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(52, 0, 0, 0);
//				keybd_event(52, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '5':
//
//				keybd_event(53, 0, 0, 0);
//				keybd_event(53, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '%':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(53, 0, 0, 0);
//				keybd_event(53, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '6':
//
//				keybd_event(54, 0, 0, 0);
//				keybd_event(54, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '^':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(54, 0, 0, 0);
//				keybd_event(54, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '7':
//
//				keybd_event(55, 0, 0, 0);
//				keybd_event(55, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '&':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(55, 0, 0, 0);
//				keybd_event(55, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//			case '8':
//
//				keybd_event(56, 0, 0, 0);
//				keybd_event(56, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '*':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(56, 0, 0, 0);
//				keybd_event(56, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//			case '9':
//
//				keybd_event(57, 0, 0, 0);
//				keybd_event(57, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//
//
//			case '(':
//				keybd_event(16, 0, 0, 0);
//				keybd_event(57, 0, 0, 0);
//				keybd_event(57, 0, KEYEVENTF_KEYUP, 0);
//				keybd_event(16, 0, KEYEVENTF_KEYUP, 0);
//				break;
//
//
//
//			case '\n':
//
//				keybd_event(13, 0, 0, 0);
//				keybd_event(13, 0, KEYEVENTF_KEYUP, 0);
//
//				break;
//			}
//		}
//		i++;
//	}
//}

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