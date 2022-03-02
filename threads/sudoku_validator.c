#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

bool validate_sudoku(void);
void *validate_row(void* param);
void *validate_col(void* param);
void *validate_subgrid(void* param);

int **arr;
bool is_rows_valid, is_cols_valid, is_subgrids_valid;

int main(int argc, char *argv[]) {
    arr = NULL;

    arr = (int **)malloc(sizeof(int *)*9);
    for(int i = 0; i < 9; i++) {
        *(arr + i) = (int *)malloc(sizeof(int)*9);

        for(int j = 0; j < 9; j++)
            scanf("%d", &arr[i][j]);
    }

    printf("The given sudoku is...\n");
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++)
            printf("%d ", arr[i][j]);
        printf("\n");
    }
    printf("\n");

    bool res = validate_sudoku();
    if(res)
        printf("The sudoku is valid\n");
    else
        printf("The sudoku is not valid\n");

    return 0;
}

bool validate_sudoku(void) {
    is_rows_valid = is_cols_valid = is_subgrids_valid = true;

    pthread_t tid[27];
    pthread_attr_t attr[27];

    for(int i = 0; i < 27; i++) {
        pthread_attr_init(&attr[i]);
        pthread_t curr_thread;
        int *arg;

        if(i < 9) {
            arg = (int *)malloc(sizeof(int));
            *arg = i;
            pthread_create(&curr_thread, &attr[i], validate_row, arg);
        }
        else if(i < 18) {
            arg = (int *)malloc(sizeof(int));
            *arg = i - 9;
            pthread_create(&curr_thread, &attr[i], validate_col, arg);
        }
        else {
            arg = (int *)malloc(sizeof(int)*2);
            arg[0] = ((i - 18)/3)*3;
            arg[1] = ((i - 18)%3)*3;

            pthread_create(&curr_thread, &attr[i], validate_subgrid, arg);
        }

        tid[i] = curr_thread;
    }

    //Wait for the threads to complete
    for(int i = 0; i < 27; i++)
        pthread_join(tid[i], NULL);

    return is_rows_valid && is_cols_valid && is_subgrids_valid;
}

void *validate_row(void* param) {
    int row = *((int *)param);

    bool is_unique[10];
    memset(is_unique, true, 10*sizeof(bool));

    for(int j = 0; j < 9; j++) {
        int curr = arr[row][j];
        
        if(is_unique[curr])
            is_unique[curr] = false;
        else
            is_rows_valid &= false;
    }

    free(param);
    pthread_exit(0);
}

void *validate_col(void* param) {
    int col = *((int *)param);

    bool is_unique[10];
    memset(is_unique, true, 10*sizeof(bool));

    for(int i = 0; i < 9; i++) {
        int curr = arr[i][col];
        
        if(is_unique[curr])
            is_unique[curr] = false;
        else 
            is_cols_valid &= false;
    }

    free(param);
    pthread_exit(0);
}

void *validate_subgrid(void* param) {
    int *subgrid = (int *)param;

    int start_row = subgrid[0];
    int start_col = subgrid[1];

    bool is_unique[10];
    memset(is_unique, true, 10*sizeof(bool));

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int curr = arr[start_row + i][start_col + j];

            if(is_unique[curr])
                is_unique[curr] = false;
            else
                is_subgrids_valid &= false;
        }
    }

    free(param);
    pthread_exit(0);
}