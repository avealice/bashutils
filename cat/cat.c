#include "cat.h"

int main(int argc, char *argv[]) {
  inputData params;
  initParams(&params);
  parseInput(argc, argv, &params);
  if (!params.error) {
    parseArgs(argc, argv, &params);
    if (!params.file && !params.error) printStdin(&params);
  }
  return 0;
}

void printStdin(inputData *params) {
  params->file = stdin;
  while (params->c != EOF) {
    printData(params);
  }
}

void initParams(inputData *params) {
  params->b = 0;
  params->e = 0;
  params->n = 0;
  params->s = 0;
  params->v = 0;
  params->t = 0;
  params->file = NULL;
  params->c = '\n';
  params->row = 1;
  params->end = 0;
  params->error = 0;
}

void parseInput(int argc, char *argv[], inputData *params) {
  int error = NoError;
  if (argv[1][0] == '-') {
    for (int i = 1; i < argc; i++) {
      if (checkFlags(argv[i]) == 1) {
        error = parseShortFlags(argv[i], params);
      } else if (checkFlags(argv[i]) == 2) {
        error = parseLongFlags(argv[i], params);
      }
    }
    if (params->b && params->n) params->n = 0;
    if (error) {
      errorMsg(error);
      params->error = 1;
    }
  }
}

void errorMsg(int error) {
  if (error == 0) {
    fprintf(stderr, "No errors\n");
  } else if (error == 1) {
    fprintf(stderr, "Error: Illegal option\n");
  } else if (error == 2) {
    fprintf(stderr, "Error: No such file or directory\n");
  }
}

int parseShortFlags(char *str, inputData *params) {
  int error = NoError;
  str++;
  while (*str != '\0') {
    if (*str == 'b') {
      params->b = 1;
    } else if (*str == 'e') {
      params->e = 1;
      params->v = 1;
    } else if (*str == 'E') {
      params->e = 1;
    } else if (*str == 'v') {
      params->v = 1;
    } else if (*str == 'n') {
      params->n = 1;
    } else if (*str == 's') {
      params->s = 1;
    } else if (*str == 't') {
      params->t = 1;
      params->v = 1;
    } else if (*str == 'T') {
      params->t = 1;
    } else {
      error = FlagDoesntExist;
    }
    str++;
  }
  return error;
}

int parseLongFlags(char *str, inputData *params) {
  int error = NoError;
  str += 2;
  if (strcmp(str, "number-nonblank") == 0) {
    params->b = 1;
  } else if (strcmp(str, "number") == 0) {
    params->n = 1;
  } else if (strcmp(str, "squeeze-blank") == 0) {
    params->s = 1;
  } else {
    error = FlagDoesntExist;
  }
  return error;
}

int checkFlags(char *arg) {
  int res = 0;
  int len = strlen(arg);
  int err = strcmp(arg, "--");
  if (arg[0] == '-') {
    if (arg[1] != '-' && len > 1) {
      res = 1;
    } else if (arg[1] == '-' && len > 2) {
      res = 2;
    } else if (arg[1] == '-' && len == 2) {
      res = 3;
    }
  }
  return res;
}

void parseArgs(int argc, char *argv[], inputData *params) {
  if (!params->error) {
    for (int i = 1; i < argc; i++) {
      if (checkFlags(argv[i]) == 0) {
        FILE *file = fopen(argv[i], "r");
        if (file) {
          params->file = file;
          printData(params);
        } else {
          errorMsg(FileDoesntExist);
          params->error = 1;
          break;
        }
        if (file) fclose(file);
      }
    }
  }
}

void printData(inputData *params) {
  params->row = 1;
  params->c = '\n';
  int count_c = 0;
  while (1) {
    int skip = 0;
    int prev_c = params->c;
    params->c = fgetc(params->file);
    if (params->c == EOF) break;
    if (prev_c == '\n' && params->c == '\n' && params->s) {
      (params->end)++;
      if (params->end > 1) continue;
    } else {
      params->end = 0;
    }
    if (prev_c == '\n' && ((params->b && params->c != '\n') || params->n))
      printf("%6u\t", (params->row)++);
    if (params->t && params->c == '\t') {
      printf("^I");
      skip = 1;
    }
    if (params->e && params->b && count_c == 0 && params->c == '\n') {
      printf("      \t$");
    } else if (params->e && params->c == '\n') {
      printf("$");
    }
    if (params->v && params->c != '\n' && params->c != '\t') {
      if (params->c < 32) {
        params->c += 64;
        printf("^");
      } else if (params->c > 127 && params->c < 160) {
        printf("M-^");
        params->c -= 64;
      } else if (params->c == 127) {
        printf("^");
        params->c -= 64;
      }
    }
    if (skip != 1) printf("%c", params->c);
    if (params->c == '\n') {
      count_c = 0;
    } else {
      count_c++;
    }
  }
}
