# Parallel Quicksort

Quicksort on a distributed cluster.

Manager process assigns a subarray (of the whole input array) to each individual node (process), which is then sorted independently, then finally all subarrays are merged using the recursive tree method.

## Running
MPI library should be installed (preferably MPICH).
```
mpicc quicksort.c -o quicksort
mpiexec -n 5 ./quicksort
```