#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DIM 1000

int calculateResult(int grid[DIM][DIM]) {
  int result = 0;
  for (int i = 0; i < DIM; i++) {
    for (int j = 0; j < DIM; j++){
      result += grid[i][j];
    }
  }
  return result;
}

enum Instruction {
  ON = 1,
  OFF = 0,
  SWITCH = 2,
  EMPTY = -1
};

struct Point {int x; int y;};

void part2(int *cell, enum Instruction instruction){
  switch (instruction){
    case ON: (*cell)++; break;
    case OFF: if(*cell > 0) {(*cell)--;}; break;
    case SWITCH: (*cell)+=2; break;
    default: break;
  }
}

void part1(int *cell, enum Instruction instruction){
  switch (instruction){
    case ON: (*cell) = ON; break;
    case OFF: (*cell) = OFF; break;
    case SWITCH: (*cell) = (*cell) == ON ? OFF : ON; break;
    default: break;
  }
}

int calculate(void (*func)(int *, enum Instruction)){
  int grid[DIM][DIM] = {{OFF}};
  FILE * file = fopen("input.txt", "r");
  char line[100];
  if(!file){
    perror("input.txt");
    exit(1);
  }
  while (fgets(line, sizeof(line), file)) {
    enum Instruction instruction = EMPTY;
    struct Point start = {0, 0};
    struct Point end = {0, 0};
    bool through = false;
    char* token = strtok(line, " ");
    while (token != NULL) {
      if(instruction == EMPTY){
        if(strcmp(token, "toggle") == 0) instruction = SWITCH;
        if(strcmp(token, "off") == 0) instruction = OFF;
        if(strcmp(token, "on") ==0) instruction = ON;
      }
      if(strcmp(token, "through") == 0){
        through = true;
      } else {
        char *comma = strchr(token, ',');
        if (comma) {
          *comma = '\0';
          int px = atoi(token);
          int py = atoi(comma + 1);
          if (through) {
            end.x = px;
            end.y = py;
          } else {
            start.x = px;
            start.y = py;
          }
        }
      }
      token = strtok(NULL, " ");
    }
    for(int i = start.x; i <= end.x; i++){
      for(int j = start.y; j <= end.y; j++){
        func(&grid[i][j], instruction);
      }
    }
  }
  fclose(file);
  return calculateResult(grid);
}

int main() {
  printf("day 06 part 01 result: %d\n", calculate(part1));
  printf("day 06 part 02 result: %d\n", calculate(part2));
  return 0;
}
