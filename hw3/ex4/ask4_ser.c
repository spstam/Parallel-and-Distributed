
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#define  MASTER 0
#define SAMPLES 1000000

bool is_sorted(int *arr, int n);
int compare_exchange(int *a, int *b);

int main (int argc, char *argv[])
{
	int n = atoi(argv[1]);
	
	// Initialize an array of size n with random numbers
	int *unsorted = (int *)malloc(n * sizeof(int));
	int *sorted = (int *)malloc(n * sizeof(int));
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		unsorted[i] = rand() % 100;
	}

	#ifdef DEBUG
	// Print the unsorted array
	printf("Unsorted array:\n");
	for (int i = 0; i < n; i++) {
		printf("%2d ", unsorted[i]);
	}
	printf("\n");
	#endif

	double tstart = clock();

	oddevenser(unsorted, n, sorted);

	double tend = clock();
	printf("Took %lf seconds\n", (tend - tstart) / CLOCKS_PER_SEC);

	assert(is_sorted(sorted, n));

	#ifdef DEBUG
	// Print the unsorted array
	printf("Sorted array:\n");
	for (int i = 0; i < n; i++) {
		printf("%2d ", sorted[i]);
	}
	printf("\n");
	#endif
}

void oddevenser(int *unsorted, int n, int *sorted)
{
	int phase, i;
	int changes = 0;
	bool prev_changes = true;

	memcpy(sorted, unsorted, n * sizeof(int));

	for (phase = 0; phase < n; phase++) {
		changes = 0;
		for (i = phase % 2; i + 1 < n; i += 2) {
			changes += (int)compare_exchange(&sorted[i], &sorted[i + 1]);
		}
		
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