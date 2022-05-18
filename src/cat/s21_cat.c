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
                        realloc(position_str, (ptr.name_file) * sizeof(int));
                    position_str[ptr.name_file - 1] = i;
                    break;
                } else if (check_flags_linux(argv[i], &ptr) && ptr.name_file == 0) {
                    break;
                } else if(ptr.name_file == 0) {
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
}

void check_file(char *argv[], struct flags *ptr, int *position_str) {
    FILE *ptr_file;
    char c;
    char *str = NULL;
    int numiration = 1;
    int j;
    for (int i = 0; i < ptr->name_file; i++) {
        if ((ptr_file = fopen(argv[position_str[i]], "r")) == NULL) {
            printf("ERROR\n");
        } else {
            for (j = 0; (c = fgetc(ptr_file)) != EOF; j++) {
                str = realloc(str, (j + 1) * sizeof(char));
                str[j] = c;
            }
            str[j] = '\0';

            output_file_with_flags(ptr, str, &numiration);
        }
    }

    
}

void output_file_with_flags(struct flags *ptr, char *str, int *numiration) {
    int check = 1;
    if (ptr->s) {
        for (int i = 0; str[i]; i++) {
            if (str[i] != ' ') {
                check = 0;
            }
        }
        if (check) {
            return;
        }
    }
    if (ptr->b) {
        for (int i = 0; str[i]; i++) {
            if (str[i] != ' ') {
                printf("     %d", *numiration);
                break;
            }
        }
    }
    if (ptr->n && ptr->b == 0) {
        printf("     %d", *numiration);
    }
    printf("%s", str);
}
