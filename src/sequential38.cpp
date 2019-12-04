#include <cstdlib>
#include <fstream>
#include "simpson.h"

int main(int argc, char const **argv) {
	
	int param = 1;
	int iterations = 1;
	double a, b, integral, time;
	integer k;
	short precision;
	string file, arg;

	while (param < argc) {
		arg = argv[param++];
		if (arg == "--performance-test") {
			iterations = stoi(argv[param++]);
		} else if (arg == "--file") {
			file = string(argv[param++]);
		} else if (arg == "-a") {
			a = stod(argv[param++]);
		} else if (arg == "-b") {
			b = stod(argv[param++]);
		} else if (arg == "-k") {
			k = stoul(argv[param++]);
		} else if (arg == "--precision") {
			precision = (short) stoi(argv[param++]);
		} else {
			cout << arg;
			return -1;
		}
	}

	string outputName = "output/SEQ_" + file;
	Stopwatch stopwatch;

	if (iterations > 1) {
		ofstream output(outputName.c_str());
		output << iterations << endl;
		cout << endl;
		time = 0;
		for (int i = 0; i < iterations; i++) {
			stopwatch.mark();
			integral = simpson2ndRule(a, b, k);
			stopwatch.mark();
			time += stopwatch.getElapsedTime(MILLISECOND);
			output << setprecision(precision) << fixed << time / (i + 1.0) << endl;
			cout << setprecision(precision) << fixed << time / (i + 1.0)  << endl;
		}
		output << endl << setprecision(precision) << fixed << getAccuracy(integral);
		output.close();
		time /= (double) iterations;
	} else {
		stopwatch.mark();
		integral = simpson2ndRule(a, b, k);
		stopwatch.mark();
		time = stopwatch.getElapsedTime(MILLISECOND);
	}

	printResults(integral, time, precision, "SEQUENTIAL");

	return 0;
}