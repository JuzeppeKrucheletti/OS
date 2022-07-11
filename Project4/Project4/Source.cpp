// Пример создания потока функцией CreateThread

#include <windows.h>
#include <iostream>
#include <time.h>
#include <process.h>
using namespace std;
UINT WINAPI threadWorker(LPVOID mass_and_size)
{
	int count = 0;
	int elem;
	cout << "Введите элемент, который хотите найти:" << endl;
	cin >> elem;
	int** mass_and_size_1 = (int**)mass_and_size;
	for (int i = 0; i < mass_and_size_1[0][0]; i++)
	{
		if (mass_and_size_1[1][i] == elem)
			count++;
	}

	cout << "На сколько секунд вы хотите приостановить поток?" << endl;
	int sec;
	cin >> sec;
	Sleep(sec * 1000);;
	cout << "Поток Worker возобновил свою работу." << endl;
	cout << "Количество элементов, совпадающих с введенным: " << count << endl;
	return 0;
}
DWORD WINAPI threadMain(LPVOID mass_size)
{
	int* mass;
	mass = new int[int(mass_size)];
	srand(time(0));
	cout << "Созданный массив: " << endl;
	for (int i = 0; i<int(mass_size); i++)
	{
		mass[i] = rand() % 10;
		cout << mass[i] << endl;
	}
	int** mass_and_size = new int* [2];
	mass_and_size[0] = new int[1];
	mass_and_size[0][0] = int(mass_size);
	mass_and_size[1] = new int[mass_and_size[0][0]];
	for (int i = 0; i < mass_and_size[0][0]; i++)
	{
		mass_and_size[1][i] = mass[i];
	}
	HANDLE	hThreadWorker;
	UINT	IDThreadWorker;


	hThreadWorker = (HANDLE)_beginthreadex(NULL, 0, threadWorker, (void*)mass_and_size, 0, &IDThreadWorker);
	cout << "Через сколько секунд вы хотите начать работу потока Worker?" << endl;
	SuspendThread(hThreadWorker);
	int sec;
	cin >> sec;
	Sleep(sec * 1000);
	ResumeThread(hThreadWorker);


	WaitForSingleObject(hThreadWorker, INFINITE);
	CloseHandle(hThreadWorker);



	return 0;
}

int main()
{
	setlocale(LC_ALL, "ru");
	HANDLE	hThreadMain;
	DWORD	IDThreadMain;
	int size;
	cout << "Введите размер массива: " << endl;
	cin >> size;

	hThreadMain = CreateThread(NULL, 0, threadMain, (void*)size, 0, &IDThreadMain);
	if (hThreadMain == NULL)
		return GetLastError();

	// ждем пока поток main закончит работу
	WaitForSingleObject(hThreadMain, INFINITE);
	// закрываем дескриптор потока
	CloseHandle(hThreadMain);

	return 0;
}
/*
Под доступом к объектам понимается возможность приложения выполнять над объектом некоторые
функции. Приложение не имеет прямого доступа к объектам. Для того чтобы получить этот доступ,
необходимо использовать его дескриптор (handle). В Win32 API дескриптор имеет тип HANDLE. Дескриптор
объекта представляет собой запись в таблице, которая поддерживается системой и содержит адрес объекта и
средства для идентификации типа объекта.
После завершения работы с объектом его дескриптор нужно закрыть, используя функцию CloseHandle
Дело в том, что в
Windows на один и тот же объект могут ссылаться несколько дескрипторов, которые создаются другими
функциями для доступа к уже созданному ранее объекту. Функция CloseHandle уничтожает объект только в
том случае, если на него больше не ссылается ни один дескриптор


Параметр lpThreadId является выходным, то есть его значение устанавливает Windows. Этот параметр
должен указывать на переменную, в которую Windows поместит идентификатор потока, который уникален для
всей системы и может в дальнейшем использоваться для ссылок на поток.
*/