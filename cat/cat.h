#ifndef CAT_H_
#define CAT_H_

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum errors { NoError, FlagDoesntExist, FileDoesntExist };

typedef struct inputData {
  unsigned int b, e, n, s, t, v, error, row, end;
  int c;
  FILE *file;
} inputData;

void initParams(inputData *params);
void parseInput(int argc, char *argv[], inputData *params);
int checkFlags(char *arg);
int parseShortFlags(char *str, inputData *params);
int parseLongFlags(char *str, inputData *params);
void parseArgs(int argc, char *argv[], inputData *params);
void printData(inputData *params);
void errorMsg(int error);
void printStdin(inputData *params);

#endif  // CAT_H_
