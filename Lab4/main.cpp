// Пример консольного процесса, который создает другое
// консольное приложение с новой консолью и ждет завершения
// работы этого приложения.
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;



HANDLE A, B, C, D, endWriter, endReader;
HANDLE hMutex1, hMutex2;
HANDLE hSemaphore;
int main()
{
	setlocale(LC_ALL, "ru");
	//
	A = CreateEvent(NULL, TRUE, FALSE, "A");
	if (A == NULL)
		return GetLastError();
	B = CreateEvent(NULL, TRUE, FALSE, "B");
	if (B == NULL)
		return GetLastError();
	C = CreateEvent(NULL, TRUE, FALSE, "C");
	if (C == NULL)
		return GetLastError();
	D = CreateEvent(NULL, TRUE, FALSE, "D");
	if (D == NULL)
		return GetLastError();
	endWriter = CreateEvent(NULL, TRUE, FALSE, "endW");
	if (endWriter == NULL)
		return GetLastError();
	endReader = CreateEvent(NULL, TRUE, FALSE, "endR");
	if (endReader == NULL)
		return GetLastError();
	hMutex1 = CreateMutex(NULL, FALSE, "MutexW1");
	if (hMutex1 == NULL)
	{
		cout << "Мьютекс не создан" << endl;
		cin.get();

		return GetLastError();
	}
	hMutex2 = CreateMutex(NULL, FALSE, "MutexW2");
	if (hMutex2 == NULL)
	{
		cout << "Мьютекс не создан" << endl;
		cin.get();

		return GetLastError();
	}
	hSemaphore = CreateSemaphore(NULL, 2, 2, "SemaphoreR");
	if (hSemaphore == NULL)
	{
		cout << "Семафор не создан" << endl;
		cin.get();

		return GetLastError();
	}
	//

	char lpszAppNameR[] = "Reader.exe";
	char lpszAppNameW[] = "Writer.exe";
	int n, m_count;
	cout << "Введите количество процессов Writer и Reader:" << endl;
	cin >> n;
	cout << "Введите количество cообщений, принятых Writer и прочитанных Reader:" << endl;
	cin >> m_count;
	/*cout << "Введите размер массива: " << endl;
	int n;
	cin >> n;
	ch = _itoa(n, ch, 10);
	lpCommLine = strcat(lpCommLine, ch);
	lpCommLine = strcat(lpCommLine, " ");
	int el;
	cout << "Введите элементы массива: " << endl;
	for (int i = 0; i < n; i++) {
		cin >> el;
		ch = _itoa(el, ch, 10);
		lpCommLine = strcat(lpCommLine, ch);
		lpCommLine = strcat(lpCommLine, " ");
	}
	cout << "Введите А. Все элементы массива >A будут размещены в массиве слева, остальные - заполнены нулями:" << endl;
	int A;
	cin >> A;
	ch = _itoa(A, ch, 10);
	lpCommLine = strcat(lpCommLine, ch);*/
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	HANDLE* H = new HANDLE[2*n];
	
	PROCESS_INFORMATION piApp;
	for (int i = 0, j = n; i < n; i++, j++) {
		char* ch = new char[1];
		char* lpCommLine = new char[1];
		lpCommLine = strcpy(lpCommLine, "");
		ch = _itoa((i+1), ch, 10);
		lpCommLine = strcat(lpCommLine, ch);
		ch = _itoa(m_count, ch, 10);
		lpCommLine = strcat(lpCommLine," ");
		lpCommLine = strcat(lpCommLine, ch);
		if (!CreateProcess(lpszAppNameW, lpCommLine, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
		{
			_cputs("Новый процесс не создан.\n");
			_getch();
			return 0;
		}
		
		H[i] = piApp.hProcess;
		if (!CreateProcess(lpszAppNameR, lpCommLine, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
		{
			_cputs("Новый процесс не создан.\n");
			_getch();
			return 0;
		}
		
		H[j] = piApp.hProcess;
	}
	HANDLE mass[] = { A, B };
	for (int i = 0; i < n*m_count; i++)
	{
		int ind = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (ind == 0)
		{
			cout << "Получено сообщение А от Writer"<<endl;
			ResetEvent(A);
			cout << "Отправлено сообщение С для Reader" << endl;
			SetEvent(C);
		}
		if (ind == 1)
		{
			cout << "Получено сообщение B от Writer"<<endl;
			ResetEvent(B);
			cout << "Отправлено сообщение D для Reader" << endl;
			SetEvent(D);
		}
	}
	//WaitForMultipleObjects(2 * n, H, true, INFINITE);
	cout<<endl << "Все процессы завершили работу" << endl;



	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

	SetEvent(endWriter);
	SetEvent(endReader);

	CloseHandle(A);
	CloseHandle(B);
	CloseHandle(C);
	CloseHandle(D);
	for (int i = 0; i < 2*n; i++)
	{
		CloseHandle(H[i]);
	}

	return 0;
}