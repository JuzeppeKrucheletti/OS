// ������ ����������� ��������, ������� ������� ������
// ���������� ���������� � ����� �������� � ���� ����������
// ������ ����� ����������.

#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;
HANDLE C, D, endReader;
HANDLE hSemaphore;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");

	C = CreateEvent(NULL, TRUE, FALSE, "C");
	if (C == NULL)
		return GetLastError();
	D = CreateEvent(NULL, TRUE, FALSE, "D");
	if (D == NULL)
		return GetLastError();
	endReader = CreateEvent(NULL, TRUE, FALSE, "endR");
	if (endReader == NULL)
		return GetLastError();
	hSemaphore = CreateSemaphore(NULL, 2, 2, "SemaphoreR");
	if (hSemaphore == NULL)
	{
		cout << "������� �� ������" << endl;
		cin.get();

		return GetLastError();
	}


	int n = atoi(argv[0]);
	int m_count = atoi(argv[1]);
	cout << "Reader #" << n << endl;
	WaitForSingleObject(hSemaphore, INFINITE);
	HANDLE mass[] = { C, D };
	for (int i = 0; i < 2*m_count; i++)
	{
		int ind = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (ind == 0)
		{
			cout << "�������� ��������� C �� Administrator" << endl;
			ResetEvent(C);
		}
		else if (ind == 1)
		{
			cout << "�������� ���������  D �� Administrator" << endl;
			ResetEvent(D);
		}
	}
	cout << "���������� ������..." << endl;
	ReleaseSemaphore(hSemaphore, 1, NULL);
	CloseHandle(C);
	CloseHandle(D);
	WaitForSingleObject(endReader, INFINITE);
	return 0;
}
