#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

const double pi = M_PI;

double random_double(double min, double max) {
    return min + (max - min) * (rand() / (RAND_MAX + 1.0));
}

int main(int argc, char* argv[])
{
    int     comm_sz;
    int     my_rank;

    long double sum = 0.0;
    int N = 1000000;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double interval = 2 * pi / comm_sz;
    double a = interval * my_rank;
    double b = a + interval;

    for(int i = 0; i < N; ++i) {
        double x = random_double(a,b);
        sum += x * powl(sinl(x),2);
    }

    long double area = interval * sum / N;
    long double sum_area = 0.0;

    MPI_Reduce(&area, &sum_area, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    printf("area: %Lf for process %d of %d\n", area, my_rank, comm_sz);

    if(my_rank == 0) {
        sleep(1);
        long double error = 100.0 * fabsl(sum_area - 9.86960440108936) / 9.86960440108936;

        printf("Area under the curve for x * sin^2(x) between [0,2pi] is: %.14Lf\n", sum_area);
        printf("The error is: %.14Lf", error);
    }
}