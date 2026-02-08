#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MAP_SIZE 500
#define MAX_KEY_SIZE 8
#define MAX_INSTRUCTIONS_SIZE 500

typedef struct {
  int size;
  char keys[MAX_MAP_SIZE][MAX_KEY_SIZE];
  unsigned short values[MAX_MAP_SIZE];
} WireMap;

typedef enum {
  ASSIGN = 0,
  NOT = 1,
  AND = 2,
  OR = 3,
  LSHIFT = 4,
  RSHIFT = 5
} InstructionType;

typedef enum {
  WIRE, SIGNAL
} SourceType;

typedef struct {
  SourceType type;
  union {
    char wire[MAX_KEY_SIZE];
    unsigned short signal;
  };
} Source;

typedef struct {
  InstructionType type;
  char target[MAX_KEY_SIZE];
  Source left;
  Source right;
  unsigned char shift;
  bool isDone;
} Instruction;

Instruction parseLine(char * line){
  Instruction result = (Instruction){0};
  if(sscanf(line, "%hu -> %7s", &result.right.signal, result.target) == 2){
    result.type = ASSIGN;
    result.right.type = SIGNAL;
  } else if (sscanf(line, "%7s -> %7s", result.right.wire, result.target) == 2) {
    result.type = ASSIGN;
    result.right.type = WIRE;
  } else if (sscanf(line, "NOT %7s -> %7s", result.right.wire, result.target) == 2){
    result.type = NOT;
    result.right.type = WIRE;
  } else if (sscanf(line, "%hu AND %7s -> %7s", &result.left.signal, result.right.wire, result.target) == 3){
    result.left.type = SIGNAL;
    result.right.type = WIRE;
    result.type = AND;
  } else if (sscanf(line, "%7s AND %hu -> %7s", result.left.wire, &result.right.signal, result.target) == 3){
    result.left.type = WIRE;
    result.right.type = SIGNAL;
    result.type = AND;
  } else if (sscanf(line, "%7s AND %7s -> %7s", result.left.wire, result.right.wire, result.target) == 3){
    result.left.type = WIRE;
    result.right.type = WIRE;
    result.type = AND;
  } else if (sscanf(line, "%hu OR %7s -> %7s", &result.left.signal, result.right.wire, result.target) == 3){
    result.left.type = SIGNAL;
    result.right.type = WIRE;
    result.type = OR;
  } else if (sscanf(line, "%7s OR %hu -> %7s", result.left.wire, &result.right.signal, result.target) == 3){
    result.left.type = WIRE;
    result.right.type = SIGNAL;
    result.type = OR;
  } else if (sscanf(line, "%7s OR %7s -> %7s", result.left.wire, result.right.wire, result.target) == 3){
    result.left.type = WIRE;
    result.right.type = WIRE;
    result.type = OR;
  } else if(sscanf(line, "%7s LSHIFT %hhu -> %7s", result.right.wire, &result.shift, result.target) == 3){
    result.right.type = WIRE;
    result.type = LSHIFT;
  } else if(sscanf(line, "%7s RSHIFT %hhu -> %7s", result.right.wire, &result.shift, result.target) == 3){
    result.right.type = WIRE;
    result.type = RSHIFT;
  } else {
    printf("[WARNING] could not parse the line: %s", line);
  }
  result.isDone = false;
  return result;
}

int getIndex(WireMap * map, char * key) {
  for (int i = 0; i < map->size; i++) {
    if(strcmp(map->keys[i], key) == 0 ) {
      return i;
    }
  }
  return -1;
}

void insert(WireMap * map, char * key, unsigned short value){
  int index = getIndex(map, key);
  if(index == -1){
    strcpy(map->keys[map->size], key);
    map->values[map->size] = value;
    map->size++;
  } else {
    map->values[index] = value;
  }
}

bool validate(unsigned short given, unsigned short expected){
  if(given != expected)
  {
    printf("calculation error: expected value %d, but was %d\n", expected, given);
    return false;
  }
  return true;
}

unsigned short getWireSignal(WireMap * map, char * wire){
  int index = getIndex(map, wire);
  if(index == -1){
    printf("hard error, the wire: %s is not defined in map\n", wire);
    exit(1);
  }
  return map->values[getIndex(map, wire)];
}

