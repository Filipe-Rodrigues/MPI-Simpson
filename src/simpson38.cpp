#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include "mpi.h"

using namespace std;

typedef unsigned long integer;

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

void init(int argc, char** argv) {
	double a, b;
	integer k;
	short precision;
	char parallel;
	cout << "\nGive me an interval and the number of cubic interpolations: ";
	cin >> a >> b >> k;
	cout << "Now give me a precision: ";
	cin >> precision;
	cout << "Do you want it to be parallel? [Y|n] ";
	cin >> parallel;
	if (parallel != 'n' and parallel != 'N') {
		parallel(argc, argv, a, b, k, precision);
	} else {
		sequential(a, b, k, precision);
	}
}

void printResults(double result, short precision, string method) {
	cout << "\nThe Simpson 3/8 Rule resulted in I = ";
	cout << setprecision(precision) << fixed << results << endl << endl;
}

void sequential(double a, double b, integer k, short precision) {
	printResults(simpson2ndRule(a, b, k), precision, "SEQUENTIAL");
}

void parallel(int argc, char** argv, double a, double b, integer k, short precision) {
	int rank, processes;
	double reduced_data;

	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	if (rank == 0) {
		for(int i = 1;i < processes;i++){
			MPI_Send(&a,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(&b,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(&k,1,MPI_UNSIGNED_LONG,i,0,MPI_COMM_WORLD);
			MPI_Send(&precision,1,MPI_SHORT,i,0,MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(&a,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&b,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&k,1,MPI_UNSIGNED_LONG,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&precision,1,MPI_SHORT,0,0,MPI_COMM_WORLD,&status);
	}

	integer k_divided = (integer) ceil((double) k / (double) processes);
	double interval_start = a + rank * ((b - a) / processes);
	double interval_end = a + (rank + 1) * ((b - a) / processes);
	double integral = simpson2ndRule(interval_start, interval_end, k_divided);

	MPI_Reduce(&integral,&reduced_data,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

	if (rank == 0) {
		integral = reduced_data;
		printResults(integral, precision, "PARALLEL");
	}

}

int main(int argc, char** argv) {
	init(argc, argc);
	return 0;
}
