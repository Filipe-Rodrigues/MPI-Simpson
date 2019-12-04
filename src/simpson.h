#ifndef SIMPSON_H
#define SIMPSON_H 1

#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#define SQRT_PI 1.7724538509055160272981674

#define SECOND 1
#define MILLISECOND 1000.0
#define MICROSECOND 1000000.0
#define NANOSECOND 1000000000.0

using namespace std;

typedef unsigned long integer;

class Stopwatch {
	private:
		clock_t t0;
		clock_t t1;
		bool good() { return (t0 != -1 and t1 != -1); }
	public:
		Stopwatch() { reset(); }
		void reset() { t0 = -1; t1 = -1; }
		void mark();
		double getElapsedTime(double resolution);
};

void Stopwatch::mark() {
	if (t0 == -1) {
		t0 = clock();
	} else if (t1 == -1) {
		t1 = clock();
	} else {
		reset();
		t0 = clock();
	}
}

double Stopwatch::getElapsedTime(double resolution) {
	if (good()) {
		return double(resolution * (t1 - t0)) / CLOCKS_PER_SEC;
	} else {
		return -1;
	}
}

double f(double x) {
	return exp(-x * x);
}

double simpson2ndRule(double a, double b, double k) {
	double n = 3*k;
	double h = (b - a) / n;
	double integral = f(a) + f(b);
	for (integer i = 1; i < n; i++) {
		if (i % 3 == 0) {
			integral += 2 * f(a + i*h);
		} else {
			integral += 3 * f(a + i*h);
		}
	}

	integral *= (3.0/8.0) * h;
	return integral;
}

double getError(double result) {
	return abs(result - SQRT_PI);
}

double getAccuracy(double result) {
	return 1.0 - (getError(result) / SQRT_PI);
}

void printResults(double result, double time, short precision, string method) {
	cout << "\nThe Simpson 3/8 Rule resulted in I = ";
	cout << setprecision(precision) << fixed << result << endl;
	cout << setprecision(3) << fixed << "The time taken in this "
		 << method << " method was " << time << " ms\n";
	cout << setprecision(precision) << fixed << "The accuracy of this result was "
		 << (getAccuracy(result) * 100.0) << "%\n\n";
}

#endif