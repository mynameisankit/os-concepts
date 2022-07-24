#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>

#define WRITE_END 1
#define READ_END 0

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("Please provide a source and destination file!\n");
        return 1;
    }
    else if(argc == 2) {
        printf("Please provide a destination file!\n");
        return 1;
    }

    const int BUFFER_SIZE = 1000;
    char dest[BUFFER_SIZE], src[BUFFER_SIZE];
    
    strcpy(dest, argv[2]);
    strcpy(src, argv[1]);

    int fd[2];
    if(pipe(fd) == -1) {
        printf("Cannot create pipe\n");
        return 1;
    }

    pid_t pid = fork();
    
    if(pid < 0) {
        printf("Cannot create process\n");
        return 1;
    }
    //Parent Process
    else if(pid > 0) {
        //Close the unused end of the pipe
        close(fd[READ_END]);

        //Open the source file and get its contents
        FILE *fptr = fopen(src, "r");
        if(fptr == NULL) {
            printf("The file doesn't exist or the program doesn't have read rights!\n");
            return 1;
        }

        int pos = 0;
        char ch;

        char buffer[BUFFER_SIZE];
        do {
            ch = fgetc(fptr);

            if(ch == EOF)
                buffer[pos++] = '\0';
            else
                buffer[pos++] = ch;
        }
        while(ch != EOF);

        //Close the file
        fclose(fptr);

        //Write to the pipe
        write(fd[WRITE_END], buffer, strlen(buffer) + 1);

        //Wait for the child to terminate
        wait(NULL);
    }
    //Child Process
    else {
        close(fd[WRITE_END]);

        char buffer[BUFFER_SIZE];

        //Read from the pipe
        read(fd[READ_END], buffer, BUFFER_SIZE);

        //Create a new file and write the content
        FILE *fptr = fopen(dest, "w");

        if(fptr == NULL) {
            printf("The file cannot be created or cannot be overwritten!\n");
            return 1;
        }

        //Write to the file
        fprintf(fptr, "%s", buffer);
        
        //Close the file
        fclose(fptr);

        exit(0);
    }

    return 0;
}