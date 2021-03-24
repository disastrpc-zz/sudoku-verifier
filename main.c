/*
Sudoku Puzzle Verifier Using Multithreading
by Jared Freed
Overview:

The program defines various structs that represent the sudoku puzzle and its data. With this several functions insidle handler.c perform the spawning of worker threads to validate the puzzle.

The puzzle is taken from user input inside argv[1].

read_into_matrix(char *p) 
Takes path to .txt file that contains the solved sudoku puzzle. It returns a pointer to a Matrix struct which encapsulates a 2d array that represents the sudoku puzzle. 

eval_grids(Matrix *p_puzzle)
Takes an Args struct which contains the starting row and column that must be checked for that particular grid, as well as a pointer to a Matrix struct containing the puzzle. It calls eval_g() to perform the evaluation.

eval_g(void *arg)
Takes Args struct. Loops through the grids of the puzzle and returns 1 if the grid is valid and 2 if its invalid.

eval_slices(Matrix *p_puzzle)
Takes pointer to Matrix struct containing the sudoku puzzle. 
Handles spawning worker threads that handle evaluation of the rows and columns of the grid. It calls eval_s() to perform the evaluation.

eval_s(void *arg)
Takes pointer to a slice representing a row or grid of the puzzle. The slice[] array also contains metadata which is used by the func such as the index of the slice and whether it was a row or column.

Note:
Input file must have a newline at the end, otherwise the puzzle won't be read properly. The ones provided already have it.
*/

#include <stdio.h>
#include <stdlib.h>
#include "handler.h"


int main(int argc, char *argv[]) {

    char *header = 
    "Project #1\nby Jared Freed for CSCI 375 Operating Systems with Professor Aftab Ahmad.\n"
    "I certify that I have done this project by myself and am accountable in accordance with the John Jay College’s rules about Academic Integrity if found otherwise.\n"
    "This is a program written in C using multithreading to verify the correctness of a Sudoku puzzle.\n" 
    "The program can verify a 9 X 9 Sudoku by checking the sum of each column, each row, and each of the nine (9) 3 X 3 blocks starting from the top left.\n"
    "It will verify if all of these have all digits from 1 to 9 by adding them to 45.\n"
    "The input is read from a file called matrix.txt attached with the submitted documents.\n";
    printf("%s\n", header);
    // Call read_into_matrix to receive Matrix struct pointer.
    // Argument is taken from argv[1] and should be a path to a file containing the 9x9 sudoku grid. 

    Matrix *p_puzzle = read_into_matrix(argv[1]);

    // calls two main methods to evaluate slices and grids for puzzle
    eval_slices(p_puzzle);
    eval_grids(p_puzzle);
    return 0;
}