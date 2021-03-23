#ifndef READER_H_

#define READER_H_
#define MAX_THREADS 27

typedef struct matrix {
    int data[10][10];
} Matrix;

Matrix* read_into_matrix();
void evaluate();

#endif