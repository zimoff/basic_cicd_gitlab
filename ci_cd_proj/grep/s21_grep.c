#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grep_lib.h"

// test
void file_proccessing(int* fileind, FILE* fp, char* argv[], Opt* opt,
                      Reg* myreg);
void show_nums(char* str);

int main(int argc, char* argv[]) {
  Opt opt = {
      .pattern_e = false,
      .ignore_register_i = false,
      .invert_search_v = false,
      .sum_matched_lines_c = false,
      .only_matched_files_l = false,
      .show_linenumber_n = false,
      .show_filenames_h = true,  // no filenames
      .dont_show_errors_s = false,
      .regex_from_file_f = false,
      .only_matched_patterns_o = false,
      .no_options = true,
      .line_counter = 0,
      .match_line_counter = 0,
      .match_file_counter = 0,
      .max_length = 0,
  };

  Reg myreg = {
      .pattern = NULL, .res = 0, .mode_comp = REG_EXTENDED, .mode_exec = 0};

  FILE* fp = NULL;
  int fileind = 0;
  int res = 0;

  res = parsing_flag_s(argc, argv, &opt);

  if ((res == 0) && (argc > 1) &&
      (parsing_opt(argc, argv, &opt, &myreg) == 0)) {
    if (myreg.pattern == NULL) {
      add_pattern(&myreg, argv[optind]);
      fileind = optind + 1;
    } else {
      fileind = optind;
    }

    if (fileind == (argc - 1)) opt.show_filenames_h = false;

    reg_comp(&myreg);

    while (argv[fileind] != NULL) {
      file_proccessing(&fileind, fp, argv, &opt, &myreg);
      fileind++;
    }

    regfree(&myreg.reg_pattern);
    free(myreg.pattern);

  } else if (!opt.dont_show_errors_s && res == 1) {
    fprintf(stderr, "%s: %s: No such file or directory(main)\n", argv[0],
            argv[fileind]);
  }

  return 0;
}

void file_proccessing(int* fileind, FILE* fp, char* argv[], Opt* opt,
                      Reg* myreg) {
  if ((fp = get_file(argv[*fileind])) != NULL) {
    opt->max_length = (get_max_length(fp) + 5);

    char str_bufer[opt->max_length];

    while (fgets(str_bufer, opt->max_length, fp) != NULL) {
      ++opt->line_counter;
      reg_proc(str_bufer, myreg);

      // invert_search_v
      if ((myreg->res == REG_NOMATCH) && (opt->invert_search_v)) {
        opt->match_line_counter++;

        if ((opt->sum_matched_lines_c) && (opt->only_matched_files_l)) break;

        if ((opt->show_filenames_h) &&
            (!opt->sum_matched_lines_c))  // show filenames
          printf("%s:", argv[*fileind]);  //$

        if (opt->sum_matched_lines_c == true) continue;

        if (opt->only_matched_files_l == true) {  // only matched files
          break;
        }

        if (opt->show_linenumber_n) printf("%d:", opt->line_counter);

        printf("%s", str_bufer);
      }

      // standard_search
      if ((myreg->res == 0) && (!opt->invert_search_v)) {
        opt->match_line_counter++;

        if ((opt->sum_matched_lines_c) && (opt->only_matched_files_l)) break;

        if ((opt->show_filenames_h) &&
            (!opt->sum_matched_lines_c))  // show filenames
          printf("%s:", argv[*fileind]);  //$

        if (opt->sum_matched_lines_c == true) continue;

        if (opt->only_matched_files_l == true) {
          break;
        }

        if (opt->show_linenumber_n) printf("%d:", opt->line_counter);

        if (opt->only_matched_patterns_o) {  // flag -o
          reg_nmatch_str(str_bufer, myreg);
        } else {
          printf("%s", str_bufer);
        }
      }
    }
    // end of while

    if ((opt->show_filenames_h) &&
        (opt->sum_matched_lines_c))   // show filenames
      printf("%s:", argv[*fileind]);  //&

    if (opt->sum_matched_lines_c == true) {
      printf("%d\n", opt->match_line_counter);
    }

    if (opt->only_matched_files_l && (!opt->match_line_counter == 0))
      printf("%s\n", argv[*fileind]);  //#

    if (opt->sum_matched_lines_c == true) {
      opt->match_line_counter = 0;
    }

    opt->line_counter = 0;

    fclose(fp);

  } else if (!opt->dont_show_errors_s) {
    fprintf(stderr, "%s: %s: No such file or directory(proc)\n", argv[0],
            argv[*fileind]);
  }
}

void show_nums(char* str) {
  char ch;
  for (int i = 0; (ch = *(str + i)) != '\0'; i++) {
    printf("%c ", ch);
  }

  printf("\n");
}
