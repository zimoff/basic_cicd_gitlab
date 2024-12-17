#include "cat_output.h"

#include <stdio.h>

#include "cat_parsing.h"

void test_chars(FILE* fp)

{
  char ch;
  printf("test chars:\n");

  while ((ch = fgetc(fp)) != EOF) {
    printf("%d ", ch);
  }
}

void printed_opt(char ch, struct Options* opt) {
  if (opt->print_tabs == true && ch == '\t')
    printf("^I");
  else if (opt->print_newline == true && ch == '\n')
    printf("$%c", ch);
  else if (opt->no_print_chars && (ch < 32 && ch != 10 && ch != 9))
    printf("^%c", ch + 64);
  else if (opt->no_print_chars && ch == 127)
    printf("^?");
  else
    printf("%c", ch);
}

void multi_opt(FILE* fp, struct Options* opt) {
  char ch = 0;
  opt->newline = true;
  bool blankline = true;

  while ((ch = fgetc(fp)) != EOF) {
    if (opt->newline == true) {
      if (ch != '\n') blankline = true;

      if ((ch == '\n') && (opt->blank_no_print == true)) {  // squeeze_s
        if (blankline == true) {
          if (opt->show_nums == true) {
            printf("%6d\t\n", opt->line_number++);
          } else {
            printed_opt(ch, opt);  // options -e -t -v
          }
          blankline = false;
          continue;
        } else {
          continue;
        }
      }

      if (opt->show_nums == true) {  // show_nums mode -n
        printf("%6d\t", opt->line_number);
        printed_opt(ch, opt);  // options -e -t -v
        opt->line_number++;
      } else if (opt->blank_no_number == true) {  // blank_no_number mode -b
        if (ch != '\n') printf("%6d\t", opt->line_number++);
        printed_opt(ch, opt);  // options -e -t -v
      } else {                 // no flags mode
        printed_opt(ch, opt);  // options -e -t -v
        opt->line_number++;
      }

      if (ch != '\n')  // newline flag
        opt->newline = false;

    } else {
      printed_opt(ch, opt);  // options -e -t -v
      if (ch == '\n') opt->newline = true;
    }
  }
  fclose(fp);
}
