#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Deine a structure to store parking record
typedef struct
{
    int entryHour, entryMinute;
    int exitHour, exitMinute;
    int lotIndex;
    int permitNumber;
} ParkingRecord;

// Define a structure to store entry and exit event
typedef struct
{
    int time;
    int type;
} Event;

// Function to parse the input file and load the parking records into memory
ParkingRecord *loadParkingData(const char *filename, int *recordCount)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        exit(1);
    }

    // Read the number of records
    fscanf(file, "%d", recordCount);

    // Dynamically allocate memory for parking records
    ParkingRecord *records = (ParkingRecord *)malloc((*recordCount) * sizeof(ParkingRecord));

    // Read each parking record
    for (int i = 0; i < *recordCount; i++)
    {
        fscanf(file, "%d:%d %d:%d %d %d",
               &records[i].entryHour, &records[i].entryMinute,
               &records[i].exitHour, &records[i].exitMinute,
               &records[i].lotIndex, &records[i].permitNumber);
    }

    fclose(file);
    return records;
}

// Function to calculate parking duration in minutes
int calculateDuration(ParkingRecord record)
{
    int entryTime = record.entryHour * 60 + record.entryMinute;
    int exitTime = record.exitHour * 60 + record.exitMinute;
    return exitTime - entryTime;
}

// General comparator function type, compare two parking records
typedef int (*comparator)(const void *, const void *);

// Define comparator compare based on permit number
int compareByPermitNumber(const void *a, const void *b)
{
    ParkingRecord *recordA = (ParkingRecord *)a;
    ParkingRecord *recordB = (ParkingRecord *)b;

    return recordA->permitNumber - recordB->permitNumber;
}

// Define comparator compare integer (used for sorting duration)
int compareInt(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Define comparator compare time in Event structure
int compareEventByTime(const void *a, const void *b)
{
    Event *eventA = (Event *)a;
    Event *eventB = (Event *)b;
    return eventA->time - eventB->time;
}

// Merge sort algorithm, pass comparator to the function so function can sort based on different attribute
// Function to merge two half of the array
void merge(void *arr, int left, int mid, int right, size_t elementSize, comparator comp)
{
    int i, j, k;
    int n1 = mid - left + 1; // number of elements in left half
    int n2 = right - mid;    // number of elements in right half

    // Alliocate memory to temporary arrays to hold the two halves
    void *L = malloc(n1 * elementSize);
    void *R = malloc(n2 * elementSize);

    // Copy data to temp array L[], R[]
    memcpy(L, (char *)arr + left * elementSize, n1 * elementSize);
    memcpy(R, (char *)arr + (mid + 1) * elementSize, n2 * elementSize);

    // Merge two halves back into arr[]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (comp((char *)L + i * elementSize, (char *)R + j * elementSize) <= 0) // if element in left array is smaller than element in right array
        {
            memcpy((char *)arr + k * elementSize, (char *)L + i * elementSize, elementSize);
            i++;
        }
        else
        {
            memcpy((char *)arr + k * elementSize, (char *)R + j * elementSize, elementSize);
            j++;
        }
        k++;
    }

    // Copy any remaining elements from L[]
    while (i < n1)
    {
        memcpy((char *)arr + k * elementSize, (char *)L + i * elementSize, elementSize);
        i++;
        k++;
    }

    // Copy any remaining elements from R[]
    while (j < n2)
    {
        memcpy((char *)arr + k * elementSize, (char *)R + j * elementSize, elementSize);
        j++;
        k++;
    }

    // Free temporary arrays
    free(L);
    free(R);
}

// Function to implement merge sort
void mergeSort(void *arr, int left, int right, size_t elementSize, comparator comp)
{
    if (left < right)
    {
        // Find the middle point
        int mid = left + (right - left) / 2;

        // Recursively sort the two halves
        mergeSort(arr, left, mid, elementSize, comp);
        mergeSort(arr, mid + 1, right, elementSize, comp);

        // Merge the two sorted halves
        merge(arr, left, mid, right, elementSize, comp);
    }
}

