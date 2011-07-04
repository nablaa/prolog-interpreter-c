#!/bin/sh
#colorgcc -Wall -Wextra -pedantic -g -std=c99 -o main *.c && ./main
colorgcc -Wall -Wextra -pedantic -g -std=c99 -o main *.c && valgrind --tool=memcheck --leak-check=full --show-reachable=yes ./main
