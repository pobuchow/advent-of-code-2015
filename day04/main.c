#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>

int mine(const char *str, int (*check)(const unsigned char *)) {
  int result = 0;
  unsigned char resultHash[MD5_DIGEST_LENGTH];
  while (1) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%s%d", str, result);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    MD5((const unsigned char *)buffer, strlen(buffer), resultHash);
    #pragma GCC diagnostic pop

    if (check(resultHash)) return result;

    result++;
  }

  return result;
}

int check5LeadingZeros(const unsigned char *hash) {
  return hash[0] == 0 && hash[1] == 0 && (hash[2] & 0b11110000) == 0;
}

int check6LeadingZeros(const unsigned char *hash) {
  return hash[0] == 0 && hash[1] == 0 && hash[2] == 0;
}

int minePart1(const char *str) {
  return mine(str, check5LeadingZeros);
}

int minePart2(const char *str) {
  return mine(str, check6LeadingZeros);
}

int validate(int (*validatedFunction)(const char *), const char *str, int expectedValue) {
  int result = validatedFunction(str);
  if (result != expectedValue) {
    printf("calculation error on %s, expected %d, but was %d\n", str, expectedValue, result);
    return 1;
  }
  return 0;
}

int calculateResult(int (*func)(const char *)) {
  FILE *file = fopen("input.txt", "r");
  char line[8196];
  if (!file) {
      perror("input.txt");
      exit(1);
  }
  if (!fgets(line, sizeof(line), file)) {
    perror("input.txt");
    fclose(file);
    exit(1);
  }
  fclose(file);

  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n') {
    line[len - 1] = '\0';
  }
  return func(line);
}

void part1() {
  int validationResult =
    validate(minePart1, "abcdef", 609043) +
    validate(minePart1, "pqrstuv", 1048970);

  if (validationResult == 0) {
    int result = calculateResult(minePart1);
    printf("day 01 part 01 result: %d\n", result);
  }
}

void part2() {
  int result = calculateResult(minePart2);
  printf("day 01 part 02 result: %d\n", result);
}

int main() {
  part1();
  part2();
  return 0;
}
