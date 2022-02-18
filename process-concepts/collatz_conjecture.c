#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
    /**
     * What is a Collatz Conjecture?
     * n = {
     *      n/2 ; if n is even
     *      3n + 1; if n is odd
     * }
     * The sequence will eventually result in 1
     * 
     * The child process will output all the results and
     * the parent will wait for the child termination using wait() 
     * system call
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

    if(pid < 1) {
        printf("Process creation failed\n");
        return 1;
    }
    //Child Process
    else if(pid == 0) {
        printf("%d ", n);
        while(n != 1) {
            if(n % 2 == 0)
                n = n/2;
            else
                n = 3*n + 1;
            
            printf("%d ", n);
        }
        printf("\n");
        exit(0);
    }
    //Parent Process
    else {
        //Wait for the child process to terminate
        wait(NULL);
    }

    return 0;
}