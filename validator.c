#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int sudoku[9][9]={{6, 2, 4, 5, 3, 9, 1, 8, 7},
                 {5, 1, 9, 7, 2, 8, 6, 3, 4},
                 {8, 3, 7, 6, 1, 4, 2, 9, 5},
                 {1, 4, 3, 8, 6, 5, 7, 2, 9},
                 {9, 5, 8, 2, 4, 7, 3, 6, 1},
                 {7, 6, 2, 3, 9, 1, 3, 4, 8},
                 {3, 7, 1, 9, 5, 6, 8, 4, 2},
                 {4, 9, 6, 1, 8, 2, 5, 7, 3},
                 {2, 8, 5, 4, 7, 3, 9, 1, 6}};

int valid[27] = {0}; //set of valid nums ether 1 or 0, if they are all 1 then the sudoku is valid


typedef struct{ //structure of data used in threads
  int row;
  int column;
} table;

void *checkColumn(void* temp){ //funcition that validates each column
  table *values = (table* ) temp;
  int row = values->row;
  int col = values->column;
  int vArray[9] = {0};
  int i;
  if(row != 0 || col > 8){
    fprintf(stderr, "CheckColumn: Invalid row or column! row: %d    col: %d\n", row, col);
    pthread_exit(NULL);
  }

  for(i = 0; i < 9; i++) { //ensures numbers 1-9 only appear once in each column
    int num = sudoku[i][col];
        if( num < 1 || num > 9 || vArray[num -1 ] == 1) { //if there is a duplicate then exit thread
          pthread_exit(NULL);
        }
        else {
          vArray[num - 1] = 1;
        }
  }
  valid[18+col] = 1; //sets values to valid
  pthread_exit(NULL);
}

void *checkRow(void* temp) //function that validates each Row
{
  table *values = (table* ) temp;
  int row = values->row;
  int col = values->column;
  int vArray[9] = {0};
  int i;
  if(col != 0 || row > 8){
    fprintf(stderr, "CheckRow: Invalid row or column! row: %d    col: %d\n", row, col);
    pthread_exit(NULL);
  }

  for(i = 0; i < 9; i++) { //ensures number 1-9 appears once in each row
    int num = sudoku[row][i];
        if( num < 1 || num > 9 || vArray[num -1 ] == 1) { //if threre is a duplicate then exit thread
          pthread_exit(NULL);
        }
        else {
          vArray[num - 1] = 1;
        }
  }
  valid[9+row] = 1; //sets values to valid
  pthread_exit(NULL);
}

void *checkSquare(void* temp) { //function validates each 3x3 square
    table *values = (table* ) temp;
    int row = values->row;
    int col = values->column;
    int vArray[9] ={0};
    int i,j;
    if(row > 6 || row % 3  != 0 || col > 6 || col % 3 != 0){
      fprintf(stderr, "CheckSquare: Invalid row or column! row: %d    col: %d\n", row, col);
      pthread_exit(NULL);
    }

    for(i = row; i < row + 3; i++) { //for loops validates that each 3 by 3 square only has 1-9 once
      for(j = col; j < col + 3; j++) {
        int num = sudoku[i][j];
        if(num < 1 || num > 9 || vArray[num - 1] == 1){
          pthread_exit(NULL);
        }
        else
        {
          vArray[num - 1] = 1;
        }
      }
    }
    valid[row + col/3] = 1; //sets validation numbers
    pthread_exit(NULL);
}


int main(){
  pthread_t threads[27]; //total of 27 threads are needed

  int index = 0;
  int i, j;

  for(i = 0; i < 9; i++) { //implements threads and makes 9 threads for each row, column, and 3 by 3 square
    for(j = 0; j < 9; j++) {
      if(i % 3 == 0 && j % 3 ==0) {
        table *data = (table *) malloc(sizeof(table));
        data->row = i;
        data->column = j;
        pthread_create(&threads[index++], NULL, checkSquare, data);
        }
        if(i == 0) {
          table *colData = (table *) malloc(sizeof(table));
          colData->row = i;
          colData->column = j;
          pthread_create(&threads[index++], NULL, checkColumn, colData);
        }
        if(j == 0) {
          table *rowData = (table *) malloc(sizeof(table));
          rowData->row = i;
          rowData->column = j;
          pthread_create(&threads[index++], NULL, checkRow, rowData);
        }
    }
  }
  for(i=0; i < 27; i++) { //makes sure all threads finished
    pthread_join(threads[i], NULL);
  }

  for(i = 0; i <27; i++){ //if any value in the array is 0 then it invalid
    if(valid[i] == 0) {
      printf("Solution Invalid!\n");
      return EXIT_SUCCESS;

    }

    printf("Solution Valid!\n");//if every value in the array is 1 then it valid
    return EXIT_SUCCESS;
  }

}
