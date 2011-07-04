#include "test_pltoken.h"

void test(const char *filename, int stop)
{
	WITH_COLOR(BLUE, printf("Testing tokens in file: %s\n", filename));

	FILE *file = fopen(filename, "r");
	if (!file) {
		WITH_COLOR(RED, printf("Could not open file: %s\n", filename));
		return;
	}

	PLToken *token = PLTokenise(file, stop);

	WITH_COLOR(YELLOW, print_tokens(token));

	PLTokensFree(token);
	fclose(file);
}

void print_tokens(PLToken *t)
{
	while (t) {
		switch (t->type) {
		case(PLTokenConstant):
		case(PLTokenVariable):
			printf("%s", t->value);
			break;
		case(PLTokenParenthesisOpen):
			printf("(");
			break;
		case(PLTokenParenthesisClose):
			printf(")");
			break;
		case(PLTokenComma):
			printf(", ");
			break;
		case(PLTokenPeriod):
			printf(".\n");
			break;
		case(PLTokenArrow):
			printf(" :- ");
			break;
		default:
			WITH_COLOR(RED, printf("Illegal token: %d\n", t->type));
		}
		t = t->next;
	}
	printf("\n");
}

void test_pltoken()
{
	WITH_COLOR(GREEN, printf("test_pltoken()\n"));
	test("sum.pl", 0);
	test("ackermann.pl", 1);
	test("facts.pl", 0);
}

