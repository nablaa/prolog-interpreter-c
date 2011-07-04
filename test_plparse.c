#include "test_plparse.h"

void test_parse(const char *filename)
{
	WITH_COLOR(BLUE, printf("Testing parsing with file: %s\n", filename));

	FILE *file = fopen(filename, "r");
	if (!file) {
		WITH_COLOR(RED, printf("Could not open file: %s\n", filename));
		return;
	}

	PLToken *token = PLTokenise(file, 0);
	PLToken *remove = token;
	const PLToken **t = &token;
	PLTerm *term = PLParse(t);

	WITH_COLOR(YELLOW, PLTermPrint(term, stdout));
	printf("\n");

	PLTermFree(term);
	PLTokensFree(remove);
	fclose(file);
}

void test_plparse()
{
	WITH_COLOR(GREEN, printf("test_plparse()\n"));
	test_parse("sum.pl");
	test_parse("parse_test.pl");
}


