#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "handler.h"

/* 
Reads provided path (p) for a file which contains the Sudoku 9x9 grid.
The format for the file is the following:
_MATRIX_BEGIN_
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
_MATRIX_END_
The result is stored in a matrix struct, which contains a 2D array and an int value that represents if the provided grid is valid.
Function returns pointer to matrix struct. 
*/

struct matrix* read_into_matrix(char *p) {
    
    struct matrix *p_puzzle = malloc(sizeof(struct matrix));
    char d[1024];
    int r = 0, c = 0;
    FILE *array_f;
    
    // Open file
    array_f = fopen(p, "r");
    if(array_f == NULL) {
        printf("[!] Error: Unable to open file!");
        exit(1);
    } 

    // Loops through rows of the array while ignoring the tags
    while(fgets(d, sizeof d, array_f) != NULL) {

        // remove trailing newline picked up by fgets()
        d[strlen(d) - 1] = '\0';
        if(strcmp(d, "_MATRIX_BEGIN_") == 0 || strcmp(d, "_MATRIX_END_") == 0) {/*Pass*/} 
        else {
            // Loops through each column in the row and adds it to the 2D array
            for(c = 0; c < 9; c++) {
                p_puzzle->data[r][c] = d[c];
                //printf("[r:%d][c:%d]", r, c);
                //printf("%c  ", (int)ptr_puzzle->data[r][c]);
            }

            r++;
        }
        //printf("\n");
    }

    //int res = (ptr_puzzle->data[0][0] - '0') + (ptr_puzzle->data[0][2] - '0');
    //printf("%d", res);
    return p_puzzle;
}

void* check_rows(void *p) {

    int r, c;
    struct matrix *p_puzzle = malloc(sizeof(struct matrix));
    p_puzzle = (struct matrix *)p;

    for(r = 0; r < 9; r++) {
        for(c = 0; c < 9; c++) {
            printf("%c", p_puzzle->data[r][c]);
        }
    }    

    return NULL;
}

// takes matrix struct and task type for init() function.
void evaluate(struct matrix *p_puzzle, int t) {

    pthread_t threads[MAX_THREADS];

    for(int i = 0; i < MAX_THREADS; i++) {

        if(i < 9) {
            int t = pthread_create(&threads[i], NULL, check_rows, (void *)p_puzzle);
        } else {
            printf("%d leftover", i);
        }
    }
}
