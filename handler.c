#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "handler.h"

/* 
Reads provided path (p) for a file which contains the Sudoku 9x9 grid.
The format for the file is the following:
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
The result is stored in a matrix struct, which contains a 2D array.
Function returns pointer to matrix struct. 
*/

int valid[9] = {0};

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
        // Loops through each column in the row and adds it to the 2D array
        for(c = 0; c < 9; c++) {
            p_puzzle->data[r][c] = d[c];
        } 
        
        r++;
    }

    return p_puzzle;
}

/*
Takes pointer to Args struct and loops through the 3x3 areas returning an int
*/
void *eval_g(void *arg){

	int c[10] = {0};
	Args *meta = (Args *)arg;
    int *ret = malloc(sizeof(int));

    // Loop through first and second dimensions of array taking 3x3 steps to form the puzzle areas.
	int i, j;
	for(i = meta->r; i < meta->r + 3; ++i){
		for(j = meta->c; j < meta->c + 3; ++j){
            // Check if a byte in c has been flipped.
			if(c[meta->puzzle->data[i][j]] == 1){
                ret[0] = 2;
				return (void *)ret;
			}
			c[meta->puzzle->data[i][j]] = 1;
		}
	}

    // Return valid subgrid
    ret[0] = 1;
	return (void *)ret; 
}

/*
Takes pointer to 9 byte slice that contains either a column or row of the sudoku puzzle
The returning array contains some bytes at the end which represent some states about that particular grid.

ret[10]
int = column/row/area number for the row/column the thread was working on

Returns a 0 if the slice was a row and 1 if it was a column
ret[11]
0 = row
1 = column

Lastly the 12'd index represents a boolean value indicating if that particular slice is valid
ret[12]
0 = false
1 = true
for slice
*/

void *eval_s(void *arg) {

    int *s = malloc(sizeof(int));
    int *ret = malloc(13*sizeof(int));
    int *slice = (int *) arg;

    for(int i = 0; i < 9; i++) {
        s[0] += (slice[i] - '0');
        ret[i] = (slice[i] - '0');
    }

    ret[10] = slice[10];
    ret[11] = slice[11];

    if(s[0] == 45) {
        ret[12] = 1;
        return (void *) ret;
    } else if(s[0] != 45) {
        ret[12] = 0;
        return (void *) ret;
    }
}

/*
Takes pointer to Matrix struct and spawns worker threads that check the puzzle's areas.
After threads finish the return value (1 or 2) is taken to check the threads.
*/
void eval_grids(Matrix *p_puzzle) {

    int i, j;
    int index = 0;
    void *r;
    pthread_t tid[9];

    // Allocate space for 2d array
    int *puzzle = (int *)malloc(10 * 10 * sizeof(int));

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
            if (i%3 == 0 && j%3 == 0) {
                Args *g_args = (Args *) malloc(sizeof(Args));
                g_args->r = i;
                g_args->c = j;
                g_args->puzzle = p_puzzle;
                pthread_create(&tid[index++], NULL, eval_g, (void *)g_args);
            }
        }
    }

    for(i = 0; i < 9; i++) {
        pthread_join(tid[i], &r);
        if(*(int *)r == 2) {
            printf("Solution for area %d is not valid\n", i + 1);
        } else {
            printf("Solution for area %d is valid\n", i + 1);
        }
    }

    free(puzzle);
}

// takes matrix struct and task type for init() function.
void eval_slices(Matrix *p_puzzle) {

    void *r;
    pthread_t tid[MAX_THREADS - 9];
    int *slice;

    for(int i = 0; i <= MAX_THREADS - 9; i++) {

        slice = malloc(12 * sizeof(int));

        // Spawns worker thread that handle rows
        if(i < 9) {

            for(int s = 0; s < 9; s++) {
                slice[s] = p_puzzle->data[i][s];
            }
            
            // Slice contains metadata which is passed to the threads
            // slice[10] == Row/Column index
            // slice[11] == Type of slice (row or column)
            slice[10] = i + 1;
            slice[11] = 0;

            int t = pthread_create(&tid[i], NULL, eval_s, (void *)slice);

        // Next batch of threads handles the columns
        } else if(i >= 9 && i < 19) {

            for(int s = 0; s < 9; s++) {
                slice[s] = p_puzzle->data[s][i - 9];
            }

            slice[10] = (i - 9) + 1;
            slice[11] = 1;

            int t = pthread_create(&tid[i], NULL, eval_s, (void *)slice);
        }
    }

    for(int e = 0; e < 18; e++) {

        pthread_join(tid[e], &r);
        int *ret = (int *)r;
        char s_type[10];

        if(ret[11] == 0) {
            strcpy(s_type, "row");
        } else if(ret[11] == 1) {
            strcpy(s_type, "column");
        }

        if(ret[12] == 1) {
            printf("Solution for %s %d is valid\n", s_type, ret[10]);
        } else if(ret[12] == 0) {
            printf("Solution for %s %d is not valid\n", s_type, ret[10]);
        }

        free(ret);
    }
    free(slice);
}