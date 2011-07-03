#!/bin/sh
colorgcc -Wall -Wextra -pedantic -g -std=c99 -o main main.c pltoken.c test_pltoken.c && ./main
#colorgcc -Wall -Wextra -pedantic -g -std=c99 -o main main.c pltoken.c test_pltoken.c && valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./main
