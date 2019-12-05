#include <cstdlib>
#include <fstream>
#include <limits>
#include <sstream>
#include "simpson.h"

using namespace std;

ifstream menuFile("menu.txt");
ifstream splashFile("splash.txt");

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

int readInteger () {
	string input = "";
	int valor;
	do {
		cout << "Please enter a valid number: ";
		getline(cin, input);

		// This code converts from string to number safely.
		stringstream myStream(input);
		if (myStream >> valor) break;
		cout << "Invalid number, please try again" << endl;
		cin.clear();
		cin.sync();
		cin.ignore(1000);
	} while (true);
	return valor;
}

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
	getline(cin, lixo);
}

void printSplash() {
	clear();
	string line;
	while (splashFile.good()) {
		getline(splashFile, line);
		cout << line << endl;
	}
	menuFile.clear();
	menuFile.seekg(0, ios::beg);
	pressToContinue();
}

void printMenu() {
	clear();
	string line;
	while (menuFile.good()) {
		getline(menuFile, line);
		cout << line << endl;
	}
	menuFile.clear();
	menuFile.seekg(0, ios::beg);
}

void runInstance(int procs, int iterations, string file, double a, double b, integer k, int precision) {
	stringstream ss;
	ss.str("");
	ss << "mpiexec -np " << ((procs == 0) ? (1) : (procs)) << " ";
	ss << "./parallel38 --performance-test " << iterations << " ";
	ss << "--file " << file << " ";
	ss << "-a " << a << " ";
	ss << "-b " << b << " ";
	ss << "-k " << k << " ";
	ss << "--precision " << precision;
	system (ss.str().c_str());
}

void testSeries(Configuration& conf) {
	integer k = conf.k;
	integer step = conf.step;
	integer max = k * 10;

	for (k; k <= max; k += step) {
		string file = brewFileName(k);
		cout << endl << "Now performing series of tests with K = " << k << "...";
		for (int procs = 0; procs <= conf.processes; procs += 4) {
			cout << endl << "\t...using " << ((procs == 0) ? (1) : (procs)) << " processes";
			runInstance(procs, conf.iterations, file, conf.a, conf.b, k, conf.precision);
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
		option = readInteger();
		switch (option) {
			case 1:
				clear();
				cout << "\nRunning a test with default config... ";
				runInstance(0, conf.iterations, "standalone.txt", conf.a, conf.b, conf.k, conf.precision);
				cout << "DONE!\n\nCheck your results in a generated text file at output/ directory!\n\n"
					 << "Press any key to continue...";
				pressToContinue();
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
	menuFile.close();
	splashFile.close();
	return 0;
}