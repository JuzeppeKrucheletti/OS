// Пример консольного процесса, который создает другое
// консольное приложение с новой консолью и ждет завершения
// работы этого приложения.

#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[0]);
	int* mass = new int[n];
	int el;
	int A = atoi(argv[argc - 1]);
	int j = 0;
	for (int i = 1; i < argc - 1; i++) {
		el = atoi(argv[i]);
		if (el > A) {
			mass[j] = atoi(argv[i]);
			j++;
		}
	}
	for (; j < n; j++) {
		mass[j] = 0;
	}
	for (int i = 0; i < n; i++) {
		cout << mass[i] << endl;
	}
	//_cprintf("\n My %d parameter =  %s", i, argv[i]);

	_getch();
	delete[] mass;
	return 0;
}
