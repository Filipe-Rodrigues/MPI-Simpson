#include <cstdlib>
#include <fstream>
#include "simpson.h"

using namespace std;

void clear () {
	system ("clear");
	cout << endl;
}

void printSplash() {
	clear();
	ifstream input("splash");
	string line;
	while (getline(input, line)) {
		cout << line << endl;
	}
	string lixo;
	cin >> lixo;
	clear();
}

void printMenu() {
	clear();
	ifstream input("menu.txt");
	string line;
	while (getline(input, line)) {
		cout << line << endl;
	}
}

int main() {
	printSplash();
	int option, iterations = 10, processes, precision = 25;
	integer k, step = 100000;
	string file;
	double a, b
	do {
		printMenu();
		cout << "    > ";
		cin >> option;
		switch (option) {
			case 1:
				system("./sequential38 --performance-test " + iterations
					+ " --file test1.txt -a -100000 -b 100000 -k 10000000 --precision 25");
		}
		system("mpirun -np 4 ./simpson38");
	} while (option != 0);
	return 0;
}