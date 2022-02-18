#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>
#include<sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main() {
    /**
     * Function of program:-
     * The parent process will write to the pipe a string
     * The child process will reverse the case of each character of the string in the pipe
     * and resend it to the parent process
     * */

    //Read - https://stackoverflow.com/questions/24766013/is-it-really-necessary-to-close-the-unused-end-of-the-pipe-in-a-process#:~:text=Pipes%20are%20destined%20to%20be,some%20mess%20in%20sent%20messages.

    const int BUFFER_SIZE = 100;
    char msg[] = "Hi There!";
    int fd1[2], fd2[2];
    
    if(pipe(fd1) == -1) {
        printf("Cannot create pipe 1\n");
        return 1;
    }

    if(pipe(fd2) == -1) {
        printf("Cannot create pipe 2\n");
        return 1;
    }

    pid_t pid;
    pid = fork();

    if(pid < 1) {
        printf("Process creation failed\n");
        return 1;
    }
    //Parent process
    else if(pid > 0) {
        //Close the unused end of the pipe
        close(fd1[READ_END]);
        close(fd2[WRITE_END]);

        //Write the message
        write(fd1[WRITE_END], msg, strlen(msg) + 1);

        //Close the write end
        close(fd1[WRITE_END]);

        //Read the message from the second pipe
        char buffer[BUFFER_SIZE];
        read(fd2[READ_END], buffer, BUFFER_SIZE);
        close(fd2[READ_END]);

        printf("Original Message : %s\n", msg);
        printf("New Message : %s\n", buffer);

        //Wait for the child to terminate
        wait(NULL);
    }
    //Child process
    else {
        //Close the unused end of the pipe
        close(fd1[WRITE_END]);
        close(fd2[READ_END]);

        char buffer[BUFFER_SIZE];
        read(fd1[READ_END], buffer, BUFFER_SIZE);
        close(fd1[READ_END]);

        //Revese the case of each letter
        for(int i = 0; i < strlen(buffer); i++) {
            char curr = buffer[i];
            buffer[i] = isupper(curr) ? tolower(curr) : toupper(curr);
        }

        //Write the message to the second pipe
        write(fd2[WRITE_END], buffer, strlen(buffer) + 1);

        close(fd2[WRITE_END]);

        //Exit normally
        exit(0);
    }

    return 0;
}