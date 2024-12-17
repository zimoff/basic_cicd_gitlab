#ifndef GREP_LIB_H
#define GREP_LIB_H
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Options {
  bool pattern_e;
  bool ignore_register_i;
  bool invert_search_v;
  bool sum_matched_lines_c;
  bool only_matched_files_l;
  bool show_linenumber_n;
  bool show_filenames_h;
  bool dont_show_errors_s;
  bool regex_from_file_f;
  bool only_matched_patterns_o;
  bool no_options;
  int line_counter;
  int max_length;
  int match_line_counter;
  int match_file_counter;

} Opt;

typedef struct my_reg {
  regex_t reg_pattern;
  regmatch_t match_arr[5];
  char* pattern;
  int res;
  char errorbuf[300];
  int mode_comp;
  int mode_exec;
} Reg;

void show_opt(Opt* opt);
void show_args(int argc, char* argv[]);

int parsing_flag_s(int argc, char* argv[], Opt* opt);
int parsing_opt(int argc, char* argv[], Opt* opt, Reg* myreg);
int get_max_length(FILE* fp);
FILE* get_file(char* filename);

int reg_proc(char* str_buf, Reg* myreg);
int reg_comp(Reg* myreg);
void reg_nmatch_str(char* str_buf, Reg* myreg);
void show_match(char* str_buf, Reg* myreg);

char* add_pattern(Reg* myreg, const char* str);
int get_file_opt(char* argv[], char* filename, Reg* myreg, Opt* opt);

#endif
