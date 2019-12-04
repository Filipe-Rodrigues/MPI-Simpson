#include <cstdlib>
#include <fstream>
#include <limits>
#include <sstream>
#include "simpson.h"

using namespace std;

struct Configuration {
	int iterations;
	int processes;
	int precision;
	integer k;
	integer step;
	double a;
	double b;

	Configuration() {
		processes = 16;
		iterations = 100;
		precision = 25;
		k = 100000;
		step = 100000;
		a = -100000;
		b = 100000;
	}
	
};

string brewFileName(integer k) {
	string fileName = "K_" + to_string(k) + ".txt";
	return fileName;
}

void clear () {
	system ("clear");
	cout << endl;
}

void pressToContinue() {
	string lixo;
	cin >> lixo;
	cin.ignore(10000, '\n');
}

void printSplash() {
	clear();
	ifstream input("splash.txt");
	string line;
	while (input.good()) {
		getline(input, line);
		cout << line << endl;
	}
	input.close();
	pressToContinue();
}

void printMenu() {
	clear();
	ifstream input("menu.txt");
	string line;
	while (input.good()) {
		getline(input, line);
		cout << line << endl;
	}
	input.close();
}

void testSeries(Configuration& conf) {
	integer k = conf.k;
	integer step = conf.step;
	integer max = k * 10;
	stringstream ss;

	for (k; k <= max; k += step) {
		string file = brewFileName(k);
		cout << endl << "Now performing series of tests with K = " << k << "...";
		for (int procs = 0; procs <= conf.processes; procs += 4) {
			ss.str("");
			ss << "mpiexec -np " << ((procs == 0) ? (1) : (procs)) << " ";
			ss << "./parallel38 --performance-test " << conf.iterations << " ";
			ss << "--file " << file << " ";
			ss << "-a " << conf.a << " ";
			ss << "-b " << conf.b << " ";
			ss << "-k " << k << " ";
			ss << "--precision " << conf.precision;
			cout << endl << "\t...using " << ((procs == 0) ? (1) : (procs)) << " processes";
			system (ss.str().c_str());
		}
		cout << endl;
	}
	cout << endl << "FINISHED!! Press enter to continue...";
	pressToContinue();
}

int main() {
	Configuration conf;
	int option = 0;
	printSplash();
	do {
		printMenu();
		cout << "    > ";
		cin >> option;
		cin.ignore(10000, '\n');
		switch (option) {
			case 1:
				clear();
				system("mpirun -np 4 ./simpson38");
				break;
			case 2:

				break;
			case 3:
				testSeries(conf);
				break;
			case 4:
				
				break;
		}
	} while (option != 0);
	return 0;
}