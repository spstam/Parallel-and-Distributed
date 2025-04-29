/******************************************************************************
* file: ask3.c
* Description:
*   Program that tests the parallel implementation of the Jacobi method.
* Authors: Thanos Kastoras, Spiros Stamoulis
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#include "jacobi_par.h"

int main(int argc, char *argv[])
{
	// Get N, maxIter and tol from the command line
	if (argc != 4) {
		printf("Usage: %s N maxIter tol\n", argv[0]);
		return 1;
	}

	num_t error;
	int N = atoi(argv[1]);
	int maxIter = atoi(argv[2]);
	num_t tol = atof(argv[3]);
	double start_time, end_time;


	// Allocate memory for A, b and x
	num_t *A = (num_t *)malloc(N * N * sizeof(num_t));
	num_t *b = (num_t *)malloc(N * sizeof(num_t));
	num_t *x = (num_t *)calloc(N, sizeof(num_t));
#ifdef SERIAL
	num_t *x_serial;
	x_serial = (num_t *)calloc(N, sizeof(num_t));
	#pragma omp parallel shared(A, b, N, maxIter, tol, x_serial, x, error)
#else
	#pragma omp parallel shared(A, b, N, maxIter, tol, x, error)
#endif
	{
		// Initialize A and b
		#pragma omp for schedule(static)
		for (int i = 0; i < N; i++) {
			b[i] = (i == N-1) ? N + 1.0 : 0.0;
			for (int j = 0; j < N; j++) {
				A[i * N + j] =	(i == j) 				?  2.0 :
								(i-j == 1 || j-i == 1)	? -1.0 :
														0.0;
			}
		}
		#pragma omp single
		{
			// Print the system information
			printf(" * Parallel Jacobi Method Test Program * \n");
			printf("System Dimentions: (%d x %d)\n", N, N);
			printf("Max Iterations: %d\n", maxIter);
			printf("Tolerance: %.e\n", tol);
			printf("Number of threads: %d\n", omp_get_num_threads());

#ifdef SERIAL
			printf("* Running the serial algorithm...\n");
			// Solve the system using the Jacobi method
			start_time = omp_get_wtime();
			error = jacobi_serial(A, b, N, maxIter, tol, x_serial);
			end_time = omp_get_wtime();
			printf("\tSerial code error: %.e\n", error);
			printf("\tSerial code execution time: %lf\n", end_time - start_time);
#endif

			printf("* Running the parallel algorithm...\n");
			start_time = omp_get_wtime();
		}
		// Solve the system using the Jacobi method
		num_t error_par = jacobi_parallel(A, b, N, maxIter, tol, x);

		#pragma omp single
		{
			end_time = omp_get_wtime();
			error = error_par;
			printf("\tParallel code error: %.e\n", error);
			printf("\tParallel code execution time: %lf\n", end_time - start_time);
		}
	} // End of omp parallel region

	// Free the allocated memory
	free(A);
	free(b);
	free(x);
#ifdef SERIAL
	free(x_serial);
#endif
	// Print the result
	return 0;
}