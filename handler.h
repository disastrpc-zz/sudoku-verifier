#ifndef READER_H_

#define READER_H_
#define MAX_THREADS 27

// Encapsulates 2d array that represents solved puzzle
typedef struct matrix {
    int data[10][10];
} Matrix;

// Represents arguments to be passed to functions
typedef struct args {
    int r;
    int c;
    Matrix *puzzle;
} Args;

Matrix* read_into_matrix();
void eval_slices();
void eval_grids();

#endif