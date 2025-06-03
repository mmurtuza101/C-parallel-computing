# Parallel Computing Projects in C

This repository contains two projects demonstrating the use of multi-threading (with Pthreads) and distributed computing (with MPI) to solve computational problems using C.

## Problem 1: MPI-Based Count Sort

**File**: `31586302_Problem1.c`  
**Tech**: C, MPI (Message Passing Interface)

- Parallelized a count sort algorithm using MPI to sort binary file data across multiple processes.
- Each MPI process reads and processes a partition of an integer dataset.
- Local frequency counts are computed and reduced using `MPI_Reduce` to compile a global sorted result.
- Handles binary file I/O and scales based on the number of MPI processes.

### How to Compile and Run:
```bash
mpicc -o count_sort 31586302_Problem1.c
mpirun -np 4 ./count_sort input_file.bin
```
## Problem 2: Pthread-based Pi Estimation

**File**: `31586302_Problem2.c`
**Tech**: C, Pthreads

- Approximates the value of π using numerical integration and multi-threading.
- Workload is evenly split across threads, each computing a portion of the integral.
- Thread-safe summation using pthread_mutex ensures accuracy.
- Calculates the final approximation and reports the absolute error from the real value.

### How to Compile and Ruin:
```bash
gcc -o pi_estimate 31586302_Problem2.c -lpthread -lm
./pi_estimate <num_terms> <num_threads>
```
