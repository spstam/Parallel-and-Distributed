
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#define  MASTER 0
#define SAMPLES 1000000

int main (int argc, char *argv[])
{
	int   numtasks, taskid, len, meaning_of_life = 0;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	double tstart, tend;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Get_processor_name(hostname, &len);

	// A
	if (taskid == MASTER) {
		printf("Processor name: %s. Number of MPI tasks: %d\n", hostname, numtasks);
	}

	// B
	if (taskid == MASTER) {
		printf("Hello, this is the Master node.\n");
	} else {
		printf("Hello this is task %d\n", taskid);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	// C
	if (taskid == MASTER) {
		meaning_of_life = 42;
		printf("MASTER: Broadcasting meaning of life to all nodes %d times using Bcast.\n", SAMPLES);
		tstart = MPI_Wtime();
	}
	for (int i = 0; i < SAMPLES; i++) {
		MPI_Bcast(&meaning_of_life, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	}

	// E
	if (taskid == MASTER) {
		tend = MPI_Wtime();
		printf("MASTER: Took %lf seconds or %e seconds per time.\n", SAMPLES, tend - tstart, (tend - tstart) / SAMPLES);
	}
	#ifdef DEBUG 
	else {
		printf("Task %d received the meaning of life: %d\n", taskid, meaning_of_life);
	}
	#endif
	MPI_Barrier(MPI_COMM_WORLD);

	// D
	if (taskid == MASTER) {
		printf("MASTER: Broadcasting meaning of life to all nodes %d times using Send/Recv.\n", SAMPLES);
		tstart = MPI_Wtime();
	}
	for (int i = 0; i < SAMPLES; i++) {
		custom_bcast(&meaning_of_life, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	}

	// E
	if (taskid == MASTER) {
		tend = MPI_Wtime();
		printf("MASTER: Took %lf seconds or %e seconds per time.\n", SAMPLES, tend - tstart, (tend - tstart) / SAMPLES);
	}
	#ifdef DEBUG 
	else {
		printf("Task %d received the meaning of life: %d\n", taskid, meaning_of_life);
	}
	#endif
	MPI_Finalize();
}

void custom_bcast(void *data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) {
	int numtasks, taskid;
	MPI_Comm_size(communicator, &numtasks);
	MPI_Comm_rank(communicator, &taskid);
	MPI_Status status;

	if (taskid == root) {
		for (int i = 0; i < numtasks; i++) {
			if (i != root) {
				MPI_Send(data, count, datatype, i, 1, communicator);
			}
		}
	} else {
		MPI_Recv(data, count, datatype, root, 1, communicator, &status);
	}
	MPI_Barrier(MPI_COMM_WORLD);
}