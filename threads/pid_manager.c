#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define MIN_PID 300
#define MAX_PID 5000
#define NUM_THREADS 100

size_t *map;

typedef struct thread_info {
    size_t *pid;
    pthread_t *tid;
} thread_info;

size_t allocate_map(size_t **map);
size_t allocate_pid(size_t **map);
size_t release_pid(size_t **map, size_t pid);

void* thread_runner(void *param) {
    pthread_t *tid = (pthread_t *)param;
    size_t pid = allocate_pid(&map);

    printf("%lu PID engaged by the process %lu\n", pid, *tid);
    
    //Set maximum sleep to 5 secods
    sleep(rand()%5);

    release_pid(&map, pid);

    printf("%lu PID disengaged by the process %lu\n", pid, *tid);

    pthread_exit(0);
}

int main() {
    srand(time(0));

    /**
     * This is an extension to process concepts PID Manager
     * 
     * Note - Compile with -pthread
     * */

    allocate_map(&map);

    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++) {
        //Get the default Attributes
        pthread_attr_init(&attr[i]);

        //Create the process
        pthread_create(&threads[i], &attr[i], thread_runner, &threads[i]);

        printf("%d thread started\n", i + 1);
    }

    //Wait for the threads to finish
    for(int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

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