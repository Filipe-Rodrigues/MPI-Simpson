#include <fstream>
#include "simpson.h"

using namespace std;

double* sequentialTimes = new double[10];

string brewFileName(int proc, integer k) {
	string fileName = "PAR_" + to_string((proc == 0) ? (1) : (proc)) + "_K_" + to_string(k) + ".txt";
	return fileName;
}

int main() {
	int k = 100000;
	ofstream outputTime("output/ALL_TIMES.txt");
	ofstream outputSpdUp("output/ALL_SPEEDUPS.txt");
	ofstream outputEff("output/ALL_EFFICIENCIES.txt");

	for (k; k <= 1000000; k += 100000) {
		int proc = 0;
		for (proc; proc <= 16; proc += 4) {
			string file = "output/" + brewFileName(proc, k);
			ifstream input(file.c_str());
			double time;
			input >> time;
			input.close();
			outputTime << time << "\t";
			if (proc == 0) {
				sequentialTimes[k / 100000] = time;
			} else {
				double speedUp = sequentialTimes[k / 100000] / time;
				double efficiency = speedUp / (double) proc;
				outputSpdUp << setprecision(3) << fixed << speedUp << "\t";
				outputEff << setprecision(3) << fixed << efficiency << "\t";
			}
		}
		outputTime << "\n";
		outputSpdUp << "\n";
		outputEff << "\n";
	}
	outputTime.close();
	outputSpdUp.close();
	outputEff.close();
}