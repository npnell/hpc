# Parallel Quicksort

Quicksort on a distributed cluster.

Manager process divides the input array into several subarrays and a individual node (process) sorts each subarray, then finally all subarrays are merged using recursive tree method.

## Running
MPI library should be installed (preferably MPICH).
```
mpicc quicksort.c -o quicksort -lm
mpiexec -n 5 ./quicksort
```