# Parallel Monte Carlo Integration

Finding area under the curve for x * sin^2(x) between [0,2pi], using the Monte Carlo Integration method on a distributed cluster.

Each individual node calculates the subarea for a section of the interval, then all subareas are summed using MPI_Reduce.

## Running
MPI library should be installed (preferably MPICH).
```
mpicc integration.c -o integration -lm
mpiexec -n 8 ./integration
```