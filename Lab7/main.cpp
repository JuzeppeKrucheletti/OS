// Пример консольного процесса, который создает другое
// консольное приложение с новой консолью и ждет завершения
// работы этого приложения.
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <time.h>
#include <process.h>
#include <conio.h>
using namespace std;
CRITICAL_SECTION cs;

class MonitorQueue
{
public:
	int* mass;
	int qend = 0;
	int qbegin = 0;
	int size = 0;
public:
	MonitorQueue(int n) {
		mass = new int[n];
		for (int i = 0; i < n; i++) {
			mass[i] = 0;
		}
		size = n;
		int qend = 0;
		int qbegin = 0;
	};
	bool isEmpty() {
		if (qend == qbegin) return true;
		return false;
	}
	bool isFull() {
		if (qend - qbegin == size ) return true;
		return false;
	}
	MonitorQueue& operator = (const MonitorQueue& B)
	{
		if (this == &B)
			return *this;
		
		size = B.size;
		qend = B.qend;
		qbegin = B.qbegin;
		delete[] mass;
		mass = new int [size];
		
		for (int i = 0; i < size; i++)
		{
			mass[i] = B.mass[i];
		}
		return *this;
	}
	~MonitorQueue() {
		delete[] mass;
	};
	void AddHead(int nElement) {
		
		mass[qend] = nElement;
		qend++;
		if (qend > size - 1) qend -= size;
	};
	int RemoveTail() {
		int ret = qbegin;
		if (this->isFull()) ;
		qbegin++;
		if (qbegin > size - 1) qbegin -= size;
		return mass[ret];

	}; 
};

MonitorQueue Q(1);

DWORD WINAPI threadConsumer(LPVOID mass)
{
	

	HANDLE SemPr, SemCons, hMutex;

	SemPr = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (SemPr == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	SemCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (SemCons == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemPr);
		return GetLastError();
	}

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemCons);
		CloseHandle(SemPr);
		return GetLastError();
	}

	int* mass1 = (int*)mass;
	int numb = mass1[0];
	int count = mass1[1];
	for (int i = 0; i < count; i++) {
		WaitForSingleObject(SemCons, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		int prod = Q.RemoveTail();
		cout << "Потребитель №" << numb << " потребил продукт " << prod<<endl;
		//cout << Q.qend << " " << Q.qbegin << endl;
		ReleaseMutex(hMutex);
		ReleaseSemaphore(SemPr, 1, NULL);
	}
	CloseHandle(hMutex);
	CloseHandle(SemCons);
	CloseHandle(SemPr);
	return 0;
}

DWORD WINAPI threadProducer(LPVOID mass)
{
	
	HANDLE SemPr, SemCons, hMutex;

	SemPr = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (SemPr == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	SemCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (SemCons == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemPr);
		return GetLastError();
	}

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemCons);
		CloseHandle(SemPr);
		return GetLastError();
	}
	srand(time(0));
	int* mass1 = (int*)mass;
	int numb = mass1[0];
	int count = mass1[1];
	for (int i = 0; i < count; i++) {
		WaitForSingleObject(SemPr, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		int prod = rand()%10+i*3;
		Q.AddHead(prod);
		cout << "Производитель №" << numb << " произвёл продукт " << prod << endl;
		//cout << Q.qend << " " << Q.qbegin << endl;
		ReleaseMutex(hMutex);
		ReleaseSemaphore(SemCons, 1, NULL);
	}
	CloseHandle(hMutex);
	CloseHandle(SemCons);
	CloseHandle(SemPr);
	return 0;
}






int main()
{
	setlocale(LC_ALL, "ru");

	HANDLE SemPr, SemCons, hMutex;
	srand(time(0));

	int size;
	cout << "Введите размер очререди:" << endl;
	cin >> size;
	MonitorQueue Q1(size);
	Q = Q1;
	SemPr = CreateSemaphore(NULL, 0, size, "SemaphoreProd");
	if (SemPr == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	SemCons = CreateSemaphore(NULL, 0, size, "SemaphoreCons");
	if (SemCons == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(SemPr);
		return GetLastError();
	}

	hMutex = CreateMutex(NULL, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}
	int n_cons, n_prod, m_count;
	cout << "Введите количество потоков Producer:" << endl;
	cin >> n_prod;
	cout << "Введите количество потоков Consumer:" << endl;
	cin >> n_cons;







	DWORD	IDThreadConsumer;
	DWORD	IDThreadProducer;

	HANDLE* H = new HANDLE[n_prod + n_cons];


	int Summ_prod = 0, Summ_cons = 0;
	while (true) {
		for (int i = 0; i < n_prod; i++) {
			cout << "Введите количество произведенных товаров для потока Producer " << i + 1 << ":\n";
			int count_prod;
			cin >> count_prod;
			Summ_prod += count_prod;
			int* mass = new int[2];
			mass[0] = i + 1;
			mass[1] = count_prod;
			H[i] = CreateThread(NULL, 0, threadProducer, (void*)mass, 0, &IDThreadProducer);
		}
		for (int i = 0; i < n_cons; i++) {
			cout << "Введите количество потребленных товаров для потока Consumer " << i + 1 << ":\n";
			int count_cons;
			cin >> count_cons;
			Summ_cons += count_cons;
			int* mass = new int[2];
			mass[0] = i + 1;
			mass[1] = count_cons;
			H[i + n_prod] = CreateThread(NULL, 0, threadConsumer, (void*)mass, 0, &IDThreadConsumer);
		}
		if (Summ_cons > Summ_prod) {
			cout << "Товаров недостаточно, чтобы обеспечить потребителей. Попробуйте ввести данные еще раз." << endl;
		}
		else if (Summ_prod > Summ_cons + size) {
			cout << "Слишком много произведенных товаров для потребления и хранения. Попробуйте ещё раз." << endl;
		}
		else break;
	}
	cout << "Потоки начали работу." << endl;
	


	ReleaseSemaphore(SemPr, size, NULL);
	WaitForMultipleObjects(n_cons + n_prod, H, true, INFINITE);

	for (int i = 0; i < n_cons + n_prod; i++) {
		CloseHandle(H[i]);
	}
	CloseHandle(hMutex);
	CloseHandle(SemCons);
	CloseHandle(SemPr);


	return 0;
}