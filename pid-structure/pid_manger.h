#ifndef PID_MANGER_H
#define PID_MANGER_H

#define MIN_PID 300
#define MAX_PID 5000
#define PID_RANGE 4701

#include <stdlib.h>

int* pidMap;

// Creates and initializes a data structure for representing pids;
// returns -1 if unsuccessful, 1 if successful
int allocate_map()
{
    pidMap = (int*)calloc(PID_RANGE, sizeof(int));

    if (pidMap)
    {
        return 1;
    }

    return -1;
}

// Allocates and returns a pid;
// return -1 if unable to allocate a pid (all pids are in use)
int allocate_pid()
{
    for (int i = 0; i < MAX_PID; i++)
    {
        if (pidMap[i] == 0)
        {
            pidMap[i] = 1;

            return i + MIN_PID;
        }
    }

    return -1;
}

// release a pid.
void release_pid(int pid)
{
    pidMap[pid - MIN_PID] = 0;
}

#endif