#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_THREADS 11
int valid = 0;

int sudoku[9][9] = { 
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

// Thread function to check all rows
void* checkRow(void* param) {
   int (*sudoku)[9] = param;
   for (int i = 0; i < 9; i++) {
        int seen[10] = {0};
        for (int j = 0; j < 9; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                printf("Invalid Sudoku: Row %d\n", i + 1);
                valid +=1;
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
   return NULL; // Return NULL or a result structure
}

// Thread function to check all columns
void* checkColumn(void* param) {
   int (*sudoku)[9] = param;
   for (int j = 0; j < 9; j++) {
        int seen[10] = {0};
        for (int i = 0; i < 9; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > 9 || seen[num]) {
                printf("Invalid Sudoku: Column %d\n", j + 1);
                valid +=1;
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
   return NULL;
}

// Thread function to check all 3x3 subgrids
void* checkSubGrid(void* param) {
    int subgridIndex = (int)(size_t)param;
    int startRow = (subgridIndex / 3) * 3;
    int startCol = (subgridIndex % 3) * 3;
    
   // Logic to check 3x3 subgrid for numbers 1 through 9
    int seen[10] = {0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = sudoku[startRow + i][startCol + j];
            if (num < 1 || num > 9 || seen[num]) {
                printf("Invalid Sudoku: Column %d\n", j + 1);
                valid +=1;
                pthread_exit(NULL);
            }
            seen[num] = 1;
        }
    }
   
   return NULL;
}




int main() {
    pthread_t threads[NUM_THREADS];
    int threadIndex = 0;

    // Row & column checks
    pthread_create(&threads[threadIndex++], NULL, checkRow, NULL);
    pthread_create(&threads[threadIndex++], NULL, checkColumn, NULL);

    // Creating nine threads for each 3x3 subgrid validation (Subgrid checks)
    for(int i = 0; i < 9; i++) {
        pthread_create(&threads[threadIndex++], NULL, checkSubGrid, (void*)(size_t)i);
    }

    // Waiting for all threads to complete
    for(int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Add validation logic here to determine if the Sudoku is valid based on the threads' results
    if (valid == 0){
    	printf("Valid Sudoku!\n");
    }
    
    return 0;
}
