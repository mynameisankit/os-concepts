#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct vector {
    int *data;
    size_t size;
};

typedef struct vector vector;

void get_array(char* line, vector *size);

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

    //Sort the arrays
    for(size_t i = 0; i < k; i++)
        qsort(array_list[i].data, array_list[i].size, sizeof(int), compare);

    //Write the sorted arrays to a file
    fptr = fopen("file2.txt", "w");
    if(fptr == NULL)
        fprintf(stderr, "Cannot open specified file\n");

    for(size_t i = 0; i < k; i++) {
        for(size_t j = 0; j < array_list[i].size; j++)
            fprintf(fptr, "%d ", array_list[i].data[j]);

        fprintf(fptr, "\n");
    }

    fclose(fptr);

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