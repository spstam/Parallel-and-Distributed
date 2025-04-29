#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

// #define VERBOSE
// Function that calculates the max value of an array
float find_max_value(float *a, int n){
	float max = a[0];
	for(int i=1; i<n; i++){
		if(a[i] > max){
			max = a[i];
		}
	}
	return max;
}

// Function that computes the 1-norm of the 1D array a of length n without unrolling
float matrix_norm_inf_unroll_1(float *a, int n)
{
	float max;
	float row_sums[n];
	
	// Initialize the column sums to 0
	for(int i=0; i<n; i++){
		row_sums[i] = 0.0;
	}

	// Compute the inf-norm of the 1D array a of length n
	for (int row = 0; row < n; row++) {
		for(int col=0; col < n; col++){
			row_sums[row] += fabsf(a[row*n + col]);
		}
	}

	// Find the maximum value in the column sums
	max = find_max_value(row_sums, n);
	return max;
}

// Function that computes the inf-norm of the 1D array a of length n with unrolling factor 2
float matrix_norm_inf_unroll_2(float *a, int n)
{
#define K (2)
	float max;
	float row_sums[n];

	// Initialize the column sums to 0
	for(int i=0; i<n; i++){
		row_sums[i] = 0.0;
	}

	// Compute the inf-norm of the 1D array a of length n
	for (int row = 0; row< n; row++) {
		int col = 0;
		for (; col+K-1 < n; col += K){
			row_sums[row] += fabsf(a[row*n + col]);
			row_sums[row] += fabsf(a[row*n + col+1]);
		}
		// For when n%K != 0
		if (col< n) {
			row_sums[row] += fabsf(a[row*n + col]);
		}
	}

	// Find the maximum value in the column sums
	max = find_max_value(row_sums, n);
	return max;
#undef K
}

// Function that computes the inf-norm of the 1D array a of length n with unrolling factor 4
float matrix_norm_inf_unroll_4(float *a, int n)
{
#define K (4)
	float max;
	float row_sums[n];

	// Initialize the column sums to 0
	for(int i=0; i<n; i++){
		row_sums[i] = 0.0;
	}

	// Compute the inf-norm of the 1D array a of length n
	for (int row = 0; row< n; row++) {
		int col = 0;
		for (; col+K-1 < n; col += K){
			row_sums[row] += fabsf(a[row*n + col]);
			row_sums[row] += fabsf(a[row*n + col+1]);
			row_sums[row] += fabsf(a[row*n + col+2]);
			row_sums[row] += fabsf(a[row*n + col+3]);
		}
		// For when n%K != 0
		for (; col< n; col++) {
			row_sums[row] += fabsf(a[row*n + col]);
		}
	}
		
	// Find the maximum value in the column sums
	max = find_max_value(row_sums, n);
	return max;
#undef K
}

// Function that computes the inf-norm of the 1D array a of length n with unrolling factor 8
float matrix_norm_inf_unroll_8(float *a, int n)
{
#define K (8)
	float max;
	float row_sums[n];

	// Initialize the column sums to 0
	for(int i=0; i<n; i++){
		row_sums[i] = 0.0;
	}

	// Compute the inf-norm of the 1D array a of length n
	for (int row = 0; row < n; row++) {
		int col = 0;
		for (; col+K-1 < n; col += K){
			row_sums[row] += fabsf(a[row*n + col]);
			row_sums[row] += fabsf(a[row*n + col+1]);
			row_sums[row] += fabsf(a[row*n + col+2]);
			row_sums[row] += fabsf(a[row*n + col+3]);
			row_sums[row] += fabsf(a[row*n + col+4]);
			row_sums[row] += fabsf(a[row*n + col+5]);
			row_sums[row] += fabsf(a[row*n + col+6]);
			row_sums[row] += fabsf(a[row*n + col+7]);
		}
		// For when n%K != 0
		for (; col < n; col++) {
			row_sums[row] += fabsf(a[row*n + col]);
		}
	}
		
	// Find the maximum value in the column sums
	max = find_max_value(row_sums, n);
	return max;
#undef K
}

