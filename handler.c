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

Matrix *read_into_matrix(char *p) {
    
    Matrix *p_puzzle = malloc(sizeof(Matrix));
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
            } 
            
            r++;
        }
    }

    return p_puzzle;
}

void *is_valid(void *arg) {

    int s; 
    int *ret = malloc(11*sizeof(int));
    int *slice = (int *) arg;

    for(int i = 0; i < 9; i++) {
        s += (slice[i] - '0');
        ret[i] = (slice[i] - '0');
    }

    if(s == 45) {
        ret[10] = 1;
        return (void *) ret;
    } else {
        ret[10] = 0;
        return (void *) ret;
    }
}

// takes matrix struct and task type for init() function.
void evaluate(Matrix *p_puzzle) {

    void *r;
    pthread_t tid[MAX_THREADS];
    int *slice;

    for(int i = 0; i <= MAX_THREADS; i++) {

        slice = malloc(10 * sizeof(int));

        if(i < 9) {

            for(int s = 0; s < 9; s++) {
                slice[s] = p_puzzle->data[i][s];
            }

            int t = pthread_create(&tid[i], NULL, is_valid, (void *)slice);

        } else if(i < 19 && i > 9) {
            
            for(int s = 0; s < 9; s++) {
                slice[s] = p_puzzle->data[s][i];
            }
        }
    }

    free(slice);

    for(int e = 0; e < 9; e++) {

        pthread_join(tid[e], &r);
        int *ret = (int *)r;
        for(int x = 0; x < 9; x++) {
            printf("%d", ret[x]);
        }
        printf("\n");
        if(ret[10] == 1) {
            printf("This solution is valid\n");
        } else {
            printf("This solution is not valid\n");
        }

    }

}
