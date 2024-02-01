#ifndef GREP_H_
#define GREP_H_
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NO_MATCH 0
#define MATCH 1
#define BUFFER_SIZE 10000

typedef struct inputData {
  unsigned int e, i, v, c, l, n, h, s, f, o, error, match, file_count;
  char pattern[BUFFER_SIZE];
} inputData;

typedef struct regstruct {
  regex_t regex;
  regmatch_t *pmatch;
} regstruct;

void parse(int argc, char *argv[], inputData *params);
void addPattern(inputData *params, char *pattern);
void addPatternFromFile(inputData *params, char *file_name);
void grepFile(int argc, char *argv[], inputData *params);
void fileWork(inputData *params, FILE *file, char *file_name);
int flagL(inputData *params, char *file_name);
void printData(inputData *params, char *line, regstruct reg, char *file_name,
               int line_count);
void printFileName(inputData *params, char *file_name);
void flagN(inputData *params, int line_count);
void flagC(inputData *params, int match_count, char *file_name);

#endif  // GREP_H_