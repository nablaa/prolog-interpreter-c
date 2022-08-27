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
#include "test_plinterpreter.h"

#define MAX_LINE 1024

FILE *open_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		WITH_COLOR(RED, printf("Could not open file: %s\n", filename));
		exit(1);
		return NULL;
	}
	return file;
}

void print_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		WITH_COLOR(RED, printf("Could not open file: %s\n", filename));
		exit(1);
	}

	char buf[MAX_LINE];
	while (fgets(buf, MAX_LINE, file)) {
		WITH_COLOR(YELLOW, printf("%s", buf));
	}

	printf("\n");
	fclose(file);
}

void test_interpret(const char *database_filename, const char *query_filename,
                   int resolves, const char *solution)
{
	WITH_COLOR(BLUE, printf("--------------------\n"));

	FILE *db_file = open_file(database_filename);
	FILE *query_file = open_file(query_filename);

	printf("Database file:\n");
	print_file(database_filename);

	printf("Query file:\n");
	print_file(query_filename);

	PLHandleInput(db_file, query_file);

	if (resolves) {
		WITH_COLOR(BLUE, printf("Should resolve with resolvent:\n"));
		WITH_COLOR(YELLOW, printf("%s\n", solution));
	} else {
		WITH_COLOR(BLUE, printf("Should not resolve\n"));
	}

	fclose(db_file);
	fclose(query_file);
}

void test_plinterpreter()
{
	WITH_COLOR(GREEN, printf("test_plinterpreter()\n"));
	test_interpret("test_interpret_1_db.pl", "test_interpret_1_query.pl", 1, "add(s(0), s(0), s(s(0)))");
	test_interpret("test_interpret_2_db.pl", "test_interpret_2_query.pl", 1, "owner(john, cat)");
	test_interpret("test_interpret_3_db.pl", "test_interpret_3_query.pl", 1, "SEE FILE");
	test_interpret("test_interpret_4_db.pl", "test_interpret_4_query.pl", 1, "sibling(sally,erica).");
	test_interpret("quicksort.pl", "qs_query.pl", 1, "SEE FILE");
	test_interpret("ackermann.pl", "ack.pl", 1, "SEE FILE");
	test_interpret("parent.pl", "parent_q.pl", 1, "SEE FILE");
}

