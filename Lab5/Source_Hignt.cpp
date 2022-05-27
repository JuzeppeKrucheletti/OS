#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
	cout << "Процесс Hignt." << endl << endl;
	HANDLE hWritePipe, hReadPipe;
	HANDLE E; 
	char lpszEnableRead[] = "E";
	E = CreateEvent(NULL, TRUE, FALSE, lpszEnableRead);
	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);
	HANDLE hWritePipe_Contr, hReadPipe_Contr;
	hWritePipe_Contr = (HANDLE)atoi(argv[3]);
	hReadPipe_Contr = (HANDLE)atoi(argv[4]);
	WaitForSingleObject(E, INFINITE);

	__int16 n;
	DWORD dwBytesRead;
	if (!ReadFile(
		hReadPipe,
		&n,
		sizeof(n),
		&dwBytesRead,
		NULL))
	{
		_cputs("Read from the pipe failed.\n");
		_getch();
		return GetLastError();
	}
	cout << "Полученная размерность массива: " << n << endl;
	int* mass = new int[n];
	cout << "Введите элементы массива: " << endl;
	int a;
	for (int i = 0; i < n; i++) {
		cin >> mass[i];
		
	}
	cout << "Введите число N (элементы >N будут переданы процессу Server): " << endl;
	int N;

	cin >> N;
	cout << "Полученный массив: " << endl;
	int count = 0;
	for (int i = 0; i < n; i++) {
		if (mass[i] > N) {
			count++;
			cout << mass[i] << endl;
		}
	}
	
	
	DWORD dwBytesWritten;
	if (!WriteFile(
		hWritePipe,
		&count,
		sizeof(count),
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
		&count,
		sizeof(count),
		&dwBytesWritten,
		NULL);
	for (int i = 0; i < n; i++)
	{
		
		if (mass[i] > N) {
			if (!WriteFile(
				hWritePipe,
				&mass[i],
				sizeof(mass[i]),
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
				&mass[i],
				sizeof(mass[i]),
				&dwBytesWritten,
				NULL);
		}
	}


	SetEvent(E);

	
	cout << "Для завершения работы процесса нажмите любую клавишу." << endl;
	_getch();
	
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(E);
	return 0;
}
