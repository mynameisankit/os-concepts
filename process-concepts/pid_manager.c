#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MIN_PID 300
#define MAX_PID 5000

size_t allocate_map(size_t **map);
size_t allocate_pid(size_t **map);
size_t release_pid(size_t **map, size_t pid);

int main() {
    /**
     * How does Linux handles the creation of PIDs?
     * Linux uses a bitmap in which a value of 0 at position i indiciates
     * that a process id of value i is available and a value of 1 indicates that the 
     * process id is currently in use
     * 
     * Using Unsigned Int Array for simplicity 
     * */

    size_t *map;
    allocate_map(&map);

    size_t pid;

    pid = allocate_pid(&map);
    if(pid == 1)
        printf("Cannot Allocate PID\n");
    else
        printf("PID Allocated : %u\n", pid);

    pid = allocate_pid(&map);
    if(pid == 1)
        printf("Cannot Allocate PID\n");
    else
        printf("PID Allocated : %u\n", pid);

    release_pid(&map, pid);

    pid = allocate_pid(&map);
    if(pid == 1)
        printf("Cannot Allocate PID\n");
    else
        printf("PID Allocated : %u\n", pid);

    free(map);
    return 0;
}

size_t allocate_map(size_t **map) {
    *map = (size_t *)malloc((MAX_PID - MIN_PID + 1)*sizeof(size_t));
    
    if(*map == NULL)
        return 0;
    else
        return 1;
}

size_t allocate_pid(size_t **map) {
    //Get the first PID being unused
    for(int i = MIN_PID; i <= MAX_PID; i++) {
        if((*map)[i - MIN_PID] == 0) {
            (*map)[i - MIN_PID] = 1;
            return i;
        }
    }

    //Return 1 if all PIDs are being used currently
    return 1;
}

size_t release_pid(size_t **map, size_t pid) {
    //Check if PID is already being used
    if((*map)[pid - MIN_PID] == 0)
        return 1;
    //Release the PID if it is being used
    else {
        (*map)[pid - MIN_PID] = 0;
        return 0;
    }
}