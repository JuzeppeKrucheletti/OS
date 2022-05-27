// Пример создания потока функцией CreateThread

#include <windows.h>
#include <iostream>
#include <time.h>
#include <process.h>
using namespace std;

//fffffff
CRITICAL_SECTION cs;
HANDLE hSummBegin, hSummEnd;
//fffffff
UINT WINAPI threadSummElement(LPVOID mass_and_size)
{
	WaitForSingleObject(hSummBegin, INFINITE);
	EnterCriticalSection(&cs);
	int** mass_and_size1 = (int**)mass_and_size;
	for (int i = mass_and_size1[0][2]-1; i < mass_and_size1[0][3]; i++) {
		mass_and_size1[0][4] += mass_and_size1[1][i];
	}
	mass_and_size = mass_and_size1;
	LeaveCriticalSection(&cs);
	SetEvent(hSummEnd);
	return 0;
}
UINT WINAPI threadWorker(LPVOID mass_and_size)
{
	cout << "Введите количество миллисекунд, на которые поток будет останавливаться после подготовки однонго элемента в массиве:" << endl;
	int millisecond;
	cin >> millisecond;
	int** mass_and_size1 = (int**)mass_and_size;
	int k = 0;
	bool b = true;
	for (int i = 0; i < mass_and_size1[0][0]; i++) {
		if (mass_and_size1[1][i] > mass_and_size1[0][1]) {
			mass_and_size1[1][k] = mass_and_size1[1][i];
			Sleep(millisecond);
			if (k == mass_and_size1[0][3]) {
				mass_and_size = mass_and_size1;
				b = false;
				//fff
				SetEvent(hSummBegin);
				WaitForSingleObject(hSummEnd, INFINITE);
				//fff
			}
			k++;
		}
	}
	for (int i = k; i < mass_and_size1[0][0]; i++) {
		mass_and_size1[1][i] = 0;
		if (i == mass_and_size1[0][3]&&b==true) {
			mass_and_size = mass_and_size1;
			b = false;
			//fff
			SetEvent(hSummBegin);
			WaitForSingleObject(hSummEnd, INFINITE);
			//fff
		}
	}
	mass_and_size = mass_and_size1;
	if (b == true) { SetEvent(hSummBegin);
	WaitForSingleObject(hSummEnd, INFINITE);
	}
	return 0;
}
DWORD WINAPI threadMain(LPVOID mass_size)
{
	//ffffff
	InitializeCriticalSection(&cs);
	hSummBegin = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hSummBegin == NULL)
		return GetLastError();
	InitializeCriticalSection(&cs);
	hSummEnd = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hSummEnd == NULL)
		return GetLastError();
	//fffrf



	int* mass;
	mass = new int[int(mass_size)];
	cout << "Введите элементы массива с консоли:" << endl;
	for (int i = 0; i<int(mass_size); i++)
	{
		cin >> mass[i];
	}
	cout << "Созданный массив: " << endl;
	for (int i = 0; i<int(mass_size); i++)
	{
		cout << mass[i] << endl;
	}
	cout << endl<<"Введите числа А, K1, K2.\n A - элементы массива больше данного числа будут размещены слева, остальные зполнены нулями.\nК1 и К2 - границы суммирования элементов массива.\n";
	int A, K1, K2;
	cin >> A;
	while (true) {
		cin >> K1 >> K2;
		if (K1 >= K2 || K1<=0 || K2<=0 || K2>(int)mass_size || K1>(int)mass_size) {
			cout << "К1 или К2 введены некорректно... Попробуйте снова." << endl;
		}
		else {
			break;
		}
	}
	int** mass_and_size = new int* [2];
	mass_and_size[0] = new int[5];
	mass_and_size[0][0] = int(mass_size);
	mass_and_size[0][1] = A;
	mass_and_size[0][2] = K1;
	mass_and_size[0][3] = K2;
	mass_and_size[0][4] = 0;
	mass_and_size[1] = new int[mass_and_size[0][0]];
	for (int i = 0; i < mass_and_size[0][0]; i++)
	{
		mass_and_size[1][i] = mass[i];
	}
	HANDLE	hThreadWorker;
	UINT	IDThreadWorker;
	HANDLE	hThreadSummElement;
	UINT	IDThreadSummElement;


	hThreadWorker = (HANDLE)_beginthreadex(NULL, 0, threadWorker, (void*)mass_and_size, 0, &IDThreadWorker);
	hThreadSummElement = (HANDLE)_beginthreadex(NULL, 0, threadSummElement, (void*)mass_and_size, 0, &IDThreadWorker);
	WaitForSingleObject(hSummBegin, INFINITE);
	cout << "Поток SummElement начал работу." << endl;
	EnterCriticalSection(&cs);
	cout << "Поток SummElement закончил работу. Cумма элементов: " << mass_and_size[0][4]<<endl;
	LeaveCriticalSection(&cs);
	WaitForSingleObject(hThreadWorker, INFINITE);
	cout << "Поток Work закончил работу. Итоговый массив:" << endl;
	for (int i = 0; i < mass_and_size[0][0]; i++) {
		cout<< mass_and_size[1][i]<<endl;
	}
	CloseHandle(hThreadWorker);
	WaitForSingleObject(hThreadSummElement, INFINITE);
	CloseHandle(hThreadSummElement);
	



	//fffffff
	DeleteCriticalSection(&cs);
	CloseHandle(hSummBegin);
	CloseHandle(hSummEnd);
	//ffffffff
	 

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

	
	WaitForSingleObject(hThreadMain, INFINITE);
	
	CloseHandle(hThreadMain);

	return 0;
}
/*
*/