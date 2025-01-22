//Developed by: 
//Simon Marcotte (scmarcot)
//Carlos Colina (cmcolina)
//Leo Dominguez Martinez (domingu1)
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include "lab1_IO.h"

typedef struct {
    int **A;
    int **B;
    int **C;
    int cover;
    int id;
    int block_x;
    int block_y;
    int size;
} thread_data_t;


void* multiply_matrix_block(void* arg) {

    thread_data_t* data = (thread_data_t*) arg;

    //Start of X and Y intervals
    int x = data->block_x * data->cover;
    int y = data->block_y * data->cover;

    //End of x and y intervals
    int xend = (int) fmin(x + data->cover, data->size);
    int yend = (int) fmin(y + data->cover, data->size);


    //from C[Xstart][Ystart] to C[xend][yend]
    for (int i = x; i < xend; i++){
        for (int j = y; j < yend; j++){
            //For each C(i,j), do the dot-product
            int sum = 0;
            for (int r = 0; r < data->size; r++) {
                sum += data->A[i][r] * data->B[r][j];
            }
            data->C[i][j] = sum;
        }
    }

    return NULL;
}



int main(int argc, char* argv[]) {

    //load in 2 input matrix

    if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        exit(1);
    }

    int num_threads = atoi(argv[1]);
    int sqrt_p = (int)sqrt(num_threads);
    if (sqrt_p * sqrt_p != num_threads) {
        printf("Number of threads must be a perfect square.\n");
        exit(1);
    }


    int **A, **B, **C;
    int size;

    Lab1_loadinput(&A, &B, &size);

    if (size % sqrt_p != 0) {
        printf("Matrix size must be divisible by sqrt(num_threads).\n");
        exit(1);
    }

    // Allocate memory for result matrix C
    C = malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        C[i] = malloc(size * sizeof(int));
    }

    double start_parallel, end_parallel;
    // thread_cover is used for the block size each thread covers
    int thread_cover = size / sqrt_p;
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    int thread_idx = 0;
    for (int k = 0; k < num_threads; k++) {
     //Apply formulas from lab to find X and Y from P(x,y)
        int x = (int) floor((double) k / sqrt_p);
        int y = k % sqrt_p;
        thread_data[thread_idx].A = A;
        thread_data[thread_idx].B = B;
        thread_data[thread_idx].C = C;
        thread_data[thread_idx].cover = thread_cover;
        thread_data[thread_idx].id = thread_idx;
        thread_data[thread_idx].block_x = x;
        thread_data[thread_idx].block_y = y;
        thread_data[thread_idx].size  = size;
        thread_idx++;
    }

    GET_TIME(start_parallel); //right before threads created

    //spawn all threads with their data
    for (int i = 0; i < num_threads; i++){
        pthread_create(&threads[i], NULL, multiply_matrix_block, &thread_data[i]);
    }
    // Join all threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    GET_TIME(end_parallel);

    // Save the result matrix
    Lab1_saveoutput(C, &size, end_parallel - start_parallel);

    // Free allocated memory
    for (int i = 0; i < size; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}


