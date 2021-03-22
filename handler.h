#ifndef READER_H_

#define READER_H_
#define MAX_THREADS 27

struct matrix {
    int is_correct;
    int data[10][10];
    int row;
    int col;
};

struct matrix* read_into_matrix();
void evaluate();

#endif