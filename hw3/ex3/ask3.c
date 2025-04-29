#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void rowMVMult(int n, double *localA, double *localb, double *localy, MPI_Comm comm) {
  int i, j, rank, size;
  MPI_Comm_rank(comm, &rank);
  MPI_Comm_size(comm, &size);

  int local_n = n / size; 

  for (i = 0; i < local_n; i++) {
    localy[i] = 0.0;
  }

  for (i = 0; i < local_n; i++) {
    for (j = 0; j < n; j++) {
      localy[i] += localA[i * n + j] * localb[j]; 
    }
  }
}

int main(int argc, char *argv[]) {
  int i, j, n, rank, size;
  double *A, *b, *y, *localA, *localb, *localy;
  double start_time, end_time;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    scanf("%d", &n);
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int local_n = n / size;
    
  localA = (double *)malloc(local_n * n * sizeof(double));
  localb = (double *)malloc(n * sizeof(double));
  localy = (double *)malloc(local_n * sizeof(double));

  if (rank == 0) {
    A = (double *)malloc(n * n * sizeof(double));
    y = (double *)malloc(n * sizeof(double));

    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        A[i * n + j] = (i + j) * 10.0;
      }
      localb[i] = 10.0;
    }

    MPI_Scatter(A, local_n * n, MPI_DOUBLE, localA, local_n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Bcast(localb, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    start_time = MPI_Wtime(); 
  } else {
    MPI_Scatter(NULL, local_n * n, MPI_DOUBLE, localA, local_n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // printf("test %d\n",rank);
    MPI_Bcast(localb, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  MPI_Barrier(MPI_COMM_WORLD); 
  rowMVMult(n, localA, localb, localy, MPI_COMM_WORLD);

  if (rank == 0) {
        MPI_Gather(localy, local_n, MPI_DOUBLE, y, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    } else {
        MPI_Gather(localy, local_n, MPI_DOUBLE, NULL, 0, MPI_DOUBLE, 0, MPI_COMM_WORLD); 
    }

  if (rank == 0) {
    end_time = MPI_Wtime();

    // printf("Resulting vector y:\n");
    // for (i = 0; i < n; i++) {
    //   printf("%f ", y[i]);
    // }
    // printf("\n");

    // printf("Execution time: %f seconds\n", end_time - start_time);
    printf("%lf", end_time-start_time);
    free(A);
    free(y);
  }

  free(localA);
  free(localb);
  free(localy);

  MPI_Finalize();
  return 0;
}