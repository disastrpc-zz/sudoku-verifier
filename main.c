/*
Sudoku validator for CSCI 375 Operating Systems
Professor Ahmad 
*/

#include <stdio.h>
#include <stdlib.h>
#include "handler.h"

int main(int argc, char *argv[]) {

    Matrix *p_puzzle = read_into_matrix(argv[1]);
    //struct matrix puzzle = *ptr_puzzle;
    evaluate(p_puzzle);
    return 0;
}