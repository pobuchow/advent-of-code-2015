#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
  int result = 0;
  if(!file){
    perror("input.txt");
    exit(1);
  }
  while (fgets(line, sizeof(line), file)) {
    result += func(line);
  }
  fclose(file);
  return result;
}

int calculateSquare(char * str) {
  int l,w,h;
  sscanf(str, "%dx%dx%d", &l, &w, &h);
  int a = l*w;
  int b = w*h;
  int c = l*h;
  return 2*l*w + 2*w*h + 2*h*l + fmin(fmin(a,b), c);
}

int comp(const void * a, const void * b){
  return (*(int *)a - *(int *) b);
}

int calculateRibbon(char * str) {
  int l,w,h;
  sscanf(str, "%dx%dx%d", &l, &w, &h);
  int dim[] = {l, w, h};
  int n = sizeof(dim)/sizeof(dim[0]);
  qsort(dim, n, sizeof(int), comp);
  return 2*dim[0] + 2*dim[1] + l*w*h;
}

void part1(){
 int validationResult =
    validate(calculateSquare, "2x3x4", 58) +
    validate(calculateSquare, "1x1x10", 43);

  if(validationResult == 0){
    int result = calculateResult(calculateSquare);
    printf("day 01 part 01 result: %d\n", result);
  }
}

void part2(){
  int validationResult =
    validate(calculateRibbon, "2x3x4", 34) + 
    validate(calculateRibbon, "1x1x10", 14);

  if(validationResult == 0){
    int result = calculateResult(calculateRibbon);
    printf("day 01 part 02 result: %d\n", result);
  }
}

int main() {
  part1();
  part2();
  return 0;
}
