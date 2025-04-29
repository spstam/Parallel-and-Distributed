
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "mpi.h"

#define  MASTER 0
#define SAMPLES 1000000

bool is_sorted(int *arr, int n);
int compare_exchange(int *a, int *b);
int *comparesplit_merge(int *sorted, int *arr, int n, bool max);
int oddevenser_phase(int *array, int n, int phase);
int *oddevenpar(int taskid, int numtasks, int task_n, int *sorted);

int main (int argc, char *argv[])
{
	int   numtasks, taskid, len, task_n, n;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	double tstart, tend;

	n = atoi(argv[1]);
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Get_processor_name(hostname, &len);

	if (taskid == MASTER) {
		printf("Processor name: %s. Number of MPI tasks: %d\n",
											hostname, numtasks);
	}

	task_n = n / numtasks;
	// Initialize an array of size n with random numbers
	int *unsorted = (int *)malloc(task_n * sizeof(int));
	int *sorted = (int *)malloc(task_n * sizeof(int));
	srand(time(NULL) + taskid);
	for (int i = 0; i < task_n; i++) {
		unsorted[i] = rand() % 10000;
	}

	#ifdef DEBUG
	// Print the unsorted array
	printf("Task %d - Unsorted array:\n", taskid);
	for (int i = 0; i < task_n; i++) {
		printf("%4d ", unsorted[i]);
	}
	printf("\n");
	#endif

	MPI_Barrier(MPI_COMM_WORLD);

	if (taskid == MASTER) {
		tstart = MPI_Wtime();
	}

	// Sort the local array
	oddevenser(unsorted, task_n, sorted);
	
	// assert(is_sorted(sorted, task_n));

	// Compare-split the sorted arrays
	sorted = oddevenpar(taskid, numtasks, task_n, sorted);
	
	MPI_Barrier(MPI_COMM_WORLD);
	if (taskid == MASTER) {
		tend = MPI_Wtime();
		printf("Took %lf seconds\n", tend - tstart);
	}

	int *global_sorted = NULL;
	if (taskid == MASTER) {
		global_sorted = (int *)malloc(n * sizeof(int));
	}
	MPI_Gather(sorted, task_n, MPI_INT, global_sorted, task_n, MPI_INT, MASTER, MPI_COMM_WORLD);

	if (taskid == MASTER) {
		assert(is_sorted(global_sorted, n));
		// Print the sorted array
		printf("Sorted array correctly\n");
		#ifdef DEBUG
		for (int i = 0; i < n; i++) {
			printf("%4d ", global_sorted[i]);
		}
		printf("\n");
		#endif
		free(global_sorted);
	}

	MPI_Finalize();
}

// Odd-even parallel sort
int *oddevenpar(int taskid, int numtasks, int task_n, int *sorted)
{
	int *temp = (int*)malloc(task_n * sizeof(int));

	for (int i = 0; i < numtasks; i++) {
		MPI_Barrier(MPI_COMM_WORLD);
		if (i % 2 == 0) { // even
			if (taskid % 2 == 0) {
				if (taskid + 1 < numtasks) {
					MPI_Send(sorted, task_n, MPI_INT, taskid + 1, 0, MPI_COMM_WORLD);
					MPI_Recv(temp, task_n, MPI_INT, taskid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					int *temp_new_sorted = comparesplit_merge(sorted, temp, task_n, false);
					free(sorted);
					sorted = temp_new_sorted;
				}
			} else if (taskid - 1 >= 0) {
				MPI_Recv(temp, task_n, MPI_INT, taskid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(sorted, task_n, MPI_INT, taskid - 1, 0, MPI_COMM_WORLD);
				int *temp_new_sorted = comparesplit_merge(sorted, temp, task_n, true);
				free(sorted);
				sorted = temp_new_sorted;
			}
		} else {
			if (taskid % 2 == 0) {
				if (taskid - 1 >= 0) {\
					MPI_Recv(temp, task_n, MPI_INT, taskid - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Send(sorted, task_n, MPI_INT, taskid - 1, 0, MPI_COMM_WORLD);
					int *temp_new_sorted = comparesplit_merge(sorted, temp, task_n, true);
					free(sorted);
					sorted = temp_new_sorted;
				}
			} else if (taskid + 1 < numtasks) {
				MPI_Send(sorted, task_n, MPI_INT, taskid + 1, 0, MPI_COMM_WORLD);
				MPI_Recv(temp, task_n, MPI_INT, taskid + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				int *temp_new_sorted = comparesplit_merge(sorted, temp, task_n, false);
				free(sorted);
				sorted = temp_new_sorted;
			}
		}
	}
	free(temp);
	return sorted;
}


// Keep n/2 max or min values of both arrays in sorted
int *comparesplit_merge(int *sorted, int *arr, int n, bool max)
{
	int s_i, a_i;
	int *temp = (int *)malloc(n * sizeof(int));
	if (!max) {
		s_i = a_i = 0;
		while (s_i + a_i < n) {
			// No need to check if s_i or a_i is out of bounds
			if (sorted[s_i] < arr[a_i]) {
				temp[s_i + a_i] = sorted[s_i];
				s_i++;
			} else {
				temp[s_i + a_i] = arr[a_i];
				a_i++;
			}
		}
	} else {
		s_i = a_i = n - 1;
		while (s_i + a_i > n - 2) {
			// No need to check if s_i or a_i is out of bounds
			if (sorted[s_i] > arr[a_i]) {
				temp[s_i + a_i - (n - 2) - 1] = sorted[s_i];
				s_i--;
			} else {
				temp[s_i + a_i - (n - 2) - 1] = arr[a_i];
				a_i--;
			}
		}
	}
	return temp;
}

void oddevenser(int *unsorted, int n, int *sorted)
{
	int phase, i;
	int changes = 0;
	bool prev_changes = true;

	memcpy(sorted, unsorted, n * sizeof(int));

	for (phase = 0; phase < n; phase++) {
		changes = oddevenser_phase(sorted, n, phase);

		// Check for sorted array in early phase
		if (changes == 0) {
			if (prev_changes == false) {
				break;
			} else {
				prev_changes = false;
			}
		} else {
			prev_changes = true;
		}	
	}
}

int oddevenser_phase(int *array, int n, int phase)
{
	int changes = 0;
	for (int i = phase % 2; i + 1 < n; i += 2) {
		changes += (int)compare_exchange(&array[i], &array[i + 1]);
	}
	return changes;
}

int compare_exchange(int *a, int *b)
{
	if (*a > *b) {
		int t = *a;
		*a = *b;
		*b = t;
		return 1;
	}
	return 0;
}

bool is_sorted(int *arr, int n)
{
	for (int i = 0; i + 1 < n; i++) {
		if (arr[i] > arr[i + 1]) {
			return false;
		}
	}
	return true;
}