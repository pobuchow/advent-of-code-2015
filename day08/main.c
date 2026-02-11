#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool validate(
  int (*validatedFunction)(char *, int (*)(char *)),
  int (*func)(char *),
  char *fileName,
  int expectedValue
) {
  int check = validatedFunction(fileName, func);
  if(check != expectedValue)
  {
    printf("calculation error on %s, expected value %d, but was %d\n", fileName, expectedValue, check);
    return false;
  }
  return true;
}

int calculateResult(char * fileName, int (*func)(char *)) {
  FILE * file = fopen(fileName, "r");
  char line[200];
  int result = 0;
  if(!file){
    perror(fileName);
    exit(1);
  }
  while (fgets(line, sizeof(line), file)) {
    result += func(line);
  }
  fclose(file);
  return result;
}

int countStringChars(char * str){
  int len = strlen(str);
  if (len > 0 && str[len - 1] == '\n') {
    len--;
  }
  return len;
}

int countInMemoryString(char * str) {
  int count = 0;
  for (int i = 1; str[i] != '\0' && str[i] != '\n'; ) {
    if (str[i] == '"') break;
    if (str[i] == '\\') {
      if (str[i + 1] == '\\' || str[i + 1] == '"') {
        count++;
        i += 2;
      } else if (str[i + 1] == 'x') {
        count++;
        i += 4;
      }
    } else {
      count++;
      i++;
    }
  }
  return count;
}

int calculateDiff(char * str){
  return countStringChars(str) - countInMemoryString(str);
}

int countEncodedStrings(char * str){
  int count = 2;
  for (int i = 0; str[i] != '\0' && str[i] != '\n'; i++) {
    if (str[i] == '"' || str[i] == '\\') {
      count += 2;
    } else {
      count += 1;
    }
  }
  return count;
}

int calculateDiff2(char * str){
  return countEncodedStrings(str) - countStringChars(str);
}

void part1(){
  if(validate(calculateResult, calculateDiff, "test.txt", 12)){
    int result = calculateResult("input.txt", calculateDiff);
    printf("day 08 part 01 result: %d\n", result);
  }
}

void part2(){
  if(validate(calculateResult, calculateDiff2, "test.txt", 19)){
    int result = calculateResult("input.txt", calculateDiff2);
    printf("day 08 part 02 result: %d\n", result);
  }
}

int main() {
  part1();
  part2();
  return 0;
}
