#include "cat_parsing.h"

#include <stdio.h>
#include <string.h>

void p_hello(int num) { printf("hello\n"); }

void show_opt(struct Options* opt) {
  printf("show_nums_n: %d\n", opt->show_nums);
  printf("blank_no_number_b: %d\n", opt->blank_no_number);
  printf("blank_no_print_s: %d\n", opt->blank_no_print);
  printf("no_print_chars_v: %d\n", opt->no_print_chars);
  printf("print_tabs_t: %d\n", opt->print_tabs);
  printf("print_newline_e: %d\n", opt->print_newline);
  printf("line number: %d \n", opt->line_number);
  printf("file_index: %d \n", opt->file_index);
}

FILE* parsing_files(int argc, char* argv[], struct Options* opt) {
  FILE* fp = NULL;

  while (opt->file_index < argc) {
    if (*argv[opt->file_index] == '-') {
      opt->file_index++;
      continue;
    } else {
      fp = fopen(argv[opt->file_index], "r");
      opt->file_index++;
      break;
    }
  }

  return fp;
}

void check_opt_long(char* str, struct Options* opt) {
  if (!strcmp(str, "--number-nonblank")) opt->blank_no_number = true;

  if (!strcmp(str, "--number")) opt->show_nums = true;

  if (!strcmp(str, "--squeeze-blank")) opt->blank_no_print = true;
}

int parsing_opt(int argc, char* argv[], struct Options* opt) {
  int len = 0;
  for (int i = 1; i < argc; i++) {
    if (*(argv[i] + 1) == '-') {
      check_opt_long(argv[i], opt);
      continue;
    }

    if (*(argv[i] + len) == '-') {
      while (*(argv[i] + ++len) != '\0') {
        check_opt((argv[i] + len), opt);
      }
      len = 0;
    }
  }
  if (opt->blank_no_number == true) opt->show_nums = false;

  return 0;
}

int check_opt(char* ch, struct Options* opt) {
  switch (*ch) {
    case 'n':
      opt->show_nums = true;
      break;
    case 'b':
      opt->blank_no_number = true;
      break;
    case 'v':
      opt->no_print_chars = true;
      break;
    case 'e':
      opt->print_newline = true;
      opt->no_print_chars = true;
      break;
    case 'E':
      opt->print_newline = true;
      break;
    case 's':
      opt->blank_no_print = true;
      break;
    case 't':
      opt->print_tabs = true;
      opt->no_print_chars = true;
      break;
    case 'T':
      opt->print_tabs = true;
      break;
  }
  return 0;
}
