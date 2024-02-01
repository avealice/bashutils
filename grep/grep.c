#include "grep.h"

int main(int argc, char *argv[]) {
  inputData params = {0};
  if (argc > 2) {
    parse(argc, argv, &params);
    if (!params.error) grepFile(argc, argv, &params);
  } else
    fprintf(stderr, "Error: Incorrect usage\n");
  if (argc > 2 && strlen(params.pattern) == 0)
    fprintf(stderr, "Error: Incorrect usage\n");
  return 0;
}

void parse(int argc, char *argv[], inputData *params) {
  int opt;
  static const char *options = "e:ivclnhsf:o";
  while ((opt = getopt_long(argc, argv, options, NULL, NULL)) != -1) {
    if (opt == 'i')
      params->i = 1;
    else if (opt == 'v')
      params->v = 1;
    else if (opt == 'c')
      params->c = 1;
    else if (opt == 'l')
      params->l = 1;
    else if (opt == 'n')
      params->n = 1;
    else if (opt == 'h')
      params->h = 1;
    else if (opt == 's')
      params->s = 1;
    else if (opt == 'o')
      params->o = 1;
    else if (opt == 'e') {
      params->e = 1;
      addPattern(params, optarg);
    }

    else if (opt == 'f') {
      params->f = 1;
      addPatternFromFile(params, optarg);
    } else {
      params->error = 1;
    }
  }
  if (params->e == 0 && argc - optind > 1) {
    addPattern(params, argv[optind++]);
  }
  params->file_count = argc - optind;
}

void addPattern(inputData *params, char *pattern) {
  char buffer[BUFFER_SIZE] = {0};
  if (strlen(pattern) != 0) {
    if (strlen(params->pattern) == 0) {
      memcpy(params->pattern, pattern, strlen(pattern) + 1);
    } else {
      strcat(buffer, params->pattern);
      strcat(buffer, "|");
      strcat(buffer, pattern);
      memcpy(params->pattern, buffer, strlen(buffer));
    }
  } else {
    strcat(buffer, params->pattern);
    strcat(buffer, "|");
    strcat(buffer, "\n");
    memcpy(params->pattern, buffer, strlen(buffer));
  }
}

void addPatternFromFile(inputData *params, char *file_name) {
  FILE *fp;
  char str[BUFFER_SIZE] = {0};
  char *ch;
  fp = fopen(file_name, "r");
  if (fp != NULL) {
    while (!feof(fp)) {
      if (fgets(str, 1024, fp)) {
        if ((ch = strchr(str, '\n')) != NULL) {
          *ch = '\0';
        }
        addPattern(params, str);
      }
    }
    fclose(fp);
  } else {
    if (!params->s) fprintf(stderr, "Error: No such file or directory\n");
    params->error = 1;
  }
}

void grepFile(int argc, char *argv[], inputData *params) {
  for (int i = optind; i < argc; i++) {
    char *file_name = argv[i];
    FILE *file = fopen(file_name, "r");
    if (file) {
      fileWork(params, file, file_name);
      fclose(file);
    } else {
      if (!params->s) fprintf(stderr, "Error: No such file or directory\n");
    }
  }
}

void fileWork(inputData *params, FILE *file, char *file_name) {
  char *line = NULL;
  size_t len = 0;
  regex_t regex;
  regmatch_t pmatch[1];
  int cflags = params->i == 1 ? REG_ICASE | REG_EXTENDED | REG_NEWLINE
                              : REG_EXTENDED | REG_NEWLINE;
  int result = regcomp(&regex, params->pattern, cflags);
  if (!result) {
    params->match = NO_MATCH;
    regstruct reg = {.pmatch = pmatch, .regex = regex};
    ssize_t nread;
    int line_count = 0;
    int match_count = 0;
    while ((nread = getline(&line, &len, file)) != -1) {
      line_count++;
      result = regexec(&regex, line, 1, pmatch, 0);
      if (!result && !params->v) {
        params->match = MATCH;
        if (flagL(params, file_name)) break;
        if (!params->c) printData(params, line, reg, file_name, line_count);
        match_count++;
      } else if (result == REG_NOMATCH && params->v) {
        params->match = MATCH;
        if (flagL(params, file_name)) break;
        if (!params->c) printData(params, line, reg, file_name, line_count);
        match_count++;
      }
    }
    flagC(params, match_count, file_name);
    free(line);
  } else {
    if (!params->s) fprintf(stderr, "Error: Wrong expression\n");
  }
  regfree(&regex);
}

void flagC(inputData *params, int match_count, char *file_name) {
  if (params->c) {
    printFileName(params, file_name);
    if (!params->l) {
      printf("%d\n", match_count);
    } else if (params->l) {
      if (!params->match)
        printf("%d\n", params->match);
      else if (params->match)
        printf("%d\n%s\n", params->match, file_name);
    }
  }
}

int flagL(inputData *params, char *file_name) {
  int res = 0;
  if (params->l && !params->c) {
    printf("%s\n", file_name);
    res = 1;
  }
  return res;
}

void printData(inputData *params, char *line, regstruct reg, char *file_name,
               int line_count) {
  int enter = 0;
  if (params->o && !params->v) {
    while (regexec(&(reg.regex), line, 1, reg.pmatch, 0) == 0) {
      enter = 0;
      printFileName(params, file_name);
      flagN(params, line_count);
      for (int c = (reg.pmatch)[0].rm_so; c < (reg.pmatch)[0].rm_eo; c++)
        putchar(line[c]);
      if (line[(reg.pmatch)[0].rm_eo] != '\0') {
        putchar('\n');
        enter = 1;
      }
      line += (reg.pmatch)[0].rm_eo;
    }
  } else {
    printFileName(params, file_name);
    flagN(params, line_count);
    printf("%s", line);
  }
  if (line[strlen(line) - 1] != '\n' && !enter) printf("\n");
}

void printFileName(inputData *params, char *file_name) {
  if (params->file_count > 1 && !params->h) {
    printf("%s:", file_name);
  }
}

void flagN(inputData *params, int line_count) {
  if (params->n) printf("%d:", line_count);
}