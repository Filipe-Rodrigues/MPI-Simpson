#include <cstdlib>
#include <fstream>
#include "mpi.h"
#include "simpson.h"

using namespace std;

void parallelmpi(int argc, char** argv, double a, double b, integer k, short precision) {

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
	
}

int main(int argc, char** argv) {
	int rank, processes;
	double reduced_data;

	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	int iterations;
	double a, b, integral, time;
	integer k;
	short precision;
	string file, arg;

	string outputName;
	Stopwatch stopwatch;

	if (rank == 0) {
		int param = 1;

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

		outputName = "output/PAR_" + to_string(processes) + "_" + file;
		Stopwatch stopwatch;
		stopwatch.mark();
		for(int i = 1;i < processes;i++){
			MPI_Send(&a,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(&b,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(&k,1,MPI_UNSIGNED_LONG,i,0,MPI_COMM_WORLD);
			MPI_Send(&precision,1,MPI_SHORT,i,0,MPI_COMM_WORLD);
			MPI_Send(&iterations,1,MPI_INT,i,0,MPI_COMM_WORLD);
		}
		stopwatch.mark();
		time = stopwatch.getElapsedTime(MILLISECOND);
	} else {
		MPI_Recv(&a,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&b,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&k,1,MPI_UNSIGNED_LONG,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&precision,1,MPI_SHORT,0,0,MPI_COMM_WORLD,&status);
		MPI_Recv(&iterations,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
	}

	short go = 0;

	if (rank == 0) {
		stopwatch.mark();
		for(int i = 1;i < processes;i++){
			MPI_Send(&go,1,MPI_SHORT,i,0,MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(&go,1,MPI_SHORT,0,0,MPI_COMM_WORLD,&status);
	}

	integer k_divided = (integer) ceil((double) k / (double) processes);
	double interval_start = a + rank * ((b - a) / processes);
	double interval_end = a + (rank + 1) * ((b - a) / processes);

	if (iterations > 1) {
		if (rank == 0) {
			stopwatch.mark();
			time += stopwatch.getElapsedTime(MILLISECOND);
		}
		ofstream output(outputName.c_str());
		if (rank == 0) {
			//output << iterations << endl;
			//cout << endl;
		}
		time = 0;
		for (int i = 0; i < iterations; i++) {
			if (rank == 0) {
				stopwatch.mark();
				for(int i = 1;i < processes;i++){
					MPI_Send(&go,1,MPI_SHORT,i,0,MPI_COMM_WORLD);
				}
			} else {
				MPI_Recv(&go,1,MPI_SHORT,0,0,MPI_COMM_WORLD,&status);
			}
			integral = simpson2ndRule(interval_start, interval_end, k_divided);
			MPI_Reduce(&integral,&reduced_data,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
			if (rank == 0) {
				stopwatch.mark();
				time += stopwatch.getElapsedTime(MILLISECOND);
				//output << setprecision(precision) << fixed << time / (i + 1.0) << endl;
				//cout << setprecision(precision) << fixed << time / (i + 1.0)  << endl;
				integral = reduced_data;
			}
		}
		if (rank == 0) {
			time /= (double) iterations;
			output << setprecision(3) << fixed << time;
			output << endl << setprecision(precision) << fixed << getAccuracy(integral);
			//output << endl << setprecision(precision) << fixed << (100 - getAccuracy(integral));
			output.close();
		}
	} else {
		integral = simpson2ndRule(interval_start, interval_end, k_divided);
		MPI_Reduce(&integral,&reduced_data,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
		if (rank == 0) {
			stopwatch.mark();
			time += stopwatch.getElapsedTime(MILLISECOND); 
			integral = reduced_data;
		}
	}

	if (rank == 0) {
		//printResults(integral, time, precision, "PARALLEL");
	}

	MPI_Finalize();
	return 0;
}
