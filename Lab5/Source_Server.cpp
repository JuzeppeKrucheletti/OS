#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main()
{
	setlocale(LC_ALL, "ru");
	cout << "Процесс Server." << endl << endl;
	char lpszComLine[80]; 
	HANDLE E; 
	char lpszEnableRead[] = "E";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe;
	SECURITY_ATTRIBUTES sa;
	E = CreateEvent(NULL, TRUE, FALSE, lpszEnableRead);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL; 
	sa.bInheritHandle = TRUE; 
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	HANDLE hWritePipe_Contr, hReadPipe_Contr;
	if (!CreatePipe(&hReadPipe_Contr, &hWritePipe_Contr, &sa, 0))
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	wsprintf(lpszComLine, "Hignt.exe %d %d %d %d",
		(int)hWritePipe, (int)hReadPipe, (int)hWritePipe_Contr, (int)hReadPipe_Contr);
	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si,&pi 
	))
	{
		_cputs("Create process failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	wsprintf(lpszComLine, "Controler.exe %d %d",
		(int)hWritePipe_Contr, (int)hReadPipe_Contr);
	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi
	))
	{
		_cputs("Create process failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	cout << "Введите размер массива: " << endl;
	__int16 n;
	cin >> n;

	DWORD dwBytesWritten;
		if (!WriteFile(
			hWritePipe,
			&n,
			sizeof(n),
			&dwBytesWritten,
			NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		WriteFile(
			hWritePipe_Contr,
			&n,
			sizeof(n),
			&dwBytesWritten,
			NULL);
		SetEvent(E);
		ResetEvent(E);
		WaitForSingleObject(E, INFINITE);

		int count;
		DWORD dwBytesRead;
		if (!ReadFile(
			hReadPipe,
			&count,
			sizeof(count),
			&dwBytesRead,
			NULL))
		{
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		cout << "Полученные от процесса Hignt элементы массива:" << endl;
		for (int i = 0; i < count; i++)
			{
				int el;
				
				if (!ReadFile(
					hReadPipe,
					&el,
					sizeof(el),
					&dwBytesRead,
					NULL))
				{
					_cputs("Read from the pipe failed.\n");
					_cputs("Press any key to finish.\n");
					_getch();
					return GetLastError();
				}
				cout << el << endl;
			}


	
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(E);
	cout << "Для завершения работы процесса нажмите любую клавишу." << endl;
	_getch();
	return 0;
}
