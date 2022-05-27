#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
int main()
{
	setlocale(LC_ALL, "ru");
	cout << "Процесс Server." << endl << endl;
	SECURITY_ATTRIBUTES sa; 
	SECURITY_DESCRIPTOR sd; 
	HANDLE hNamedPipe;
	char lpszInMessage[80]; 
	DWORD dwBytesRead; 
	char lpszOutMessage[80]; 
	DWORD dwBytesWrite;
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;
	hNamedPipe = CreateNamedPipe("\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named pipe failed." << endl<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		_getch();
		return 0;
	}
	cout << "Сервер ожидает подключения клиента." << endl;
	if (!ConnectNamedPipe(hNamedPipe,(LPOVERLAPPED)NULL))
	{
		cerr << "The connection failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		_getch();
		return 0;
	}
	cout << "Введите размер массива: " << endl;
	int n;
	cin >> n;
	wsprintf(lpszOutMessage, "%d", n);
	if (!WriteFile(hNamedPipe, lpszOutMessage,sizeof(lpszOutMessage),&dwBytesWrite,(LPOVERLAPPED)NULL ))
	{
		cerr << "Writing to the named pipe failed: " << endl << "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		_getch();
		CloseHandle(hNamedPipe);
		return 0;
	}
	if (!ReadFile(hNamedPipe, lpszInMessage, sizeof(lpszInMessage), &dwBytesRead, (LPOVERLAPPED)NULL ))
	{
		cerr << "Reading to the named pipe failed: " << endl << "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		_getch();
		CloseHandle(hNamedPipe);
		return 0;
	}
	int count = atoi(lpszInMessage);
	cout << "Полученный от процесса Hignt массив: " << endl;
	for (int i = 0; i < count; i++) {
		if (!ReadFile(hNamedPipe,lpszInMessage, sizeof(lpszInMessage),&dwBytesRead, (LPOVERLAPPED)NULL ))
		{
			cerr << "Reading to the named pipe failed: " << endl << "The last error code: " << GetLastError() << endl;
			cout << "Press any char to finish the client: ";
			_getch();
			CloseHandle(hNamedPipe);
			return 0;
		}
		cout << lpszInMessage << endl;
	}
	
	CloseHandle(hNamedPipe);
	
	cout << "Для завершения нажмите любую клавишу.";
	return 0;
}
