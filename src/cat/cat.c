#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct format {
    int b;
    int e;
    int n;
    int s;
    int t;
    int T;
    int E;
};

int check_flags(char* str);
void init(struct format* S);

int main(int argc, char* argv[]) {
    char* str;
    int tmp, y = 0;
    struct format S;
    init(&S);
    for (int i = 1; i < argc; i++) {
        str = argv[i];
        if (strstr(argv[i], "--number-nonblank") && (strlen(argv[i])) == 17) {
            S.b = 1;
            printf("test1");
        } else if (strstr(argv[i], "--number") && (strlen(argv[i])) == 8) {
            S.n = 1;
            printf("test2");
        } else if (strstr(argv[i], "--squeeze-blank") &&
                   strlen(argv[i]) == 15) {
            printf("test3");
            S.s = 1;
        } else if (*str == '-') {
            printf("test4");
        }
        else {
            printf("cat: illegal option -- -");
            exit(1);
        }
         for (tmp = 0; str[tmp]; tmp++) {
                if (str[tmp] == '-') {
                    // if (check_flags(str + tmp + 1)) {
                    //     printf("goood");
                    // } else {
                    //     printf("no");
                    // }
                }
            } 
        tmp = 0;  
    }
    return 0;
}

int check_flags(char* str) {
    int check = 1;
    switch (*str) {
        case 'b':
            break;
        case 'v':
            break;
        case 'e' || 'E':
            break;
        case 'n':
            break;
        case 't':
            break;
        case 's':
            break;
        default:
            check = 0;
            break;
    }
    return check;
}

void init(struct format* S) {
    S->b = 0;
    S->e = 0;
    S->n = 0;
    S->s = 0;
    S->t = 0;
}