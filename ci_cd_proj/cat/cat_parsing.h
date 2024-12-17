#ifndef CAT_PARSING_H
#define CAT_PARSING_H

#include <stdbool.h>
#include <stdio.h>

struct Options {
  bool show_nums;
  bool blank_no_number;
  bool blank_no_print;
  bool no_print_chars;
  bool print_tabs;
  bool print_newline;
  bool newline;
  int line_number;
  int file_index;
};

void p_hello(int num);
void show_opt(struct Options* opt);
int parsing_opt(int argc, char* argv[], struct Options* opt);
int check_opt(char* ch, struct Options* opt);
FILE* parsing_files(int argc, char* argv[], struct Options* opt);

#endif
