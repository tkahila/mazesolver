#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "stack.h"

//structure for maze data
typedef struct {
    char **table;
    int rows;
    int columns;
    int start_x;
    int start_y;
} MazeData;


void freeMazeData(MazeData *data) {
    for (int i = 0; i < data->rows; i++) {
        free(data->table[i]);
    }
    free(data->table);
}

void printMaze(MazeData *data){

  for (int x = 0; x < data->rows; x++)
  {
    for (int y = 0; y < data->columns; y++){
      printf("%c", data->table[x][y]);
    }  
  }
  printf("\n");

}

//find starting point '^' from array and return true if found
int findStartPoint(MazeData *data){

    //get staring point
    for (int x = 0; x < data->rows; x++) {
        for (int y = 0; y < data->columns; y++) {
            if (data->table[x][y] == '^') {
                data->start_x = x;
                data->start_y = y;
                return true;
            }
        }
    }
    printf("Cannot find start point\n");
    return false;
}

//solve maze using DFS algorithm
int solveMazeDFS(MazeData *data, int maxMoves){


    if(!findStartPoint(data)){
        return -1;
    }

    //allocate memory for isvisited 2d boolean array
    bool **isvisited = (bool **)malloc(data->rows * sizeof(bool *));
    for (int i = 0; i < data->rows; i++) {
        isvisited[i] = (bool *)malloc(data->columns * sizeof(bool));
    }

    //init isvisited array with falses
    for (int i = 0; i < data->rows; i++) {
        for (int j = 0; j < data->columns; j++) {
            isvisited[i][j] = false;
        }
    }

    Point start_index = {data->start_x, data->start_y, '^'};
    Point current_index = {data->start_x, data->start_y, '^'};

    //Stack will be reallocated when pushing new point to stack
    Stack *pathstack = createStack(1);
    int moves = 0;

    push(pathstack, start_index);

    isvisited[start_index.x][start_index.y] = true;

    //run until stack is empty i.e cant find exit
    while(!isEmpty(pathstack)){

        if(moves > maxMoves){
             printf("Cannot solve maze by using given maximum moves: %d\n", maxMoves);
             goto FREE_MEMORY;
        }

        //if exit 'E' found then print solution and exit from loop
        if(data->table[current_index.x][current_index.y] == 'E'){

            int pathlength = 0;
            //add path in maze array from stack
            while(!isEmpty(pathstack)){
                Point p = pop(pathstack);
                data->table[p.x][p.y] = p.directionMark;
                pathlength++;
            }

            printMaze(data);
            printf("Maze solved with move count %d!\n", moves);
            printf("Pathlength: %d\n", pathlength);
            
            goto FREE_MEMORY;
            
        }

        //up, down, left, right directions for for loop
        int direction[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        char directionMarks[4] = {'^','v','<','>'};

        bool visited = false;

        for (int i = 0; i < 4; i++) {

                if(
                // check boundaries
                current_index.x + direction[i][0] >=0 
                && current_index.y + direction[i][1] >=0
                && current_index.x + direction[i][0] < data->rows 
                && current_index.y + direction[i][1] < data->columns
                //check if point already visited
                && isvisited[current_index.x + direction[i][0]][current_index.y + direction[i][1]] == false 
                //check if wall detected
                && data->table[current_index.x + direction[i][0]][current_index.y + direction[i][1]] != '#')
                {
                    //if next point is valid then add it to the stack
                    Point p1 = {current_index.x + direction[i][0], current_index.y + direction[i][1]};
                    p1.directionMark = directionMarks[i];
                    
                    push(pathstack, p1);
                    current_index = p1;
                    isvisited[current_index.x][current_index.y] = true;
                    visited = true;
                    //keep record from used moves
                    moves++;

                }

          }
          //delete point from stack if point does not lead anywhere
          if(!visited){
            pop(pathstack);
            //go back to earlier point and continue
            current_index = peek(pathstack);
          }
    }

    printf("Cannot solve maze\n");

    FREE_MEMORY:

    destroyStack(pathstack);

    for (int i = 0; i < data->rows; i++) {
        free(isvisited[i]);
    }
    free(isvisited);

  }


//Dynamically allocate 2d array from given textfile
int create_array_from_textfile(MazeData *data, FILE *fileptr) {

    if (fileptr == NULL) {
        perror("Cannot open file \n"); 
        return -1;
    }

    data->rows = 0;
    data->columns = 0;
    data->table = NULL;

    char *line = NULL;
    size_t pituus = 0;

    while (getline(&line, &pituus, fileptr) != -1) {

        (data->rows)++;

        data->table = (char **)realloc(data->table, data->rows * sizeof(char *));

        if (data->table == NULL) {
            perror("memory reallocation failed");
            free(line);
            freeMazeData(data); 
            return -1;
        }
        data->table[data->rows - 1] = strdup(line);

        if (data->table[data->rows - 1] == NULL) {
            perror("memory allocation failed");
            free(line);
            freeMazeData(data); 
            return -1;
        }

        int linelength = strlen(line);
        if (linelength > data->columns) {
            data->columns = linelength;
        }
    }

    free(line);
    return 1;
}


int main(int argc, char *argv[]) {

    FILE *fptr = NULL;
    
    int maxMoves = INT_MAX;

    if(argc > 1){
        fptr = fopen(argv[1], "r");
        if(argc > 2){
            maxMoves = atoi(argv[2]);
        }
    }
    else{
        printf("No maze textfile given as argument... Using default: maze1.txt max moves: INT_MAX\n\n");
        char filename[] = "maze1.txt";
        fptr = fopen(filename, "r");
    }
            
    if (fptr == NULL) { 
        printf("Cannot open file \n"); 
        return 1;
    } 
  
    MazeData myMaze;

    if(create_array_from_textfile(&myMaze, fptr)){
        solveMazeDFS(&myMaze, maxMoves);
    }
    
    freeMazeData(&myMaze);
    fclose(fptr); 

    return 0;
}