#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

/* Function to merge the two haves arr[l..m] and arr[m+1..r] of array arr[] */
void merge(int arr[], int l, int m, int r)
{

    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
#pragma omp for
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];

#pragma omp for
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    //printf("hier thread %d\n", omp_get_thread_num());
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

//find minimum of function
int min(int x, int y) { return (x < y) ? x : y; }

//merge sort function -iterative
void mergeSort(int arr[], int n)
{

    int curr_size; // For current size of subarrays to be merged
    // curr_size varies from 1 to n/2
    int left_start; // For picking starting index of left subarray
                    // to be merged

    // Merge subarrays in bottom up manner.  First merge subarrays of
    // size 1 to create sorted subarrays of size 2, then merge subarrays
    // of size 2 to create sorted subarrays of size 4, and so on.
    int c = 0;
    printf("hier\n");

    for (curr_size = 1; curr_size <= n - 1; curr_size = curr_size * 2)
    {

        // Pick starting point of different subarrays of current size
        for (left_start = 0; left_start < n - 1; left_start += 2 * curr_size)
        {
            // Find ending point of left subarray. mid+1 is starting
            // point of right

            int mid = min(left_start + curr_size - 1, n - 1);

            int right_end = min(left_start + 2 * curr_size - 1, n - 1);

            // Merge Subarrays arr[left_start...mid] & arr[mid+1...right_end]
            merge(arr, left_start, mid, right_end);
        }
    }
}

/* Function to print an array */
void printArray(int *arr, int size)
{

    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int testfunc(int32_t *arr, int n)
{
    //#pragma omp for schedule(dynamic, 4)
    for (int i = 0; i < n - 1; i++)
    {

        if (arr[i] > arr[i + 1])
        {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{

    double startTime = omp_get_wtime();
    int n = atoi(argv[1]);
    //sample array
    int32_t *arr = malloc(n * sizeof(int32_t));
    //int32_t *arr;
    //random numbers generator from stdlib
    time_t t;
    srand((unsigned)time(&t));
#pragma omp for schedule(dynamic, 4)
    for (int i = 0; i < n; i++)
    {
        int a = rand() % 100;
        arr[i] = a;
    }

    //call function to print array

    printf("Given array is \n");
    //printArray(arr, n);

    //call function to sort Array

    mergeSort(arr, n);

    //now call funcrtion to print sorted array again.
#pragma omp single
    {
        printf("\nSorted array is \n");
        //printArray(arr, n);
    }
    //simple "Test" to check if sorted array is sorted
#pragma omp barrier

#pragma omp master
    {
        if (testfunc(arr, n) == 0)
        {
            printf("test failed\n");
        }
        else
        {
            printf("test successfull\n");
        }
    }
    free(arr);

    double endTime = omp_get_wtime();
    printf("time: %2.4f seconds\n", endTime - startTime);
    return 0;
}