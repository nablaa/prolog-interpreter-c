#ifndef TESTS_H
#define TESTS_H

#define RESET  "\x1b[0m"
#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"

#define SET(color) printf(color);

#define WITH_COLOR(color, statement) \
	SET(color);                  \
	statement;                   \
	SET(RESET);

#define TEST_BOOLEAN(expression, name)                           \
	do {                                                     \
		printf("%s ", name);                             \
		if (expression) {                                \
			WITH_COLOR(GREEN, printf("OK\n"));       \
		} else {                                         \
			WITH_COLOR(RED, printf("FAIL\n"));       \
		}                                                \
	} while (0);                                             \

#endif /* TESTS_H */
