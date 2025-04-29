#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define M (50)
#define K (300000)
#define N (20)

void print2DArray(int rows, int cols, int array[rows][cols]) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}
}

int main(int argc,char *argv[])
{
	// Dynamically allocate memory for matrix A[M][K]
	int *A = (int *)malloc(M*K * sizeof(int));
	
	// Dynamically allocate memory for matrix B[K][N]
	int *B = (int *)malloc(K*N * sizeof(int));

	// Dynamically allocate memory for matrix C[M][N]
	int *C = (int *)malloc(M*N * sizeof(int));
	
	int tid;
	if (argc!=3){
		printf("Wrong args. Correct input:\nProgram_name <nthreads> <size of chunks>\n");
		return 0;
	}
	int nthreads=atoi(argv[1]);
	int chunk=atoi(argv[2]);
	omp_set_num_threads(nthreads);
	double thread_end_time, thread_start_time;

	double times[nthreads];// times of execution per thread
	int calcs[nthreads];//no of flop
	double start_all=omp_get_wtime();
	double sum_times=0;

	//init time and flops arrays
	for (int i = 0; i < nthreads; i++) {
		times[i] = 0;
		calcs[i] = 0;
	}

	//printf("Starting matrix-matrix multiplication example with %d threads, M= %d, Κ= %d, Ν= %d, chunk= %d\n",nthreads,M,K,N,chunk);

	#pragma omp parallel private(tid, thread_start_time, thread_end_time)
	{
		tid = omp_get_thread_num();
		#pragma omp sections
		{
			#pragma omp section
			{
				thread_start_time = omp_get_wtime();
				//init A
				for(int i = 0; i < M; i++){
					for(int j = 0; j < K; j++){
						A[i*K + j] = i + j;
					}
				}
				thread_end_time = omp_get_wtime();
				times[tid]+=thread_end_time-thread_start_time;
				calcs[tid]+=M*K;
			}

			#pragma omp section
			{
				thread_start_time = omp_get_wtime();
				//init B
				//printf("Thread %d initialised B\n",tid);
				for(int i = 0; i < K; i++){
					for(int j = 0; j < N; j++){
						B[i*N + j] = i*j;
					}
				}
				thread_end_time = omp_get_wtime();
				times[tid] += thread_end_time-thread_start_time;
				calcs[tid] += N*K;
			}

		}//end sections
		tid = omp_get_thread_num();
		//printf("Thread %d is starting computations\n",tid);

		#pragma omp for schedule(dynamic, chunk)
		for (int i = 0; i < M; i++) {
			thread_start_time = omp_get_wtime();
			for (int j = 0; j < N; j++) {
				for (int k = 0; k < K; k++) {
					#if LEVEL == 3
					#pragma omp atomic
					#endif
					C[i*N + j] += A[i*K + k] * B[k*N + j];  // Perform the multiplication and accumulation
					calcs[tid] += 2;  // Each multiplication and addition is 2 FLOPs
				}
			}
			
			thread_end_time = omp_get_wtime();
			times[tid] += thread_end_time - thread_start_time;
		}
	} 
	double end_all=omp_get_wtime();

    //end of computations
    printf("Computations are done.\n");
    printf("***********************************\n");
    printf("Time per thread:\n");
    for(int i=0;i<nthreads;i++){
        printf("Time for thread %d = %lf sec. Number of FLOp = %d\n",i, times[i], calcs[i]);
        sum_times+=times[i];
    }
    // printf("%lf\n", end_all-start_all);
    printf("Total Parallel Time = %lf sec\n",end_all-start_all);
    printf("Wall Clock Time = %lf sec\n",sum_times);
    printf("************************************\n");

    // printf("Matrix A:\n");
    // print2DArray(M, K, A);
    // printf("\n\n");

    // printf("Matrix B:\n");
    // print2DArray(K, N, B);
    // printf("\n\n");

    // printf("Matrix C:\n");
    // print2DArray(M, N, C);

    // Free the top-level pointers
    free(A);
    free(B);
    free(C);

    return 0;
}