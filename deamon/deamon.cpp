// deamon.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>


DWORD WINAPI ThreadCheckProc(LPVOID lParam)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi = { 0 };
	HANDLE hMutex;

	while (true)
	{
		hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE,
			L"PcManagerClient.exe"
		);
		if (!hMutex)
		{
			CreateProcess(L"D:\\SourceCode\\vs\\PcManagerClient\\Debug\\PcManagerClient.exe",
				NULL, NULL, NULL, FALSE,
				CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else
		{
			CloseHandle(hMutex);
		}
		Sleep(500);
	}
}

int main()
{
	//CreateEvent
	DWORD threadId;
	CreateMutex(NULL, TRUE, 
		L"deamon.exe");
	CreateThread(NULL, 0, ThreadCheckProc,
		NULL,
		0, &threadId
	);
	printf("Protected\n");
	while (1)
	{
		Sleep(500);
	}

	return 0;
}