#include <fstream>
#include "simpson.h"

using namespace std;

string brewFileName(int proc, integer k) {
	string fileName = "PAR_" + to_string((proc == 0) ? (1) : (proc)) + "_K_" + to_string(k) + ".txt";
	return fileName;
}

int main() {
	int k = 100000;
	ofstream outputTime("output/ALL_TIMES.txt");
	ofstream outputAcc("output/ALL_ACCURACIES.txt");
	for (k; k <= 1000000; k += 100000) {
		int proc = 0;
		for (proc; proc <= 16; proc += 4) {
			string file = "output/" + brewFileName(proc, k);
			ifstream input(file.c_str());
			double time, acc;
			input >> time;
			input >> acc;
			input.close();
			outputTime << time << "\t";
			outputAcc << setprecision(16) << fixed << acc << "\t";
		}
		outputTime << "\n";
		outputAcc << "\n";
	}
	outputTime.close();
	outputAcc.close();
}