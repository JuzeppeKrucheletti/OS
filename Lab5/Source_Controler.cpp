#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
	cout << "Процесс Controler." << endl << endl;
	HANDLE hWritePipe, hReadPipe;
	HANDLE E;
	char lpszEnableRead[] = "E";
	E = CreateEvent(NULL, TRUE, FALSE, lpszEnableRead);
	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);

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
	cout << "Полученная от процесса Server размерность массива: " << n << endl;

	WaitForSingleObject(E, INFINITE);
	cout << "Полученная от процесса Hignt размерность массива: " << endl;
	int count;
	
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
	cout << count << endl;
	cout << "Полученный от процесса Hignt массив: " << endl;
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
	


	

	cout << "Для завершения работы процесса нажмите любую клавишу." << endl;
	_getch();

	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(E);
	return 0;
}
