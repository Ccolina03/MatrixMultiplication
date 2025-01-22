/*
Generate two same sized square matrices in which the every element is uniformly randomly selected integer within some absolute value

-----
Compiling:
    > gcc matrixgen.c -o matrixgen

-----
Synopsis:
    matrixgen [-sbpo]

-----
Options:
    -s    specify the size of the matrices (default 10) 
    -b    upper bound of the absolute value of the elements (default 5)
    -p    print the result
    -o    specify the output path (default "./data_input") 

-----
Outputs:
    Output file:
    data_input: storing the two generated matrices

-----
Error returns:
    -1    unexpected options
    -2    fail to open file 

-----
Example:
    >matrixgen 
    generate two 10 by 10 matrices with elements between -5 and 5, and store the result in "./data_input"
   
    >matrixgen -s 100 -b 10 -p
    generate two 100 by 100 matrices with elements between -10 and 10, print the result and store it in "data_input"
*/

/* matrixgen.c
Generate two same-sized square matrices, each element is a uniformly
random integer within ±bound, and store them in "data_input".
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

void generate_matrix(int **matrix, int size, int bound) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = rand() % (2 * bound + 1) - bound;
        }
    }
}

void print_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void save_matrices_to_file(const char *path, int **A, int **B, int size) {
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Error opening file");
        exit(-2);
    }
    // First line: size
    fprintf(file, "%d\n", size);

    // Next 'size' lines: matrix A
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d\t", A[i][j]);
        }
        fprintf(file, "\n");
    }

    // Next 'size' lines: matrix B
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fprintf(file, "%d\t", B[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    int size = 10;           // default size
    int bound = 5;           // default bound
    int print = 0;           // whether to print or not
    const char *output_path = "./data_input";

    int opt;
    while ((opt = getopt(argc, argv, "s:b:po:")) != -1) {
        switch (opt) {
        case 's':
            size = atoi(optarg);
            break;
        case 'b':
            bound = atoi(optarg);
            break;
        case 'p':
            print = 1;
            break;
        case 'o':
            output_path = optarg;
            break;
        default:
            fprintf(stderr, "Usage: %s [-s size] [-b bound] [-p] [-o output_path]\n", argv[0]);
            exit(-1);
        }
    }

    srand(time(NULL));

    // Allocate
    int **A = (int **)malloc(size * sizeof(int *));
    int **B = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        A[i] = (int *)malloc(size * sizeof(int));
        B[i] = (int *)malloc(size * sizeof(int));
    }

    // Generate
    generate_matrix(A, size, bound);
    generate_matrix(B, size, bound);

    // Optionally print
    if (print) {
        printf("Matrix A:\n");
        print_matrix(A, size);
        printf("Matrix B:\n");
        print_matrix(B, size);
    }

    // Save to file
    save_matrices_to_file(output_path, A, B, size);

    // Free
    for (int i = 0; i < size; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);

    return 0;
}
