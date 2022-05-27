// Пример консольного процесса, который создает другое
// консольное приложение с новой консолью и ждет завершения
// работы этого приложения.

#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE A, B, endWriter;
HANDLE hMutex1, hMutex2;



int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");


	A = CreateEvent(NULL, TRUE, FALSE, "A");
	if (A == NULL)
		return GetLastError();
	B = CreateEvent(NULL, TRUE, FALSE, "B");
	if (B == NULL)
		return GetLastError();
	endWriter = CreateEvent(NULL, TRUE, FALSE, "endW");
	if (endWriter == NULL)
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

	int n = atoi(argv[0]);
	cout << "Writer #" << n << endl;
	int m_count = atoi(argv[1]);
	char mess;
	HANDLE mass[] = { hMutex1, hMutex2 };
	int mutex_num = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		
	cout << "Введите сообщения A или В (всего " << m_count << " сообщений):" << endl;
	for (int i = 0; i < m_count; i++) {
		cin >> mess;
		if (mess == 'A') SetEvent(A);
		else if (mess == 'B') SetEvent(B);
		else i--;
	}
	cout << "Завершение работы..." << endl;
	if(mutex_num==0) ReleaseMutex(hMutex1);
	else ReleaseMutex(hMutex2);
	CloseHandle(A);
	CloseHandle(B);
	WaitForSingleObject(endWriter, INFINITE);
	return 0;
}
