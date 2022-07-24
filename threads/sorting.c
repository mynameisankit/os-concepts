#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

typedef struct array {
    int start, end;
    int *arr;
} array;

void* sort(void* params);
void* merge(void* params);

void print_arr(array *arr);

int main(int argc, char *argv[]) {
    if(argc == 1) {
        fprintf(stderr, "Please input some numbers\n");
        exit(1);
    }
    
    array num;
    num.arr = (int *)malloc(sizeof(int)*(argc - 1));
    num.start = 0;
    num.end = argc - 2;

    for(int i = 1; i < argc; i++)
        num.arr[i - 1] = atoi(argv[i]);

    print_arr(&num);

    pthread_t tid;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, sort, &num);
    pthread_join(tid, NULL);

    print_arr(&num);

    free(num.arr);

    return 0;
}

void* sort(void* params) {
    array *arr = (array *)params;

    if(arr->start == arr->end)
        pthread_exit(0);

    pthread_t tid[3];
    pthread_attr_t attr[3];
    array** args = (array **)malloc(sizeof(array *)*2);

    //Divide and sort the sub-parts
    for(int i = 0; i < 2; i++) {
        pthread_attr_init(&attr[i]);
        args[i] = (array *)malloc(sizeof(array));
        args[i]->arr = arr->arr;
    }

    int mid = arr->start + (arr->end - arr->start)/2;

    args[0]->start = arr->start;
    args[0]->end = mid;
    pthread_create(&tid[0], &attr[0], sort, args[0]);
    
    args[1]->start = mid + 1;
    args[1]->end = arr->end;
    pthread_create(&tid[1], &attr[1], sort, args[1]);

    //Wait for the sub-parts to finish
    for(int i = 0; i < 2; i++)
        pthread_join(tid[i], NULL);

    free(args);

    //Merge the subarray
    pthread_attr_init(&attr[2]);
    pthread_create(&tid[2], &attr[2], merge, arr);
    pthread_join(tid[2], NULL);

    pthread_exit(0);
}

void* merge(void *params) {
    array *arr = (array *)params;

    int mid = arr->start + (arr->end - arr->start)/2;
    
    int *tmp = (int *)malloc(sizeof(int)*(arr->end - arr->start));
    int ptr1 = arr->start, ptr2 = mid + 1;

    int count = 0;
    while(ptr1 <= mid && ptr2 <= arr->end) {
        int curr;
        
        if(arr->arr[ptr1] > arr->arr[ptr2])
            curr = arr->arr[ptr2++];
        else
            curr = arr->arr[ptr1++];

        tmp[count++] = curr;
    }

    //Left-Overs
    while(ptr1 <= mid)
        tmp[count++] = arr->arr[ptr1++];

    while(ptr2 <= arr->end)
        tmp[count++] = arr->arr[ptr2++];

    count = arr->start;
    for(int i = 0; i <= (arr->end - arr->start); i++) {
        arr->arr[count] = tmp[i];
        count++;
    }

    free(tmp);

    pthread_exit(0);
}

void print_arr(array *arr) {
    printf("The array is : ");
    for(int i = arr->start; i <= arr->end; i++)
        printf("%d ", arr->arr[i]);
    printf("\n");

    return;
}