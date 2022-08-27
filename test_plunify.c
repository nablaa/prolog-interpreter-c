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
#include "test_plunify.h"

PLTerm *load_term(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		WITH_COLOR(RED, printf("Could not open file: %s\n", filename));
		exit(1);
		return NULL;
	}

	PLToken *token = PLTokenise(file, 0);
	PLToken *rem = token;
	const PLToken *t = token;
	PLTerm *term = PLParse(&t);
	PLTokensFree(rem);
	fclose(file);
	return term;
}

void print_unifier(const PLUnifier *u)
{
	printf("{");
	while (u) {
		printf("%s = ", u->variable);
		PLTermPrint(u->term, stdout);
		printf(", ");
		u = u->next;
	}
	printf("}\n");
}

void test_unification(const char *filename1, const char *filename2, int should_unify, const char *correct_unifier)
{
	WITH_COLOR(BLUE, printf("--------------------\n"));

	PLTerm *t1 = load_term(filename1);
	PLTerm *t2 = load_term(filename2);

	printf("T1:\t");
	WITH_COLOR(YELLOW, PLTermPrint(t1, stdout));
	printf("T2:\t");
	WITH_COLOR(YELLOW, PLTermPrint(t2, stdout));
	printf("\n");

	PLUnifier *mgu;
	int status = PLUnify(t1, t2, &mgu);

	if (!status) {
		if (should_unify) {
			WITH_COLOR(GREEN, printf("Unifies\n"));
		} else {
			WITH_COLOR(RED, printf("Should not unify!\n"));
		}

		printf("Unified with MGU:\n");
		WITH_COLOR(YELLOW, print_unifier(mgu));

		printf("The correct MGU is:\n");
		WITH_COLOR(YELLOW, printf("%s\n", correct_unifier));
	} else {
		if (should_unify) {
			WITH_COLOR(RED, printf("Should unify with:\n"));
			WITH_COLOR(YELLOW, printf("%s\n", correct_unifier));
		} else {
			WITH_COLOR(GREEN, printf("Does not unify\n"));
		}
	}

	printf("\n");

	PLUnifierFree(mgu);
	PLTermFree(t1);
	PLTermFree(t2);
}

void test_termapply(const char *t_filename, const char *u_filename, char *variable, const char *result)
{
	WITH_COLOR(BLUE, printf("test_termapply\n"));
	PLTerm *t = load_term(t_filename);
	PLTerm *u = load_term(u_filename);

	printf("t:\t");
	WITH_COLOR(YELLOW, PLTermPrint(t, stdout));
	printf("u:\t");
	WITH_COLOR(YELLOW, PLTermPrint(u, stdout));
	printf("\n");

	PLUnifier *unifier = PLUnifierCreate(u, variable);
	PLUnifierApplyToTerms(&t, unifier);
	PLUnifierFree(unifier);

	printf("t2:\t");
	WITH_COLOR(YELLOW, PLTermPrint(t, stdout));

	printf("Should be:\n\t");
	WITH_COLOR(YELLOW, printf("%s\n", result));

	PLTermFree(t);
	PLTermFree(u);
}

void test_plunify()
{
	WITH_COLOR(GREEN, printf("test_plunify()\n"));
	test_unification("unify_test_1.pl", "unify_test_1.pl", 1, "{}");
	test_unification("unify_test_1.pl", "unify_test_2.pl", 1, "{Number = 1}");
	test_unification("unify_test_3.pl", "unify_test_4.pl", 1, "{List = list(b, list(c, last))}");
	test_unification("unify_test_5.pl", "unify_test_6.pl", 1, "{A = list(a, X), Y = list(b, C)}");
	test_unification("unify_test_7.pl", "unify_test_8.pl", 0, "");
	test_unification("unify_test_9.pl", "unify_test_10.pl", 0, "");
	test_unification("unify_test_11.pl", "unify_test_12.pl", 0, "");

	test_termapply("apply_1_t.pl", "apply_1_u.pl", "N", "sum(s(done),hello(done,M)):-sum(M,hello(done,done))");
}
