#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int validate(int (*validatedFunction)(char *), char * str, int expectedValue){
  int check = validatedFunction(str);
  if(check != expectedValue)
  {
    printf("calculation error on %s, expected value %d, but was %d\n", str, expectedValue, check);
    return 1;
  }
  return 0;
}

int calculateResult(int (*func)(char *)) {
  FILE * file = fopen("input.txt", "r");
  char line[8196];
  if(!file){
    perror("input.txt");
    exit(1);
  }
  if (!fgets(line, sizeof(line), file)) {
    perror("input.txt");
    fclose(file);
    exit(1);
  }
  fclose(file);
  return func(line);
}

int calculateFloor(char * str) {
  int strSize = strlen(str);
  int result = 0;
  for (int i = 0; i < strSize; i++) {
    switch (str[i]) {
      case '(':
        result++;
      break;
      case ')':
        result--;
        break;
    }
  }
  return result;
}

int calculateBasement(char * str) {
  int strSize = strlen(str);
  int floorCounter = 0;
  for (int i = 0; i < strSize; i++) {
    switch (str[i]) {
      case '(':
        floorCounter++;
      break;
      case ')':
        floorCounter--;
        break;
    }
    if(floorCounter==-1){
      return i+1;
    }
  }
  return -1;
}

void part1(){
 int validationResult =
    validate(calculateFloor, "(())", 0) +
    validate(calculateFloor, "()()", 0) +
    validate(calculateFloor, "(((", 3) +
    validate(calculateFloor, "(()(()(", 3) +
    validate(calculateFloor, "))(((((", 3) +
    validate(calculateFloor, "())", -1) +
    validate(calculateFloor, "))(", -1) +
    validate(calculateFloor, ")))", -3) +
    validate(calculateFloor, ")())())", -3);
  
  if(validationResult == 0){
    int part1Result = calculateResult(calculateFloor);
    printf("day 01 part 01 result: %d\n", part1Result);
  }
}

void part2(){
  int validationResult =
    validate(calculateBasement, ")", 1) + 
    validate(calculateBasement, "()()))()", 5);
  
  if(validationResult == 0){
    int part2Result = calculateResult(calculateBasement);
    printf("day 01 part 02 result: %d\n", part2Result);
  }
}

int main() {
  part1();
  part2();
  return 0;
}
