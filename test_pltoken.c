/**
 * MIT License
 *
 * Copyright (c) 2022 Miika-Petteri Matikainen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "test_pltoken.h"

void test_token(const char *filename, int stop)
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
	test_token("sum.pl", 0);
	test_token("ackermann.pl", 0);
	test_token("facts.pl", 1);
	test_token("parse_test.pl", 0);
}

