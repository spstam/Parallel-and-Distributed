#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ITERATIONS 1000

int main(int argc, char** argv) {
    int rank, size;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    double avint, avfloat, avdoubl;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Processor name: %s\n", processor_name);
        printf("Number of tasks: %d\n", size);
        printf("Hello. This is the master node.\n");
    }else{

        printf("Hello. This is node %d.\n", rank);
    }


    if (rank < 2) {
        MPI_Barrier(MPI_COMM_WORLD);

        double start_time, end_time;

        if (rank == 0) {
            int int_data = 42;
            float float_data = 42.42;
            double double_data = 42.4242;

            start_time = MPI_Wtime();
            for (int i = 0; i < NUM_ITERATIONS; i++) {
                MPI_Sendrecv(&int_data, 1, MPI_INT, 1, 0, &int_data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            end_time = MPI_Wtime();
            avint= (end_time - start_time) / NUM_ITERATIONS * 1e6;

            start_time = MPI_Wtime();
            for (int i = 0; i < NUM_ITERATIONS; i++) {
                MPI_Sendrecv(&float_data, 1, MPI_FLOAT, 1, 0, &float_data, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            end_time = MPI_Wtime();
            avfloat = (end_time - start_time) / NUM_ITERATIONS * 1e6;

            start_time = MPI_Wtime();
            for (int i = 0; i < NUM_ITERATIONS; i++) {
                MPI_Sendrecv(&double_data, 1, MPI_DOUBLE, 1, 0, &double_data, 1, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            end_time = MPI_Wtime();
            avdoubl = (end_time - start_time) / NUM_ITERATIONS * 1e6;

        } else if (rank == 1) {
            int int_data;
            float float_data;
            double double_data;

            for (int i = 0; i < NUM_ITERATIONS; i++) {
                MPI_Sendrecv(&int_data, 1, MPI_INT, 0, 0, &int_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            for (int i = 0; i < NUM_ITERATIONS; i++) {
                MPI_Sendrecv(&float_data, 1, MPI_FLOAT, 0, 0, &float_data, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            for (int i = 0; i < NUM_ITERATIONS; i++) {
                MPI_Sendrecv(&double_data, 1, MPI_DOUBLE, 0, 0, &double_data, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (rank==0){
        printf("Average time for int: %lf microseconds\n",avint);
        printf("Average time for float: %lf microseconds\n",avfloat);
        printf("Average time for double: %lf microseconds\n", avdoubl);
    }

    MPI_Finalize();
    return 0;
}
