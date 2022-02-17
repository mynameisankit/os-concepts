#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<sys/shm.h>
#include<sys/stat.h>        /* For mode constants */
#include<fcntl.h>           /* For O_* constants */

int main(int argc, char *argv[]) {
    /**
     * This version is an extension to the collatz conjecture via child
     * 
     * Establish a shared memory using shm_open() and then write the 
     * elements in the seqeunce to that shared memory 
     * and wait using the wait() until the child terminates and then output
     * the sequence
     * 
     * Useful Functions:-
     * ftruncate() -> configure the size of the shared memory object
     * mmap() -> Establish a memory-mapped file contained the shared memory object
     * 
     * Note - Link with "-lrt" while compiling
     * */

    if(argc <= 1) {
        printf("Please provide the value of n\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    if(n <= 0) {
        printf("Please pass a positive integer\n");
        exit(1);
    }

    pid_t pid = fork();

    //Name and Size (in bytes) of shared memory object
    const int SIZE = 32000;
    const char *name = "collatz";

    //Pointer and file descriptor of shared memory object
    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);
    //Configure the size of shared memory object
    ftruncate(shm_fd, SIZE);
    //Memory map the shared memory object
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    //Child Process
    if(pid == 0) {
        const int BUFFER_SIZE = 100;

        char buffer[BUFFER_SIZE];

        while(n != 1) {
            if(n % 2 == 0)
                n = n/2;
            else
                n = 3*n + 1;

            //Conver n to string
            sprintf(buffer, "%d", n);
            //Write to shared memory
            sprintf(ptr, "%s", buffer);
            //Increment the pointer
            ptr += strlen(buffer);

            //Pad with one space
            sprintf(buffer, " ");
            sprintf(ptr, "%s", buffer);
            ptr += strlen(buffer);
        }

        exit(0);
    }
    //Parent Process
    else {
        //Wait for the child process to terminate
        wait(NULL);

        /**
         * Read from the shared memory after child has finished executing
         * Resolves producer-consumer problem by synchronization through wait() sys call
         * */
        printf("%s\n", (char *)ptr);
    }

    return 0;
}