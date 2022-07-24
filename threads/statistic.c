#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<limits.h>

#define ARR_END INT_MAX

double avg;
int max, min;

void* get_avg(void *params);
void* get_max(void *params);
void* get_min(void *params);

int main(int argc, char *argv[]) {
    if(argc == 1) {
        fprintf(stderr, "Please input some numbers\n");
        exit(1);
    }

    /**
     * Imp Point:-
     * A void pointer cannot be dereferenced
     * We need to typecast the underlying pointer first, 
     * and then use it
     * */

    int *arr = (int *)malloc(sizeof(int)*argc);

    for(int i = 1; i < argc; i++)
        arr[i - 1] = atoi(argv[i]);
    
    //Set the end as null to detect the end of the array
    arr[argc - 1] = ARR_END;

    pthread_t threads[3];
    pthread_attr_t attr[3];

    //Start the threads

    pthread_attr_init(&attr[0]);
    pthread_create(&threads[0], &attr[0], get_avg, arr);

    pthread_attr_init(&attr[1]);
    pthread_create(&threads[1], &attr[1], get_min, arr);

    pthread_attr_init(&attr[2]);
    pthread_create(&threads[2], &attr[2], get_max, arr);

    //Wait for the threads to finish
    for(int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);

    printf("The average value is %lf\n", avg);
    printf("The maximum value is %d\n", max);
    printf("The minimum value is %d\n", min);
    
    free(arr);
    return 0;
}

void* get_avg(void *params) {
    int *arr = (int *)params;

    int sum, count, curr;
    sum = count = 0;

    curr = *(arr + count++);
    while(curr != ARR_END) {        
        sum += curr;
        curr = *(arr + count++);
    }

    avg = (double)sum/(count - 1);

    pthread_exit(0);
}

void* get_max(void *params) {
    int *arr = (int *)params;

    max = INT_MIN;

    int count = 0, curr = *(arr + count++);;    
    while(curr != ARR_END) {
        if(curr > max)
            max = curr;

        curr = *(arr + count++);
    }

    pthread_exit(0);
}

void* get_min(void *params) {
    int *arr = (int *)params;

    min = INT_MAX;

    int count = 0, curr = *(arr + count++);;    
    while(curr != ARR_END) {
        if(min > curr)
            min = curr;

        curr = *(arr + count++);
    }

    pthread_exit(0);
}