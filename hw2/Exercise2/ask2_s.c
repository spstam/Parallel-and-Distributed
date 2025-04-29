#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define M (50)
#define K (300000)
#define N (20)

void initA(long int *A, long int rows, long int cols) {
	long int i;
	long int j;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			A[i*cols+j]=i+j;
		}
	}
	return;
}

void initB(long int *B, long int rows, long int cols) {
	long int i;
	long int j;
	for(i=0;i<rows;i++){
		for(j=0;j<cols;j++){
			B[i*cols + j]=i*j;
		}
	}
	return;
}

void print2DArray(long int rows, long int cols, long int *array) {
	for (long int i = 0; i < rows; i++) {
		for (long int j = 0; j < cols; j++) {
			printf("%ld ", array[i*cols + j]);
		}
		printf("\n");
	}
}

int main(int argc,char *argv[]){
	// Dynamically allocate memory for matrix A[M][K]
	long int *A = (long int *)malloc(M*K * sizeof(long int));
	if (A == NULL) {
		printf("A Memory not allocated.\n");
		exit(0);
	}
	// Dynamically allocate memory for matrix B[K][N]
	long int *B = (long int *)malloc(K*N * sizeof(long int));
	if (B == NULL) {
		printf("B Memory not allocated.\n");
		exit(0);
	}
	// Dynamically allocate memory for matrix C[M][N]
	long int *C = (long int *)malloc(M*N * sizeof(long int));
	if (C == NULL) {
		printf("C Memory not allocated.\n");
		exit(0);
	}

	initA(A,M,K);
	initB(B,K,N);

	clock_t before = clock();
	
	//init A
	for(int i = 0; i < M; i++){
		for(int j = 0; j < K; j++){
			A[i*K + j] = i + j;
		}
	}

	//init B
	for(int i = 0; i < K; i++){
		for(int j = 0; j < N; j++){
			B[i*N + j] = i*j;
		}
	}

	for (long int i = 0; i < M; i++) {
		for (long int j = 0; j < N; j++) {
			C[i*N+j] = 0;
			for (long int k = 0; k < K; k++) {
				C[i*N + j] += A[i*K + k] * B[k*N + j];
			}
		}
	}

	clock_t difference = clock() - before;
	printf("Exec time: %lf\n", (double)difference / CLOCKS_PER_SEC);

	// printf("Matrix A:\n");
	// print2DArray(M, K, A);
	// printf("\n\n\n");

	// printf("Matrix B:\n");
	// print2DArray(K, N, B);
	// printf("\n\n\n");

	// printf("Matrix C:\n");
	// print2DArray(M, N, C);

	free(A);
	free(B);
	free(C);

	return 0;
}