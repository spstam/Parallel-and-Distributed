#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main (int argc, char *argv[]) {
	int tid;

	/* Fork a team of threads giving them their own copies of variables */
	#pragma omp parallel private(tid)
	{
		double start=omp_get_wtime();
		tid=omp_get_thread_num();
		#pragma omp master
		printf("HELLO! I  am the Master thread. I created all participating threads.\n");
		
		#pragma omp single
		printf("OpenMP version: %d. Information provided by thread %d.\n", _OPENMP, tid);
		
		#pragma omp single
		printf("Number of proccessors = %d.\n", omp_get_num_procs());
		
		#pragma omp single
		printf("Number of threads = %d.\n", omp_get_num_threads());
		
		#pragma omp single
		printf("Max threads = %d.\n", omp_get_max_threads());
		
		#pragma omp single
		printf("Dynamic threads enabled? = %d\n", omp_get_dynamic());
		
		#pragma omp single
		printf("Nested parallelism levels supported = %d\n", omp_get_nested());
		
		#pragma omp single
		printf("In parallel? = %d\n", omp_in_parallel());

		double end = omp_get_wtime();
		printf("I am thread %d and worked for %lf msec\n",tid,end-start);
	}
	return 0;
}

