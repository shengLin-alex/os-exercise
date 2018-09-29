#include "pid_manger.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    // initial pid map
    int map = allocate_map();
    if (map == 1)
    {
        printf("Pid map initialized.\n");

        int i = 0;

        // create 15 process to test.
        while (i < 15)
        {
            int pid = allocate_pid();
            printf("Process: %d; Pid: %d\n", i + 1, pid);
            i++;
        }

        // release pid 304
        release_pid(304);
        printf("pid 304 released.\n");

        release_pid(299);

        release_pid(309);
        printf("pid 309 released.\n");

        // this pid will be 304
        int pidOne = allocate_pid();
        printf("Process: %d; Pid: %d\n", ++i, pidOne);

        // this pid will be 309
        int pidTwo = allocate_pid();
        printf("Process: %d; Pid: %d\n", ++i, pidTwo);

        // this pid will be 315
        int pidThree = allocate_pid();
        printf("Process: %d; Pid: %d\n", ++i, pidThree);
    }
    else
    {
        printf("Fail to initializ pid map.\n");
    }

    if (pidMap)
    {
        free(pidMap);
    }

    return 0;
}