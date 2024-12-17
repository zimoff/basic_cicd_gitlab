#include <stdbool.h>
#include <stdio.h>

#include "cat_output.h"
#include "cat_parsing.h"

int main(int argc, char *argv[]) {
  struct Options opt = {.file_index = 1, .line_number = 1};

  FILE *fp;

  parsing_opt(argc, argv, &opt);
  do {
    fp = parsing_files(argc, argv, &opt);
    if (fp != NULL) multi_opt(fp, &opt);

  } while (fp != NULL);

  return 0;
}
