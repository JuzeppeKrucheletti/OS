#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <time.h>
#include <process.h>
#include <conio.h>
using namespace std;
HANDLE WorkBegin;
class MonitorQueue
{
	HANDLE Full, Empty, Access;
	int* mass;
	int qend = 0;
	int qbegin = 0;
	int size = 0;
public:
	MonitorQueue() {};
	void CreateMQ(int size) {
		this->size = size;
		Full = CreateSemaphore(NULL, size, size, "SemaphoreProd");
		if (Full == NULL)
		{
			cout << "Create semaphore failed. Press any key to exit." << endl;
			cin.get();
		}

		Empty = CreateSemaphore(NULL, 0, size, "SemaphoreCons");
		if (Empty == NULL)
		{
			cout << "Create semaphore failed. Press any key to exit." << endl;
			cin.get();
			CloseHandle(Full);
		}
		Access = CreateMutex(NULL, FALSE, "Mutex");
		if (Access == NULL)
		{
			cout << "Create mutex failed." << endl;
			cout << "Press any key to exit." << endl;
			cin.get();
		}
		mass = new int[size];
		for (int i = 0; i < size; i++) {
			mass[i] = 0;
		}
	
		int qend = 0;
		int qbegin = 0;





	}
	bool isEmpty() {
		if (qend == qbegin) return true;
		return false;
	}
	bool isFull() {
		if (qend - qbegin == size) return true;
		return false;
	}
	
	~MonitorQueue() {
		delete[] mass;
		CloseHandle(Access);
		CloseHandle(Empty);
		CloseHandle(Full);
	};
	void AddHead(int nElement) {
		WaitForSingleObject(Full, INFINITE);
		WaitForSingleObject(Access, INFINITE);
		mass[qend] = nElement;
		qend++;
		if (qend > size - 1) qend -= size;
		cout << "Producer произвёл продукт: " << nElement << endl;
		ReleaseMutex(Access);
		ReleaseSemaphore(Empty, 1, NULL);
		Sleep(rand() % 500);
	};
	void RemoveTail() {
		
		WaitForSingleObject(Empty, INFINITE);
		WaitForSingleObject(Access, INFINITE);
		int ret = qbegin;
		if (this->isFull());
		qbegin++;
		if (qbegin > size - 1) qbegin -= size;
		cout << "Consumer потребил продукт: " << mass[ret] << endl;
		ReleaseMutex(Access);
		ReleaseSemaphore(Full, 1, NULL);
		
		Sleep(rand()%500);

	};
};

MonitorQueue Q;

DWORD WINAPI threadConsumer(LPVOID mass)
{
	WaitForSingleObject(WorkBegin, INFINITE);

	
	
	while (true) {

		Q.RemoveTail();
		
	}
		
	
	return 0;
}

DWORD WINAPI threadProducer(LPVOID mass)
{

	WaitForSingleObject(WorkBegin, INFINITE);
	
	srand(time(NULL));
	while (true) {
		int prod = rand();
		Q.AddHead(prod);
		
	}
	return 0;
}






int main()
{
	setlocale(LC_ALL, "ru");

	
	
	int size;
	cout << "Введите размер хранилища:" << endl;
	cin >> size;
	Q.CreateMQ(size);
	







	DWORD	IDThreadConsumer;
	DWORD	IDThreadProducer;

	HANDLE* H = new HANDLE[2];
	H[0] = CreateThread(NULL, 0, threadProducer, (void*)0, 0, &IDThreadProducer);
	H[1] = CreateThread(NULL, 0, threadConsumer, (void*)0, 0, &IDThreadConsumer);
	cout << "Потоки начали работу." << endl;
	SetEvent(WorkBegin);


	WaitForMultipleObjects(2, H, true, INFINITE);

	for (int i = 0; i < 2; i++) {
		CloseHandle(H[i]);
	}
	CloseHandle(WorkBegin);

	return 0;
}