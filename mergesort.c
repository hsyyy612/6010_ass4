#include <stdio.h>
#include <stdlib.h>

// Function to merge two half of the array
void merge(int arr[], int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;   //决定左半部分的array会有多少个元素
    int n2 = right - mid;      // 决定右半部分的array会有多少个元素

    // Temporary arrays to hold the two halves
    int L[n1], R[n2];

    // Copy data to temp array L[], R[]
    for (i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }

    for (j = 0; j < right; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    // Merge two halves back into arr[]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy any remaining elements from L[]
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy any remaining elements from R[]
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Function to implement merge sort
void mergeSort(int arr[], int left, int right)
{
    if (left < right)
    {
        // Find the middle point
        int mid = left + (left + right) / 2;

        // Recursively sort the two halves
        mergeSort(arr, left, mid);
        mergeSort(arr, mid+1, right);

        // Merge the two sorted halves
        merge(arr, left, mid, right);
    }
}