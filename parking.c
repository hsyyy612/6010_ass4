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
ParkingRecord* loadParkingData(const char* filename, int* recordCount)
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

// Function to compare two integer for qsort
int compare(const void* a, const void* b)
{
    return (*(int*) a - *(int*) b);
}

// Function to compare two event for qsort
int compareEvent(const void *a, const void *b)
{
    Event *eventA = (Event *)a;
    Event *eventB = (Event *)b;

    // Sort by time first
    if (eventA->time != eventB->time)
    {
        return eventA->time - eventB->time;
    }

    // If time are the same, prioritize exits
    return eventA->type - eventB->type;
}

// Function to print duplicate permits (Task p)
void findDuplicatePermit(ParkingRecord* records, int recordCount)
{
    for (int i = 0; i < recordCount; i++)
    {
        for (int j = i + 1; j < recordCount; j++)
        {
            if (records[i].permitNumber == records[j].permitNumber)
            {
                printf("%d %d %d:%d %d:%d\n",
                       records[i].permitNumber, records[i].lotIndex,
                       records[i].entryHour, records[i].entryMinute,
                       records[i].exitHour, records[i].exitMinute);
                printf("%d %d %d:%d %d:%d\n",
                       records[j].permitNumber, records[j].lotIndex,
                       records[j].entryHour, records[j].entryMinute,
                       records[j].exitHour, records[j].exitMinute);
            }
        }
    }
}

// Function to calculate the median duaration of each parking lot (Task d)
void calculateMedianDuration(ParkingRecord *records, int recordCount)
{
    // Define a 2D array to store duration, each parking lot occupy a row
    int max_lot = 10;
    int durationRecord[max_lot][recordCount];
    int durationCount[max_lot];
    // Group the records by parking lot
    for (int i = 0; i < recordCount; i++)
    {
        int indexNumber = records[i].lotIndex;
        durationRecord[indexNumber][durationCount[indexNumber]] = calculateDuration(records[i]);
        durationCount[indexNumber]++;
    }

    // For each parking lot, sort the duration and find the median
    for (int lot = 0; lot < max_lot; lot++)
    {
        if (durationCount[lot] > 0)
        {
            // Sort the duration for each parking lot
            qsort(durationRecord[lot], durationCount[lot], sizeof(int), compare);

            // Calculate the median
            float median;
            int n = durationCount[lot];
            if (n%2 == 1)
            {
                // Odd number of elements, median is the middle element
                median = durationRecord[lot][n / 2];
            } else {
                // Even number of elements, median is the mean of the two middle elements
                median = (durationRecord[lot][n / 2 - 1] + durationRecord[lot][n / 2]) / 2;
            }

            // Print the result for the lot
            printf("%d %.6f", lot, median);
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
    qsort(events, eventCount, sizeof(Event), compareEvent);

    // Traverse through the events and track te current accupacy
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
    printf("%d", maximumOccupacy);
}

int main(int argc, char* argv[])
{
    // Load parking data from the file
    int recordCount;
    ParkingRecord *records = loadParkingData(argv[1], &recordCount);

    // Handle different tasks based on the command line argument
    char option = argv[2][0];
    switch(option)
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