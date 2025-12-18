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

struct Location {
  int x;
  int y;
};

struct Location * traverseMap(char * str, int * size){
  int strSize = strlen(str);
  struct Location * visitedLocations = malloc(sizeof(struct Location) * (strSize + 1));
  visitedLocations[0] = (struct Location){0,0};
  int index = 1;
  for (int i = 0; i < strSize; i++) {
    struct Location nextLocation = visitedLocations[index-1];
    switch (str[i]) {
      case '^': nextLocation.y++; break;
      case 'v': nextLocation.y--; break;
      case '>': nextLocation.x++; break;
      case '<': nextLocation.x--; break;
      default: continue;
    }
    visitedLocations[index] = nextLocation;
    index++;
  }
  (*size) = index;
  return visitedLocations;
}

int countUnique(struct Location * locations, int locationsSize){
  struct Location * uniqueLocations = malloc(sizeof(struct Location) * (locationsSize));
  int uniqueLocationsSize = 0;
  for(int i = 0; i < (locationsSize); i++){
    int x = locations[i].x;
    int y = locations[i].y;
    int isUnique = true;
    for(int j = 0; j < uniqueLocationsSize; j++){
      if(uniqueLocations[j].x == x && uniqueLocations[j].y == y){
        isUnique = false;
      }
    }
    if(isUnique){
      uniqueLocations[uniqueLocationsSize] = locations[i];
      uniqueLocationsSize++;
    }
  }
  free(uniqueLocations);
  return uniqueLocationsSize;
}

int calculateUniqueVisits(char * str) {
  int visitedLocationsSize = 0;
  struct Location * visitedLocations = traverseMap(str, &visitedLocationsSize);
  int uniqueLocationsSize = countUnique(visitedLocations, visitedLocationsSize);
  free(visitedLocations);
  return uniqueLocationsSize;
}

void splitMap(char * map, char * left, char * right){
  size_t r = 0, l = 0;

  for(size_t i = 0; map[i] != '\0'; i++){
    if(i%2 == 0){
      left[l++] = map[i];
    } else {
      right[r++] = map[i];
    }
  }

  left[l] = '\0';
  right[r] = '\0';
}

int calculateUniqueSplitedVisits(char * str) {
  int strLen = strlen(str);
  char splitLeft[strLen/2 + 2];
  char splitRight[strLen/2 + 2];
  int visitedLocationsSplitLeftSize = 0, visitedLocationsSplitRightSize = 0;
  splitMap(str, splitLeft, splitRight);

  struct Location * splitLeftLocations = traverseMap(splitLeft, &visitedLocationsSplitLeftSize);
  struct Location * splitRightLocations = traverseMap(splitRight, &visitedLocationsSplitRightSize);

  int visitedLocationsSize = visitedLocationsSplitLeftSize + visitedLocationsSplitRightSize;

  struct Location * visitedLocations = malloc(sizeof(struct Location) * visitedLocationsSize);

  for (int i = 0; i < visitedLocationsSplitLeftSize; i++){
    visitedLocations[i] = splitLeftLocations[i];
  }
  for (int i = 0; i < visitedLocationsSplitRightSize; i++){
    visitedLocations[visitedLocationsSplitLeftSize + i] = splitRightLocations[i];
  }

  free(splitLeftLocations);
  free(splitRightLocations);

  return countUnique(visitedLocations, visitedLocationsSize);
}

void part1(){
 int validationResult =
    validate(calculateUniqueVisits, ">", 2) +
    validate(calculateUniqueVisits, "^>v<", 4) +
    validate(calculateUniqueVisits, "^v^v^v^v^v", 2);

  if(validationResult == 0){
    int result = calculateResult(calculateUniqueVisits);
    printf("day 01 part 01 result: %d\n", result);
  }
}

void part2(){
  int validationResult =
    validate(calculateUniqueSplitedVisits, "^v", 3) +
    validate(calculateUniqueSplitedVisits, "^>v<", 3) +
    validate(calculateUniqueSplitedVisits, "^v^v^v^v^v", 11);

  if(validationResult == 0){
    int result = calculateResult(calculateUniqueSplitedVisits);
    printf("day 01 part 02 result: %d\n", result);
  }
}

int main() {
  part1();
  part2();
  return 0;
}
