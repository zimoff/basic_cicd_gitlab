#include "grep_lib.h"

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_opt(Opt* opt) {
  printf("pattern_e: %d \n", opt->pattern_e);
  printf("ignore_register_i: %d \n", opt->ignore_register_i);
  printf("invert_search_v: %d \n", opt->invert_search_v);
  printf("only_matched_lines_c: %d \n", opt->sum_matched_lines_c);
  printf("only_matched_files_l: %d \n", opt->only_matched_files_l);
  printf("show_linenumber_n: %d \n", opt->show_linenumber_n);
  printf("show_filenames_h: %d \n", opt->show_filenames_h);
  printf("dont_show_errors_s: %d \n", opt->dont_show_errors_s);
  printf("regex_from_file_f: %d \n", opt->regex_from_file_f);
  printf("sum_matched_lines_c: %d \n", opt->sum_matched_lines_c);
  printf("no_options: %d \n", opt->no_options);
  printf("line_counter: %d \n", opt->line_counter);
  printf("max_length: %d \n", opt->max_length);
  printf("match_line_counter: %d \n", opt->match_line_counter);
}

void show_args(int argc, char* argv[]) {
  printf("show args: \n");
  for (int i = 0; i < argc; i++) printf("argv: %s \n", argv[i]);
}
int parsing_flag_s(int argc, char* argv[], Opt* opt) {
  int key;
  char opt_str[] = "e:ivclnhsf:o";
  int res = 0;
  opterr = 0;

  while ((key = getopt_long(argc, argv, opt_str, NULL, NULL)) != -1) {
    switch (key) {
      case 's':
        opt->dont_show_errors_s = true;
        break;
    }
  }

  if (argc == optind) {
    res = 1;
    if (!opt->dont_show_errors_s)
      fprintf(stderr, "%s: there is no aguments\n", argv[0]);
  }
  optind = 1;
  opterr = 1;
  return res;
}

int parsing_opt(int argc, char* argv[], Opt* opt, Reg* myreg) {
  int key;
  int res = 0;
  char opt_str[] = "e:ivclnhsf:o";

  while ((key = getopt_long(argc, argv, opt_str, NULL, NULL)) != -1) {
    switch (key) {
      case 'e':
        add_pattern(myreg, optarg);
        opt->pattern_e = true;
        opt->no_options = false;
        break;
      case 'i':
        opt->ignore_register_i = true;
        myreg->mode_comp = REG_EXTENDED | REG_ICASE;
        opt->no_options = false;
        break;
      case 'v':
        opt->invert_search_v = true;
        opt->no_options = false;
        break;
      case 'c':
        opt->sum_matched_lines_c = true;
        opt->no_options = false;
        break;
      case 'l':
        opt->only_matched_files_l = true;
        opt->no_options = false;
        break;
      case 'n':
        opt->show_linenumber_n = true;
        opt->no_options = false;
        break;
      case 'h':
        opt->show_filenames_h = false;
        opt->no_options = false;
        break;
      case 's':
        opt->dont_show_errors_s = true;
        opt->no_options = false;
        break;
      case 'f':
        res = get_file_opt(argv, optarg, myreg, opt);
        opt->regex_from_file_f = true;
        opt->no_options = false;
        break;
      case 'o':
        opt->only_matched_patterns_o = true;
        opt->no_options = false;
        break;
      case '?':
        printf("unknown key: %c \n", key);
        res = 1;
        break;
      case ':':
        printf("there is no parameters: %c \n", key);
        res = 1;
        break;
    }
  }
  return res;
}

FILE* get_file(char* filename) {
  FILE* fp;

  fp = fopen(filename, "r");

  return fp;
}

int get_file_opt(char* argv[], char* filename, Reg* myreg, Opt* opt) {
  FILE* fp;
  int res = 0;

  int max_length;

  if ((fp = fopen(filename, "r")) != NULL) {
    max_length = get_max_length(fp);

    char str_bufer[max_length];

    while (fscanf(fp, "%s", str_bufer) != EOF) {
      add_pattern(myreg, str_bufer);
    }
    fclose(fp);
  } else {
    res = 1;
    if (!opt->dont_show_errors_s)
      fprintf(stderr, "%s: %s: No such file or directory(get_file_opt)\n",
              argv[0], filename);
  }

  return res;
}

int get_max_length(FILE* fp) {
  int max_length = 0;
  int length = 0;
  char ch;

  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n') {
      length++;
      if (length > max_length) max_length = length;
      length = 0;
    } else {
      length++;
    }
  }

  rewind(fp);

  return max_length;
}

int reg_proc(char* str_buf, Reg* myreg) {
  if (myreg->res == 0 || myreg->res == REG_NOMATCH) {
    myreg->res = regexec(&myreg->reg_pattern, str_buf, 5, myreg->match_arr,
                         myreg->mode_exec);
  } else {
    regerror(myreg->res, &myreg->reg_pattern, myreg->errorbuf, 300);
    printf("proc_res: %d| %s\n", myreg->res, myreg->errorbuf);
  }

  return myreg->res;
}

int reg_comp(Reg* myreg) {
  myreg->res = regcomp(&myreg->reg_pattern, myreg->pattern, myreg->mode_comp);
  if (myreg->res != 0) {
    printf("comp_fail: %d\n", myreg->res);
    regerror(myreg->res, &myreg->reg_pattern, myreg->errorbuf, 300);
    printf("res: %d| %s\n", myreg->res, myreg->errorbuf);
  }

  return myreg->res;
}

void reg_nmatch_str(char* str_buf, Reg* myreg) {
  int offset = 0;

  myreg->res = regexec(&myreg->reg_pattern, str_buf + offset, 5,
                       myreg->match_arr, myreg->mode_exec);

  while ((strlen(str_buf + offset) != 0) && (myreg->res != REG_NOMATCH)) {
    show_match(str_buf + offset, myreg);

    offset += myreg->match_arr[0].rm_eo;

    myreg->res = regexec(&myreg->reg_pattern, str_buf + offset, 5,
                         myreg->match_arr, myreg->mode_exec);
  }
}

void show_match(char* str_buf, Reg* myreg) {
  for (int j = myreg->match_arr[0].rm_so; j < myreg->match_arr[0].rm_eo; j++)
    printf("%c", *(str_buf + j));
  printf("\n");
}

char* add_pattern(Reg* myreg, const char* str) {
  const char bd[] = "|";

  if (myreg->pattern == NULL) {
    myreg->pattern = calloc(strlen(str) + 1, sizeof(char));
    strcpy(myreg->pattern, str);
  } else {
    size_t new_size = strlen(myreg->pattern) + strlen(bd) + strlen(str) + 1;
    myreg->pattern = realloc(myreg->pattern, new_size);
    strncat(myreg->pattern, bd, strlen(bd));
    strncat(myreg->pattern, str, strlen(str));
  }

  return myreg->pattern;
}
