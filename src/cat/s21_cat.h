#ifndef SRC_S21_CAT_H_
#define SRC_S21_CAT_H_

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
    int b, e, n, s, t, v, name_file;
};

int check_flags_linux(char *argv, struct flags *ptr);
int check_flags_bash(char *argv[], int i, int y, struct flags *ptr);
void output_file_with_flags(struct flags *ptr, char *str, int *numiration, int *check_s);
void check_file(char *argv[], struct flags *ptr, int *position_str);
void init_struct_flags(struct flags *ptr);

#endif  // SRC_S21_CAT_H_
