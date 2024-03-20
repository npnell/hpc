#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>

void swap(int *A, int i, int j)
{
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

void merge(int A[], int res[], int p, int q, int r)
{
    if(r - p + 1 > 8) {
        merge(A, res, p, (p + q) * 0.5, q);
        merge(A, res, q + 1, (q + 1 + r) * 0.5, r);
    }

    int m = 0, n = 0;
    for(int i = p; i <= r; ++i) {
        if(res[p + m] < res[q + 1 + n] && m <= q - p) {
            A[i] = res[p + m];
            m = m + 1;
        } else if(n <= r - q - 1) {
            A[i] = res[q + 1 + n];
            n = n + 1;
        }
    }
    for(int i = p; i < r; ++i) {
        res[i] = A[i];
    }
}

int partition(int A[], int p, int r)
{
    int x = A[r];
    int i = p - 1;
    for(int j = p; j < r; ++j) {
        if(A[j] <= x) {
            i = i + 1;
            swap(A, i, j);
        }
    }
    swap(A, i + 1, r);
    return i + 1;
}

void quicksort(int A[], int p, int r)
{
    if(p < r) {
        int q = partition(A, p, r);
        quicksort(A, p, q - 1);
        quicksort(A, q + 1, r);
    }
}

int main(int argc, char* argv[])
{
    int A[] = {4, 1, 11, 3, 2, 16, 9, 6, 10, 14, 8, 7, 21, 13, 17, 5};
    int n = sizeof(A) / sizeof(A[0]);

    MPI_Init(NULL, NULL);

    int     comm_sz;
    int     my_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int p, r;
    int res[16];
    if(my_rank == 0) {
        for(int i = 1; i <= 4; ++i) {
            p = (i - 1) * 4;
            r = (i - 1) * 4 + 3;
            MPI_Send(&p, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&r, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        sleep(1);
        for(int i = 1; i <= 4; ++i)
            MPI_Recv(res + (i - 1) * 4, 4, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        merge(A, res, 0, 7, 15);
    } else {
        sleep(2);
        MPI_Recv(&p, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&r, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        quicksort(A, p, r);

        MPI_Send(A + p, 4, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    if(my_rank == 0) {
        for(int i = 0; i < n; ++i)
            printf("%d ", A[i]);
        printf("\n");
    }

    return 0;
}