// ������ ����������� ��������, ������� ������� ������
// ���������� ���������� � ����� �������� � ���� ����������
// ������ ����� ����������.
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");
	char lpszAppName[] = "C:\\Users\\ewgen\\source\\repos\\Project2\\x64\\Debug\\Child.exe";
	char* ch = new char[1];
	char* lpCommLine = new char[1];
	lpCommLine = strcpy(lpCommLine, "");
	cout << "������� ������ �������: " << endl;
	int n;
	cin >> n;
	ch = _itoa(n, ch, 10);
	lpCommLine = strcat(lpCommLine, ch);
	lpCommLine = strcat(lpCommLine, " ");
	int el;
	cout << "������� �������� �������: " << endl;
	for (int i = 0; i < n; i++) {
		cin >> el;
		ch = _itoa(el, ch, 10);
		lpCommLine = strcat(lpCommLine, ch);
		lpCommLine = strcat(lpCommLine, " ");
	}
	cout << "������� �. ��� �������� ������� >A ����� ��������� � ������� �����, ��������� - ��������� ������:" << endl;
	int A;
	cin >> A;
	ch = _itoa(A, ch, 10);
	lpCommLine = strcat(lpCommLine, ch);
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwX = 1000;
	si.dwFlags = STARTF_USEPOSITION;
	
	PROCESS_INFORMATION piApp;
	
	if (!CreateProcess(lpszAppName, lpCommLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		_cputs("����� ������� �� ������.\n");
		_getch();
		return 0;
	}

	_cputs("������ ����� �������.\n");
	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

	return 0;
}