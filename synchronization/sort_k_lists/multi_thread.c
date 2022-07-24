#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>

struct vector {
    int *data;
    size_t size;
};

typedef struct vector vector;

//Global Variables
pthread_mutex_t mutex;

void get_array(char* line, vector *size);
void* sort_array(void *params);

int compare(const void *a, const void *b) {
   return (*(int*)a - *(int*)b);
}

int main(int argc, char* argv[]) {
    FILE* fptr;

    fptr = fopen("file1.txt", "r");
    if(fptr == NULL)
        fprintf(stderr, "Cannot open specified file\n");

    char* buffer = NULL;
    size_t buffer_size = 0;

    vector *array_list = NULL; 
    size_t k = 0;
    
    while(getline(&buffer, &buffer_size, fptr) != -1) {
        vector array;

        //Remove newline character
        buffer[buffer_size - 1] = '\0';

        get_array(buffer, &array);

        array_list = (vector *)realloc(array_list, sizeof(array)*(k + 1));

        array_list[k] = array;

        k++;

        free(buffer);
        buffer = NULL;
        buffer_size = 0;
    }

    fclose(fptr);

    //Create K-Threads then sort the arrays
    //and then write the sorted arrays to a file
    pthread_t tid[k];
    pthread_attr_t attr[k];

    //Create a mutex lock to prevent two threads from
    //accessing the file at the same time
    pthread_mutex_init(&mutex, NULL);

    for(size_t i = 0; i < k; i++) {
        pthread_attr_init(&attr[i]);
        pthread_create(&tid[i], &attr[i], sort_array, &(array_list[i]));
    }

    //Wait for the K-Threads to finish
    for(size_t i = 0; i < k; i++)
        pthread_join(tid[i], NULL);

    //Clean-Up
    if(buffer != NULL)
        free(buffer);
    
    if(array_list != NULL) {
        for(size_t i = 0; i < k; i++) {
            if(array_list[i].data != NULL)
                free(array_list[i].data);
        }

        free(array_list);
    }

     pthread_mutex_destroy(&mutex);

    return 0;
}

//Converts the string into an array of numbers
void get_array(char* line, vector *array) {
    const char* delimiter = " ";

    array->size = 0;
    for(size_t i = 0; i < strlen(line); i++) {
        if(line[i] == ' ')
            (array->size)++;
    }
    (array->size)++;
    
    array->data = (int *)malloc(sizeof(int)*(array->size));

    char* token = strtok(line, delimiter);
    size_t ctr = 0;
    
    while(token != NULL) {        
        (array->data)[ctr++] = atoi(token);
        token = strtok(NULL, delimiter);
    }
    
    return;
}

void* sort_array(void *params) {
    vector *arr = (vector *)params;

    qsort(arr->data, arr->size, sizeof(int), compare);

    //Spin Lock
    pthread_mutex_lock(&mutex);

    //Critical Section
    FILE *fptr = fopen("file2.txt", "a");
    if(fptr == NULL) {
        fprintf(stderr, "Cannot open specified file\n");
        pthread_exit(0);
    }

    for(size_t i = 0; i < (arr->size); i++)
        fprintf(fptr, "%d ", (arr->data)[i]);
    fprintf(fptr, "\n");

    fclose(fptr);

    //Releasing the lock
    pthread_mutex_unlock(&mutex);

    //Terminate
    pthread_exit(0);
}