/******************************************************************************
* file: matrix_norm.c
* Description:
*   Parallel implementation of the Jacobi method for solving a system of linear
*   equations.
* Authors: Thanos Kastoras, Spiros Stamoulis
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include "jacobi_par.h"

#ifdef JACOBI_MESSAGES
num_t calculate_residual(num_t *A, num_t *b, num_t *x, int N)
{
	num_t *Ax = (num_t *)malloc(N * sizeof(num_t));
	num_t residual = 0.0;

	for (int i = 0; i < N; i++) {
		Ax[i] = 0.0;
		for (int j = 0; j < N; j++) {
			Ax[i] += A[i*N+j] * x[j];
		}
	}

	for (int i = 0; i < N; i++) {
		residual += (b[i] - Ax[i]) * (b[i] - Ax[i]);
	}

	free(Ax);

	return sqrt(residual);
}
#endif


inline void swap(num_t **a, num_t **b) {
	void *tmp = *a;
	*a = *b;
	*b = tmp;
}
int max_err_idx;

#ifdef NESTED_JACOBI
num_t jacobi_parallel(num_t *A, num_t *b, int N, int maxIter, num_t tol, num_t *x)
{
	int iter = 0;
	num_t diff;
	int tid = omp_get_thread_num();
	int threads = omp_get_num_threads();

	// Declaring the variables as static to be shared between threads
	static num_t error;
	static num_t *x_old;
	static num_t *x_new;
	static num_t *local_x_new;
	
	#pragma omp single
	{
		local_x_new = calloc(threads, sizeof(num_t));
	}
	#pragma omp single
	{
		x_old = calloc(N, sizeof(num_t));
	}
	#pragma omp single
	{
		x_new = x;
	}

	do {
		#pragma omp barrier
		#pragma omp single
		{
			error = 0.0;
		}
		for (int i = 0; i < N; i++) {
			local_x_new[tid] = 0;

			// Calculate local sums
			#pragma omp for schedule(static) nowait
			for (int j = 0; j < N; j++) {
				if (i != j)
					local_x_new[tid] -= A[i*N+j] * x_old[j];
			}

			// Add in the global sum
			#pragma omp single
			{
				x_new[i] = b[i];
			}
			#pragma omp atomic
			x_new[i] += local_x_new[tid];
			#pragma omp barrier
			// Divide by the diagonal element
			#pragma omp single
			{
				x_new[i] /= A[i*N+i];

				// Calculate the difference
				diff = fabs(x_old[i] - x_new[i]);
				if (diff > error) {
					error = diff;
					max_err_idx = i;
				}
			}
			#pragma omp barrier
		}
#ifdef JACOBI_MESSAGES
		#pragma omp single
		{
			// Print iter, residual, difference
			printf("iter = %d, error = %.e, max_err_idx=%d, residual = %.e\n", iter, error, max_err_idx, calculate_residual(A, b, x_new, N));
		}
#endif

		// Swap the pointers
		#pragma omp single
		swap(&x_old, &x_new);
		#pragma omp barrier
		// printf("Iter: %d, Thread %d: error = %.e\n", iter, tid, error);
	} while (++iter < maxIter && error > tol);

	#pragma omp single
	{
		// Free the memory
		if (x_old != x) {
			memcpy(x, x_old, N * sizeof(num_t));
			free(x_old);
		} else {
			free(x_new);
		}
	}
	#pragma omp barrier

	#pragma omp single
	{
		free(local_x_new);
	}

	return error;
}
#else
num_t jacobi_parallel(num_t *A, num_t *b, int N, int maxIter, num_t tol, num_t *x)
{
	int iter = 0;
	num_t diff;
	int threads = omp_get_num_threads();
	int tid = omp_get_thread_num();
	int block_size = 4;
	int err_idx = tid * block_size;
	
	// Declaring the variables as static to be shared between threads
	static num_t error;
	static num_t *x_old;
	static num_t *x_new;
	static num_t *local_error;

	#pragma omp single
	{
		x_old = calloc(N, sizeof(num_t));
	}
	#pragma omp single
	{
		local_error = calloc(threads, block_size*sizeof(num_t));
	}
	#pragma omp single
	{
		x_new = x;
		error = 0.0;
	}

	do {
		#pragma omp barrier
		local_error[err_idx] = 0.0;

		#pragma omp single
		{
			error = 0.0;
		}

		#pragma omp for schedule(static) nowait
		for (int i = 0; i < N; i++) {
			// Initialize x_new[i] with b[i]
			x_new[i] = b[i];

			// First part of the sum
			for (int j = 0; j < i; j++) {
				x_new[i] -= A[i*N+j] * x_old[j];
			}

			// Second part of the sum
			for (int j = i+1; j < N; j++) {
				x_new[i] -= A[i*N+j] * x_old[j];
			}

			// Divide by the diagonal element
			x_new[i] /= A[i*N+i];

			// Calculate the difference
			diff = fabs(x_old[i] - x_new[i]);
			if (diff > local_error[err_idx])
				local_error[err_idx] = diff;
		}

		if (local_error[err_idx] > error) {
			#pragma omp critical
			{
				if (local_error[err_idx] > error)
					error = local_error[err_idx];
			}
		}
		#pragma omp barrier
#ifdef JACOBI_MESSAGES
		#pragma omp single
		{
			// Print iter, residual, difference
			printf("iter = %d, error = %.e, residual = %.e\n", iter, error, calculate_residual(A, b, x_new, N));
		}
#endif
		#pragma omp single
		{
			// Swap the pointers
			swap(&x_old, &x_new);
		}
		// printf("Iter: %d, Thread %d: error = %.e\n", iter, tid, error);
	} while (++iter < maxIter && error > tol);

	#pragma omp single
	{
		// Free the memory
		if (x_old != x) {
			memcpy(x, x_old, N * sizeof(num_t));
			free(x_old);
		} else {
			free(x_new);
		}
	}
	#pragma omp barrier

	return error;
}
#endif


/* Serial implementation of Gaus Jacobi method. */
num_t jacobi_serial(num_t *A, num_t *b, int N, int maxIter, num_t tol, num_t *x)
{
	int iter = 0;
	num_t error, diff;
	
	num_t *x_new = calloc(N, sizeof(num_t));
	num_t *x_old = x;

	do {
		error = 0.0;
		for (int i = 0; i < N; i++) {
			x_new[i] = b[i];
			for (int j = 0; j < i; j++) {
				x_new[i] -= A[i*N+j] * x_old[j];
			}
			for (int j = i+1; j < N; j++) {
				x_new[i] -= A[i*N+j] * x_old[j];
			}
			x_new[i] /= A[i*N+i];
			diff = fabs(x_old[i] - x_new[i]);
			if (diff > error)
				error = diff;
		}
#ifdef JACOBI_MESSAGES
		// Print iter, residual, difference
		printf("iter = %d, error = %.e, residual = %.e\n", iter, error, calculate_residual(A, b, x_new, N));
#endif
		swap(&x_old, &x_new);
	} while (++iter < maxIter && error > tol);

	if (x_old != x) {
		memcpy(x, x_old, N * sizeof(num_t));
		free(x_old);
	} else {
		free(x_new);
	}

	return error;
}


