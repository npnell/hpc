# Hello MPI

Simple hello world example from "An Introduction to Parallel Programming" (Pacheco, Malensek, 2nd ed).

Client processes send a greeting message to the manager process.

## Running
MPI library should be installed (preferably MPICH).
```
mpicc hello.c -o hello
mpiexec -n 2 ./hello
```