// Function to print duplicate permits (Task p)
void findDuplicatePermit(ParkingRecord *records, int recordCount)
{
    // Sort the records by permit number using mergeSort
    mergeSort(records, 0, recordCount - 1, sizeof(ParkingRecord), compareByPermitNumber);

    // Iterate through start number and find the duplicate
    for (int i = 0; i < recordCount; i++)
    {
        if (records[i].permitNumber == records[i + 1].permitNumber)
        {
            printf("%d %d %d:%d %d:%d\n",
                   records[i].permitNumber, records[i].lotIndex,
                   records[i].entryHour, records[i].entryMinute,
                   records[i].exitHour, records[i].exitMinute);

            // Loop to print all records with the same permit number
            while (i < recordCount && records[i].permitNumber == records[i + 1].permitNumber)
            {
                i++;
                printf("%d %d %d:%d %d:%d\n",
                       records[i].permitNumber, records[i].lotIndex,
                       records[i].entryHour, records[i].entryMinute,
                       records[i].exitHour, records[i].exitMinute);
            }
        }
    }
}

// Function to calculate the median duaration of each parking lot (Task d)
void calculateMedianDuration(ParkingRecord *records, int recordCount)
{
    // Define a 2D array to store duration, each parking lot occupy a row
    const int max_lot = 100;
    int durationRecord[max_lot][recordCount];
    int durationCount[max_lot];

    // Initialize durationCount to 0 using a loop
    for (int i = 0; i < max_lot; i++)
    {
        durationCount[i] = 0;
    }

    // Group the records by parking lot
    for (int i = 0; i < recordCount; i++)
    {
        int indexNumber = records[i].lotIndex;

        // Ensure the lotIndex is within bounds
        if (indexNumber >= 0 && indexNumber < max_lot)
        {
            durationRecord[indexNumber][durationCount[indexNumber]] = calculateDuration(records[i]);
            durationCount[indexNumber]++;
        }
        else
        {
            printf("Error: Lot index %d is out of bounds.\n", indexNumber);
        }
    }

    // For each parking lot, sort the duration and find the median
    for (int lot = 0; lot < max_lot; lot++)
    {
        if (durationCount[lot] > 0)
        {
            // Sort the duration for each parking lot
            mergeSort(durationRecord[lot], 0, durationCount[lot] - 1, sizeof(int), compareInt);

            // Calculate the median
            float median;
            int n = durationCount[lot];
            if (n % 2 == 1)
            {
                // Odd number of elements, median is the middle element
                median = durationRecord[lot][n / 2];
            }
            else
            {
                // Even number of elements, median is the mean of the two middle elements
                median = (durationRecord[lot][n / 2 - 1] + durationRecord[lot][n / 2]) / 2.0;
            }

            // Print the result for the lot
            printf("%d %.6f\n", lot, median);
        }
    }
}

// Function to calculate maximum occupacy (Task o)
int maximumOccupacy(ParkingRecord *records, int recordCount)
{
    Event events[recordCount * 2];
    int eventCount = 0;

    // Conver parking records into entry and exit events
    for (int i = 0; i < recordCount; i++)
    {
        int entryTime = records[i].entryHour * 60 + records[i].entryMinute;
        int exitTime = records[i].exitHour * 60 + records[i].exitMinute;

        // Create an entry event
        events[eventCount].time = entryTime;
        events[eventCount].type = +1;
        eventCount++;

        // Create an exit event
        events[eventCount].time = exitTime;
        events[eventCount].type = -1;
        eventCount++;
    }

    // Sort event by time, with exit before entries if times are equal
    mergeSort(events, 0, eventCount - 1, sizeof(Event), compareEventByTime);

    // Traverse through the events and track te current occupacy
    int currentOccupacy = 0;
    int maximumOccupacy = 0;

    for (int i = 0; i < eventCount; i++)
    {

        // Update occupacy count based on whether it's an entry or and exit
        currentOccupacy += events[i].type;

        // Track the maximum occupacy
        if (currentOccupacy > maximumOccupacy)
        {
            maximumOccupacy = currentOccupacy;
        }
    }

    // Print the maximum occupacy
    printf("%d\n", maximumOccupacy);
}

int main(int argc, char *argv[])
{
    // Load parking data from the file
    int recordCount;
    ParkingRecord *records = loadParkingData(argv[1], &recordCount);

    // Handle different tasks based on the command line argument
    char option = argv[2][0];
    switch (option)
    {
    case 'p':
        findDuplicatePermit(records, recordCount);
        break;
    case 'd':
        calculateMedianDuration(records, recordCount);
        break;
    case 'o':
        maximumOccupacy(records, recordCount);
        break;
    }

    // Free dynamically allocated memory
    free(records);
    return 0;
}