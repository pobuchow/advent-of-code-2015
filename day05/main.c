#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VOWELS_SIZE 5

bool validate(bool (*validatedFunction)(char *), char * str, bool expectedValue){
  int check = validatedFunction(str);
  if(check != expectedValue)
  {
    printf("calculation error on %s, expected value %d, but was %d\n", str, expectedValue, check);
    return false;
  }
  return true;
}

int calculateResult(bool (*func)(char *)) {
  FILE * file = fopen("input.txt", "r");
  char line[20];
  int result = 0;
  if(!file){
    perror("input.txt");
    exit(1);
  }
  while (fgets(line, sizeof(line), file)) {
    result += func(line) ? 1 : 0;
  }
  fclose(file);
  return result;
}

bool hasAtLeastVowels(char * input, unsigned short minValue){
  unsigned short inputSize = strlen(input);
  unsigned short result = 0;
  char vowels[VOWELS_SIZE] = {'a', 'e', 'i', 'o', 'u'};
  for(int i = 0; i < inputSize; i++){
    for(int j = 0; j < VOWELS_SIZE; j++){
      if(input[i] == vowels[j]){
        result++;
        if(result >= minValue) return true;
      }
    }
  }
  return false;
}

bool hasDoubleLetter(char * input){
  unsigned short inputSize = strlen(input);
  for(int i = 0; i < inputSize - 1; i++){
    if(input[i] == input[i+1]){
      return true;
    }
  }
  return false;
}

bool hasForbiddenStrings(char * input){
  unsigned short inputSize = strlen(input);
  for (int i = 0; i < inputSize - 1; i++) {
    switch (input[i]) {
      case 'a': if(input[i+1] == 'b') {return true;} break;
      case 'c': if(input[i+1] == 'd') {return true;} break;
      case 'p': if(input[i+1] == 'q') {return true;} break;
      case 'x': if(input[i+1] == 'y') {return true;} break;
    }
  }
  return false;
}

bool isNice(char * input){
  return hasAtLeastVowels(input, 3) && hasDoubleLetter(input) && !hasForbiddenStrings(input);
}

struct LetterPosition { char letter; char next; int position;};

bool hasPairOfNotOverpapping2Letters(char * input){
  unsigned short inputSize = strlen(input);
  struct LetterPosition * letterPositions = (struct LetterPosition * ) malloc((inputSize -1)  * sizeof(struct LetterPosition));
  for(int i = 0; i < inputSize - 1; i++){
    letterPositions[i] = (struct LetterPosition) {input[i], input[i+1], i};
  }
  for(int i = 0; i < inputSize - 1; i++){
    for(int j = 0; j < inputSize - 1; j++){
      if(i != j && 
        letterPositions[i].letter == letterPositions[j].letter && 
        letterPositions[i].next == letterPositions[j].next && 
        abs(letterPositions[i].position - letterPositions[j].position) > 1){
        free(letterPositions);
        return true;
      }
    }
  }
  free(letterPositions);
  return false;
}

bool hasLetterRepetingNeighbours(char * input){
  unsigned short inputSize = strlen(input);
  for(int i = 1; i < inputSize - 1; i++){
    if(input[i - 1] == input[i + 1]) return true;
  }
  return false;
}

bool isNicer(char * input){
  return hasPairOfNotOverpapping2Letters(input) && hasLetterRepetingNeighbours(input);
}

void part1(){
 bool validationResult =
    validate(isNice, "ugknbfddgicrmopn", true) &&
    validate(isNice, "aaa", true) &&
    validate(isNice, "jchzalrnumimnmhp", false) &&
    validate(isNice, "haegwjzuvuyypxyu", false) &&
    validate(isNice, "dvszwmarrgswjxmb", false);

  if(validationResult == true){
    int result = calculateResult(isNice);
    printf("day 05 part 01 result: %d\n", result);
  }
}

void part2(){
 bool validationResult =
    validate(isNicer, "qjhvhtzxzqqjkmpb", true) &&
    validate(isNicer, "xxyxx", true) &&
    validate(isNicer, "uurcxstgmygtbstg", false) &&
    validate(isNicer, "ieodomkazucvgmuy", false);

  if(validationResult == true){
    int result = calculateResult(isNicer);
    printf("day 05 part 02 result: %d\n", result);
  }
}

int main() {
  part1();
  part2();
  return 0;
}
