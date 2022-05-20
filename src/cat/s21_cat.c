#include "s21_cat.h"

int main(int argc, char *argv[]) {
    int *position_str = NULL;
    struct flags ptr;
    init_struct_flags(&ptr);
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            for (int y = 0; argv[i][y]; y++) {
                if (argv[i][y] != '-') {
                    ptr.name_file += 1;
                    position_str =
                        realloc(position_str, ptr.name_file * sizeof(int));
                    position_str[ptr.name_file - 1] = i;
                    break;
                } else if (argv[i][y + 1] == '-') {
                    check_flags_linux(argv[i], &ptr);
                    break;
                } else {
                    for (int j = y + 1; argv[i][j]; j++) {
                        if (!check_flags_bash(argv, i, j, &ptr)) {
                            printf("cat: неверный ключ — «%c»\n", argv[i][j]);
                        }
                    }
                    break;
                }
            }
        }
        if (position_str) {
            check_file(argv, &ptr, position_str);
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
        printf("cat: нераспознанный параметр «%s»\n", argv);
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
        ptr->v = 1;
        res = 2;
    } else if (argv[i][y] == 'n') {
        ptr->n = 1;
        res = 3;
    } else if (argv[i][y] == 's') {
        ptr->s = 1;
        res = 4;
    } else if (argv[i][y] == 't') {
        ptr->t = 1;
        ptr->v = 1;
        res = 5;
    } else if (argv[i][y] == 'T') {
        ptr->t = 1;
        res = 6;
    } else if (argv[i][y] == 'E') {
        ptr->e = 1;
        res = 7;
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
    ptr->check_s = 0;
    ptr->numiration = 1;
}

void check_file(char *argv[], struct flags *ptr, int *position_str) {
    FILE *ptr_file;
    char c;
    char *str = NULL;
    int j;
    int tmp = 0;

    for (int i = 0; i < ptr->name_file; i++) {
        if ((ptr_file = fopen(argv[position_str[i]], "r")) == NULL) {
            printf("cat: %s: Нет такого файла или каталога\n",
                   argv[position_str[i]]);
        } else {
            for (j = 0; (c = fgetc(ptr_file)) != EOF; j++) {
                str = realloc(str, (tmp + 1) * sizeof(char));
                str[tmp] = c;
                tmp++;
                if (c == '\n') {
                    str = realloc(str, (tmp + 1) * sizeof(char));
                    str[tmp + 1] = '\0';
                    output_file_with_flags(ptr, str);
                    tmp = 0;
                    str = NULL;
                }
            }
            if (str) {
                output_file_with_flags(ptr, str);
                str = NULL;
            }
            fclose(ptr_file);
            j = 0;
            tmp = 0;
        }
    }
    if (!str) {
        free(str);
    }
}

void output_file_with_flags(struct flags *ptr, char *str) {
    if (ptr->s) {
        if (check_null_or_empty_str(str)) {
            ptr->check_s = 0;
        } else {
            ptr->check_s += 1;
        }
        if (ptr->check_s > 1) {
            return;
        }
    }
    if (ptr->b) {
        if (check_null_or_empty_str(str)) {
            printf("%6d\t", ptr->numiration);
            ptr->numiration += 1;
        }
    }
    if (ptr->n && ptr->b == 0) {
        printf("%6d", ptr->numiration);
        ptr->numiration += 1;
    }
    for (int i = 0; str[i]; i++) {
        if (ptr->v == 1) {
            int ch = (int)str[i];
            if (str[i] < 0) {
                str[i] &= 127;
                ch = (int)str[i];
                ch += 128;
            }
            if (ch != 9 && ch != 10 && ch < 32) {
                printf("^");
                str[i] += 64;
            } else if (ch == 127) {
                printf("^");
                str[i] = '?';
            } else if (ch > 127 && ch < 160) {
                printf("M-^");
                str[i] = ch - 64;
                if (str[i] == 'J') {
                    printf("%c", str[i]);
                }
            } else if (ch > 160 && ch <= 255) {
                str[i] -= 128;
            }
        }
        if (ptr->t == 1 && str[i] == '\t') {
            printf("^");
            str[i] = 'I';
        }
        printf("%c", str[i]);
    }
}
int check_null_or_empty_str(char *str) {
    int check = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ' && str[i] != '\n') {
            check = 1;
        }
        if (str[i] == '\t') {
            check = 1;
        }
    }
    return check;
}