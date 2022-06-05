.PHONY: all clean test check rebuild s21_cat
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: clean s21_cat

s21_cat:
	${CC} ${CFLAGS} s21_cat.c -o s21_cat

s21_cat_test:
	${CC} ${CFLAGS} -fprofile-arcs -ftest-coverage ./s21_cat_test.c -o s21_cat_test

clean:
	rm -rf s21_cat.d* s21_cat_test.d* s21_cat s21_cat_test s21_cat_test.g* ../common/test*.txt

check: s21_cat s21_cat_test
	cppcheck --enable=all --suppress=missingIncludeSystem *.c *.h
	python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h
	CK_FORK=no leaks -atExit -- ./s21_cat
	CK_FORK=no leaks -atExit -- ./s21_cat_test

test: s21_cat s21_cat_test
	./s21_cat_test
	make clean

rebuild: clean all