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

#endif /* TESTS_H */
