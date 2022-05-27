#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
int main()
{
	setlocale(LC_ALL, "ru");
	cout << "Процесс Hignt." << endl << endl;
	
	HANDLE hNamedPipe;
	char machineName[80] = "DESKTOP-TJQ4906";
	char pipeName[80];
	char lpszOutMessage[80]; 
	DWORD dwBytesWrite; 
	char lpszInMessage[80]; 
	DWORD dwBytesRead; 
	
	wsprintf(pipeName, "\\\\%s\\pipe\\demo_pipe",
		machineName);
	
	hNamedPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,(HANDLE)NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named pipe failed." << endl << "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		_getch();
		return 0;
	}
	if (!ReadFile(hNamedPipe,lpszInMessage, sizeof(lpszInMessage), &dwBytesRead, (LPOVERLAPPED)NULL))
	{
		cerr << "Data reading from the named pipe failed." << endl << "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		_getch();
		return 0;
	}
	cout << "Полученная размерность массива: " << lpszInMessage << endl;
	int n = atoi(lpszInMessage);
	int* mass = new int[n];
	cout << "Введите элементы массива: " << endl;
	int a;
	for (int i = 0; i < n; i++) {
		cin >> mass[i];

	}
	cout << "Введите число N (элементы >N будут переданы процессу Server): " << endl;
	int N;

	cin >> N;
	cout << "Отправленный процессу Server массив: " << endl;
	int count = 0;
	for (int i = 0; i < n; i++) {
		if (mass[i] > N) {
			count++;
			cout << mass[i] << endl;
		}
	}
	

	wsprintf(lpszOutMessage, "%d", count);
	if (!WriteFile(hNamedPipe,lpszOutMessage, sizeof(lpszOutMessage), &dwBytesWrite, (LPOVERLAPPED)NULL))
	{
		cerr << "Data writing to the named pipe failed." << endl << "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		_getch();
		return 0;
	}
	for (int i = 0; i < n; i++) {
		if (mass[i] > N) {
			wsprintf(lpszOutMessage, "%d", mass[i]);
			if (!WriteFile(hNamedPipe, lpszOutMessage, sizeof(lpszOutMessage), &dwBytesWrite, (LPOVERLAPPED)NULL ))
			{
				cerr << "Data writing to the named pipe failed." << endl << "The last error code: " << GetLastError() << endl;
				CloseHandle(hNamedPipe);
				cout << "Press any char to finish the server: ";
				_getch();
				return 0;
			}
		}
	}
	 
	CloseHandle(hNamedPipe);
	
	cout << "Для завершения нажмите любую клавишу.";
	_getch();
	return 0;
}