#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

//To run the program for 10 seconds
#include<signal.h>

int main() {
    /**
     * What is a zombie process?
     * It is a child process of some parent process, which has completed 
     * its execution and whose parent has not yet called wait() system call 
     * to obtain the status or has terminated and not yet been assigned 
     * to init process as parent and still has its entry in the process table
     * */

    pid_t pid = fork();

    if(pid < 1) {
        printf("Process creation failed\n");
        return 1;
    }
    //Child Process
    else if(pid == 0) {
        //Run the child process for atleast 10 seconds
        exit(0);
    }
    //Parent Process
    else {
        //Do not call the wait() system call
        //wait()

        //Print the child process ID
        printf("Child Process : %d\n", pid);

        //Run parent for 10 seconds
        sleep(10);
    }

    return 0;
}