// Function that computes the inf-norm of the 1D array a of length n with unrolling factor 16
float matrix_norm_inf_unroll_16(float *a, int n)
{
#define K (16)
	float max;
	float row_sums[n];

	// Initialize the column sums to 0
	for(int i=0; i<n; i++){
		row_sums[i] = 0.0;
	}

	// Compute the inf-norm of the 1D array a of length n
	for (int row = 0; row < n; row++) {
		int col = 0;
		for (; col+K-1 < n; col += K){
			row_sums[row] += fabsf(a[row*n + col]);
			row_sums[row] += fabsf(a[row*n + col+1]);
			row_sums[row] += fabsf(a[row*n + col+2]);
			row_sums[row] += fabsf(a[row*n + col+3]);
			row_sums[row] += fabsf(a[row*n + col+4]);
			row_sums[row] += fabsf(a[row*n + col+5]);
			row_sums[row] += fabsf(a[row*n + col+6]);
			row_sums[row] += fabsf(a[row*n + col+7]);
			row_sums[row] += fabsf(a[row*n + col+8]);
			row_sums[row] += fabsf(a[row*n + col+9]);
			row_sums[row] += fabsf(a[row*n + col+10]);
			row_sums[row] += fabsf(a[row*n + col+11]);
			row_sums[row] += fabsf(a[row*n + col+12]);
			row_sums[row] += fabsf(a[row*n + col+13]);
			row_sums[row] += fabsf(a[row*n + col+14]);
			row_sums[row] += fabsf(a[row*n + col+15]);
		}
		// For when n%K != 0
		for (; col < n; col++) {
			row_sums[row] += fabsf(a[row*n + col]);
		}
	}
		
	// Find the maximum value in the column sums
	max = find_max_value(row_sums, n);
	return max;
#undef K
}

// Number of supported unroll factors
#define NO_SUPPORTED (5)
// Maximum unroll factors supported
int supported_unroll_factors[] = {1, 2, 4, 8, 16};
// Function pointer matrix with the function pointers for the 1D array functions
float (*matrix_norm_inf[NO_SUPPORTED])(float *, int) = {
	matrix_norm_inf_unroll_1,
	matrix_norm_inf_unroll_2,
	matrix_norm_inf_unroll_4,
	matrix_norm_inf_unroll_8,
	matrix_norm_inf_unroll_16
};

// Function that initializes the 1D array
float *init_array(int n)
{
	float *a = (float *)malloc(n * n * sizeof(float));
	for (int i = 0; i < n * n; i++) {
		a[i] = 1.0;
	}

	return a;
}

// Main function of the program
int main(int argc, char *argv[])
{
	int n = 8;
	int executions_per_version = 1;

	if (argc > 2) {
		executions_per_version = atoi(argv[1]);
		n = atoi(argv[2]);
	} else {
		printf("Usage: %s <executions_per_version> <n>\n", argv[0]);
		exit(1);
	}

	// Initialize the 1D array
	float *a = init_array(n);

	// Array of execution times
	float exec_time_mean[NO_SUPPORTED];
	float exec_time_std[NO_SUPPORTED];

	for (int i = 0; i < NO_SUPPORTED; i++) {
		float exec_times[executions_per_version];
		// Warm-up
		matrix_norm_inf[i](a, n);

		for (int j = 0; j < executions_per_version; j++) {
			clock_t start = clock(), diff;
			float result = matrix_norm_inf[i](a, n);
			diff = clock() - start;
			exec_times[j] = (float)((diff * 1000) / (float)CLOCKS_PER_SEC);
			assert(result == (float)n);
#ifdef VERBOSE
			printf("Execution time for version %d: %.2f ms\n", supported_unroll_factors[i], exec_times[j]);
#endif
		}

		// Calculate the mean and standard deviation of the execution times
		exec_time_mean[i] = 0.0;
		for (int j = 0; j < executions_per_version; j++) {
			exec_time_mean[i] += exec_times[j];
		}
		exec_time_mean[i] /= (float)executions_per_version;
		
		exec_time_std[i] = 0.0;
		for (int j = 0; j < executions_per_version; j++) {
			exec_time_std[i] += (exec_times[j] - exec_time_mean[i]) * (exec_times[j] - exec_time_mean[i]);
		}
		exec_time_std[i] /= executions_per_version;
		exec_time_std[i] = sqrt(exec_time_std[i]);

	}

	// Check if directory results exists
	system("mkdir -p results");

	// Print the results in a file
	char filename[100];
	sprintf(filename, "results/matrix_norm_inf_%d.dat", n);
	FILE *fp = fopen(filename, "w");
	assert(fp != NULL);
	fprintf(fp, "K, Mean, Std\n");
	for (int i = 0; i < NO_SUPPORTED; i++) {
		fprintf(fp, "%d, %f, %f\n", supported_unroll_factors[i], exec_time_mean[i], exec_time_std[i]);
	}
	fclose(fp);

#ifdef VERBOSE
	// Print the results in the console
	printf("\nK, Mean, Std\n");
	for (int i = 0; i < NO_SUPPORTED; i++) {
		printf("%d, %.2f, %.2f\n", supported_unroll_factors[i], exec_time_mean[i], exec_time_std[i]);
	}
#endif

	return 0;
}