#include "s21_cat.h"

int main(int argc, char *argv[]) {
    int *position_str = NULL;
    struct flags ptr;
    init_struct_flags(&ptr);
    if (argc > 1) {
        for (int i = 1; i < argc && !ptr.errors; i++) {
            for (int y = 0; argv[i][y]; y++) {
                if (argv[i][y] != '-') {
                    ptr.name_file += 1;
                    position_str =
                        realloc(position_str, ptr.name_file * sizeof(int));
                    position_str[ptr.name_file - 1] = i;
                    break;
                } else if (argv[i][y + 1] == '-') {
                    if (!check_flags_linux(argv[i], &ptr)) {
                        ptr.errors = 1;
                    }
                    break;
                } else {
                    for (int j = y + 1; argv[i][j] && !ptr.errors; j++) {
                        if (!check_flags_bash(argv, i, j, &ptr)) {
                            ptr.errors = 1;
                        }
                    }
                    break;
                }
            }
        }
        if (position_str) {
            check_file(argv, &ptr, position_str);
        } else {
            free(position_str);
        }
    }
    return 0;
}
int check_flags_linux(char *argv, struct flags *ptr) {
    int res = 0;
    if (!strcmp(argv, "--number-nonblank")) {
        ptr->b = 1;
        res = 1;
    } else if (!strcmp(argv, "--number")) {
        ptr->n = 1;
        res = 2;
    } else if (!strcmp(argv, "--squeeze-blank")) {
        ptr->s = 1;
        res = 3;
    } else {
        printf(
            "cat: illegal option -- %s\nusage: cat [-benstuv] [file "
            "...]\n",
            argv);
    }
    return res;
}

int check_flags_bash(char *argv[], int i, int y, struct flags *ptr) {
    int res = 0;
    if (argv[i][y] == 'b') {
        res = 1;
        ptr->b = 1;
    } else if (argv[i][y] == 'e') {
        ptr->e = 1;
        res = 2;
    } else if (argv[i][y] == 'v') {
        ptr->v = 1;
        res = 3;
    } else if (argv[i][y] == 'n') {
        ptr->n = 1;
        res = 4;
    } else if (argv[i][y] == 's') {
        ptr->s = 1;
        res = 5;
    } else if (argv[i][y] == 't') {
        ptr->t = 1;
        ptr->v = 1;
        res = 6;
    } else if (argv[i][y] == 'T') {
        ptr->t = 1;
        res = 7;
    } else if (argv[i][y] == 'E') {
        ptr->e = 1;
        res = 8;
    } else {
        printf(
            "cat: illegal option -- %c\nusage: cat [-benstuv] [file "
            "...]\n",
            argv[i][y]);
    }
    return res;
}
void init_struct_flags(struct flags *ptr) {
    ptr->b = 0;
    ptr->e = 0;
    ptr->n = 0;
    ptr->s = 0;
    ptr->t = 0;
    ptr->v = 0;
    ptr->name_file = 0;
    ptr->errors = 0;
}

void check_file(char *argv[], struct flags *ptr, int *position_str) {
    FILE *ptr_file;

    for (int i = 0; i < ptr->name_file; i++) {
        if ((ptr_file = fopen(argv[position_str[i]], "rt")) == NULL) {
            printf("cat: %s: No such file or directory\n",
                   argv[position_str[i]]);
        } else {
            if (!ptr->errors) {
                output_file_with_flags(ptr, ptr_file);
            }
            fclose(ptr_file);
        }
    }
    free(position_str);
}

void output_file_with_flags(struct flags *ptr, FILE *ptr_file) {
    int new_line = 1, count = 1, second_line = 1, third_line = 0, check_J = 0;
    char c = fgetc(ptr_file);
    while (!feof(ptr_file)) {
        if (ptr->s && new_line && c == '\n') {
            if (third_line) {
                third_line = -1;
            }
        }
        if (ptr->n && new_line && ptr->b != 1) {
            if (third_line != -1) {
                printf("%6d\t", count++);
            }
        }
        if (ptr->b && new_line && c != '\n') {
            printf("%6d\t", count++);
        }
        if (ptr->e && c == '\n') {
            if (third_line != -1) {
                printf("$");
            }
        }
        if (ptr->e || ptr->v) {
            int ch = (int)c;
            if (c < 0) {
                c &= 127;
                ch = (int)c;
                ch += 128;
            }
            if (ch != 9 && ch != 10 && ch < 32) {
                printf("^");
                c += 64;
            } else if (ch == 127) {
                printf("^");
                c = '?';
            } else if (ch > 127 && ch < 160) {
                printf("M-^");
                c = ch - 64;
                if (c == 'J' && (ptr->b || ptr->n)) {
                    printf("%c", c);
                    printf("%6d\t", count);
                    count += 1;
                    check_J = 1;
                }
            } else if (ch > 160 && ch <= 255) {
                c -= 128;
            }
        }
        if (ptr->t && c == '\t') {
            printf("^");
            c = 'I';
        }
        if (third_line != -1 && check_J != 1) {
            printf("%c", c);
        }
        new_line = (c == '\n') ? 1 : 0;
        third_line = (second_line && c == '\n') ? 1 : 0;
        second_line = (new_line && c == '\n') ? 1 : 0;
        check_J = 0;
        c = fgetc(ptr_file);
    }
}
