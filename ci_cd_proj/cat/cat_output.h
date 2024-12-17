#ifndef CAT_OUTPUT_H
#define CAT_OUTPUT_H

#include <stdbool.h>
#include <stdio.h>

#include "cat_parsing.h"

void multi_opt(FILE *fp, struct Options *opt);

void test_chars(FILE *fp);

#endif
