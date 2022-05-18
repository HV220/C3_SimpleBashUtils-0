#include "s21_cat.h"

int main(int argc, char* argv[]) {
   int tmp = 0, check = 1;
   char **p_str = NULL;
   char *str = NULL;
   struct flags ptr;
   init_struct_flags(&ptr);
   if (argc > 0)
    {
        for (int i = 1; i < argc; i++)
        {
            for (int y = 0; argv[i][y]; y++)
            {
                    if (argv[i][y] != '-')
                    {
                        ptr.name_file += 1;
                        p_str = realloc(p_str, ptr.name_file * sizeof(char*));
                        
                        for (int k = 0; k < ptr.name_file; k++)
                        {
                            p_str[i] = realloc(str, tmp * sizeof(char));
                        }
                        
                        // for (int j = y; argv[i][j]; j++)
                        // {
                        //   if (str = realloc(str, (tmp + 1) * sizeof(char)))
                        //     {
                        //         str[tmp] = argv[i][j];
                        //         tmp++;
                        //     } else {
                        //         if (str != NULL) {
                        //             free(str);
                        //         }
                        //         exit(2);
                        //         printf("memory error");
                        //     } }                                  
                             break;
                    } else if (check_flags_linux(argv[i], &ptr)) {
                     break;
                    } else
                    {
                        for (int j = y + 1; argv[i][j]; j++)
                        {
                            if (!check_flags_bash(argv, i, j, &ptr)) {
                                printf("cat: неверный ключ — «%c»\n", argv[i][j]);
                                exit(1);
                            }
                        }
                        break;
                    } 
                }    
        }
    }
    return 0;
}
int check_flags_linux(char *argv, struct flags *ptr) {
    int res = 0;
    if (!strcmp(argv, "--number-nonblank"))
    {
       ptr->b = 1;
       res = 1;
    } else if (!strcmp(argv, "--number"))
    {
        ptr->n = 1;
       res = 2;
    } else if (!strcmp(argv, "--squeeze-blank"))
    {
        ptr->s = 1;
       res = 3;
    } else {
        printf("cat: нераспознанный параметр «%s»\n", argv);
        exit(3);
    }
    return res;    
}

int check_flags_bash(char *argv[], int i, int y, struct flags *ptr) {
    int res = 0;
    if (argv[i][y] == 'b')
    {
       res = 1;
       ptr->b = 1;
    } else if (argv[i][y] == 'e')
    {
       ptr->e = 1;
       ptr->v = 1;
       res = 2;
    } else if (argv[i][y] == 'n')
    {
       ptr->n = 1;
       res = 3;
    }
    else if (argv[i][y] == 's')
    {
       ptr->s = 1;
       res = 4;
    }
    else if (argv[i][y] == 't')
    {
       ptr->t = 1;
       ptr->v = 1;
       res = 5;
    }
    else if (argv[i][y] == 'T')
    {
       ptr->t = 1;
       res = 6;
    }
    else if (argv[i][y] == 'E')
    {
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
char*** realisation_str(int n) {
    char *** temp = (char ***) malloc(n * sizeof(char **));
    for (int i=0; i < n; i++) {
        temp[i] = (char **) malloc(n * sizeof(char*));
        for (int j = 0; j < n; j++)
            temp[i][j] = (char *) malloc(n * sizeof(char*));   
    }

    return temp;
}
