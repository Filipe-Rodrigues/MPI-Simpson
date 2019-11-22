#include <iostream>
#include "mpi.h"

float f(float x){

        float return_val;
        return_val = x*x+2*x; //pode ser utilizada qualquer funo
        return return_val;

}

int main(int argc, char **argv){
        float integral;
        float a,b;
        int n;
        float h;
        float x;
        int i;
        int rank;
        float reduced_data;

        MPI_Status status;
        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &n);

        if(rank == 0){
                printf("Enter a, b:\n");
                scanf("%f %f", &a, &b);
                for(int i = 1;i < n;i++){
                        MPI_Send(&a,1,MPI_FLOAT,i,0,MPI_COMM_WORLD);
                        MPI_Send(&b,1,MPI_FLOAT,i,0,MPI_COMM_WORLD);
                }
        }
        else{
                MPI_Recv(&a,1,MPI_FLOAT,0,0,MPI_COMM_WORLD,&status);
                MPI_Recv(&b,1,MPI_FLOAT,0,0,MPI_COMM_WORLD,&status);
        }

        h = (b-a)/n;

        if(rank == 0){
                integral = (f(a) + f(b))/2.0;
        }
        else{
                x = a + (h * rank);
                integral = f(x);
        }

        MPI_Reduce(&integral,&reduced_data,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);

        if(rank == 0){
                integral = reduced_data * h;
                printf("With n = %d trapezoids, our estimate \n", n);
                printf("of the integral from %f to %f = %f\n", a, b, integral);
        }

        MPI_Finalize();
        return 0;

}