unsigned short calculateSignalForWire(char * inputFileName, char * wire, Instruction init){
  WireMap map = {0};
  unsigned short instructionsSize = 0;
  unsigned short doneInstructions = 0;
  Instruction instructions[MAX_INSTRUCTIONS_SIZE];
  FILE * file = fopen(inputFileName, "r");
  char line[50];
  if(!file){
    perror(inputFileName);
    exit(1);
  }

  if(init.target[0] != '\0'){
    instructions[instructionsSize++] = init;
  }

  while (fgets(line, sizeof(line), file)) {
    Instruction parsed = parseLine(line);
    if(init.target[0] != '\0' && strcmp(parsed.target, init.target) == 0){
      parsed.isDone = true;
    }
    instructions[instructionsSize++] = parsed;
  }

  while(instructionsSize != doneInstructions){
    bool isProgressMade = false;
    for(int i = 0; i < instructionsSize; i++){
      Instruction * instruction = &instructions[i];
      if(instruction->isDone) continue;
      switch (instruction->type) {
        case ASSIGN:
          if(instruction->right.type == WIRE && getIndex(&map, instruction->right.wire) == -1) continue;
          insert(&map, instruction->target, instruction->right.type == WIRE ? getWireSignal(&map, instruction->right.wire) : instruction->right.signal);
          instruction->isDone = true;
          doneInstructions++;
          isProgressMade = true;
          break;
        case AND:
          if(instruction->right.type == WIRE && getIndex(&map, instruction->right.wire) == -1) continue;
          if(instruction->left.type == WIRE && getIndex(&map, instruction->left.wire) == -1) continue;
          insert(
            &map,
            instruction->target,
            (instruction->left.type == WIRE ? getWireSignal(&map, instruction->left.wire) : instruction->left.signal)
            &
            (instruction->right.type == WIRE ? getWireSignal(&map, instruction->right.wire) : instruction->right.signal)
          );
          instruction->isDone = true;
          doneInstructions++;
          isProgressMade = true;
          break;
        case OR:
          if(instruction->right.type == WIRE && getIndex(&map, instruction->right.wire) == -1) continue;
          if(instruction->left.type == WIRE && getIndex(&map, instruction->left.wire) == -1) continue;
          insert(
            &map,
            instruction->target,
            (instruction->left.type == WIRE ? getWireSignal(&map, instruction->left.wire) : instruction->left.signal)
            |
            (instruction->right.type == WIRE ? getWireSignal(&map, instruction->right.wire) : instruction->right.signal)
          );
          instruction->isDone = true;
          doneInstructions++;
          isProgressMade = true;
          break;
        case LSHIFT:
          if(instruction->right.type == WIRE && getIndex(&map, instruction->right.wire) == -1) continue;
          insert(
            &map,
            instruction->target,
            getWireSignal(&map, instruction->right.wire) << instruction->shift
          );
          instruction->isDone = true;
          doneInstructions++;
          isProgressMade = true;
          break;
        case RSHIFT:
          if(instruction->right.type == WIRE && getIndex(&map, instruction->right.wire) == -1) continue;
          insert(
            &map,
            instruction->target,
            getWireSignal(&map, instruction->right.wire) >> instruction->shift
          );
          instruction->isDone = true;
          doneInstructions++;
          isProgressMade = true;
          break;
        case NOT:
          if(instruction->right.type == WIRE && getIndex(&map, instruction->right.wire) == -1) continue;
          insert(&map, instruction->target, ~getWireSignal(&map, instruction->right.wire));
          instruction->isDone = true;
          doneInstructions++;
          isProgressMade = true;
          break;
      }
    }
    printf("progress: %d done out of %d  (%d %%)\n", doneInstructions, instructionsSize, doneInstructions * 100 / instructionsSize);
    if(!isProgressMade) break;
  }
  fclose(file);
  return getWireSignal(&map, wire);
}

void part1(){
  bool validationResult =
    validate(calculateSignalForWire("test.txt", "d", (Instruction){0}), 72) &&
    validate(calculateSignalForWire("test.txt", "e", (Instruction){0}), 507) &&
    validate(calculateSignalForWire("test.txt", "f", (Instruction){0}), 492) &&
    validate(calculateSignalForWire("test.txt", "g", (Instruction){0}), 114) &&
    validate(calculateSignalForWire("test.txt", "h", (Instruction){0}), 65412) &&
    validate(calculateSignalForWire("test.txt", "i", (Instruction){0}), 65079) &&
    validate(calculateSignalForWire("test.txt", "x", (Instruction){0}), 123) &&
    validate(calculateSignalForWire("test.txt", "y", (Instruction){0}), 456);

  if(validationResult == true){
    int result = calculateSignalForWire("input.txt", "a", (Instruction){0});
    printf("day 07 part 01 result: %d\n", result);
  }
}

void part2(){
  unsigned short a = calculateSignalForWire("input.txt", "a", (Instruction){0});
  Instruction override = {0};
  override.type = ASSIGN;
  override.right.type = SIGNAL;
  override.right.signal = a;
  strcpy(override.target, "b");
  printf("day 07 part 02 result: %d\n", calculateSignalForWire("input.txt", "a", override));
}

int main() {
  part1();
  part2();
  return 0;
}
