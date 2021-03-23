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

ret[10]
int = column/row/area number

ret[11]
0 = row
1 = column
2 = grid

ret[12]
0 = false
1 = true
for slice
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

void *eval_slice(void *arg) {

    int s; 
    int *ret = malloc(13*sizeof(int));
    int *slice = (int *) arg;

    for(int i = 0; i < 9; i++) {
        s += (slice[i] - '0');
        ret[i] = (slice[i] - '0');
    }

    ret[10] = slice[10];
    ret[11] = slice[11];

    if(s == 45) {
        ret[12] = 1;
        return (void *) ret;
    } else {
        ret[12] = 0;
        return (void *) ret;
    }
}

void *eval_area(void *arg) {
    return 0;
}

// takes matrix struct and task type for init() function.
void evaluate(Matrix *p_puzzle) {

    void *r;
    pthread_t tid[MAX_THREADS];
    int *slice;

    for(int i = 0; i <= MAX_THREADS; i++) {

        slice = malloc(12 * sizeof(int));

        if(i < 9) {

            for(int s = 0; s < 9; s++) {
                slice[s] = p_puzzle->data[i][s];
            }
            
            slice[10] = i + 1;
            slice[11] = 0;

            int t = pthread_create(&tid[i], NULL, eval_slice, (void *)slice);

        } else if(i >= 9 && i < 19) {

            for(int s = 0; s < 9; s++) {
                slice[s] = p_puzzle->data[s][i - 9];
            }

            slice[10] = (i - 9) + 1;
            slice[11] = 1;

            int t = pthread_create(&tid[i], NULL, eval_slice, (void *)slice);
        }
    }

    //free(slice);

    for(int e = 0; e < 19; e++) {

        pthread_join(tid[e], &r);
        int *ret = (int *)r;
        char s_type[10];

        // for(int x = 0; x < 12; x++) {
        //     printf("%d", ret[x]);
        // }
        // printf("\n");

        if(ret[11] == 0) {
            strcpy(s_type, "row");
        } else if(ret[11] == 1) {
            strcpy(s_type, "column");
        } else if(ret[11] == 2) {
            strcpy(s_type, "area");
        }

        if(ret[12] == 1) {
            printf("Solution for %s %d is valid\n", s_type, ret[10]);
        } else if(ret[12] == 0) {
            printf("Solution for %s %d is not valid\n", s_type, ret[10]);
        }

    }

}
