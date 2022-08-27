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
#include "test_plterm.h"

void run_tests(PLTerm *t)
{
	TEST_BOOLEAN(PLTermIsCompound(t), "PLTermIsCompound");
	TEST_BOOLEAN(!PLTermIsVariable(t), "!PLTermIsVariable");
	TEST_BOOLEAN(!PLTermIsConstant(t), "!PLTermIsConstant");
	TEST_BOOLEAN(PLTermArity(t) == 3, "PLTermArity == 3");
	TEST_BOOLEAN(PLVariableOccurs("N", t), "PLVariableOccurs N");
	TEST_BOOLEAN(PLVariableOccurs("M", t), "PLVariableOccurs M");
	TEST_BOOLEAN(PLVariableOccurs("I", t), "PLVariableOccurs I");
	TEST_BOOLEAN(!PLVariableOccurs("K", t), "!PLVariableOccurs K");
	TEST_BOOLEAN(PLTermCompatible(t, t), "PLTermCompatible(t, t)");
	TEST_BOOLEAN(!PLTermCompatible(t, t->datum.compoundTerm.arguments), "!PLTermCompatible(t, t->datum.compoundTerm.arguments)");
	TEST_BOOLEAN(PLTermEqual(t, t), "PLTermEqual(t, t)");
	TEST_BOOLEAN(!PLTermEqual(t, t->datum.compoundTerm.arguments), "!PLTermEqual(t, t->datum.compoundTerm.arguments)");

	PLTerm *copy = PLTermCopy(t);
	TEST_BOOLEAN(PLTermEqual(t, copy), "PLTermEqual(t, copy)");
	printf("Copy:\n");
	WITH_COLOR(YELLOW, PLTermPrint(copy, stdout));

	PLTermRenameVariables(t);
	PLTermRenameVariables(copy);
	printf("After renames:\n");
	WITH_COLOR(YELLOW, PLTermPrint(t, stdout));
	WITH_COLOR(YELLOW, PLTermPrint(copy, stdout));
	TEST_BOOLEAN(!PLTermEqual(t, copy), "!PLTermEqual(t, copy)");
	PLTermFree(copy);
}

void test_term(const char *filename)
{
	WITH_COLOR(BLUE, printf("Testing terms in file: %s\n", filename));

	FILE *file = fopen(filename, "r");
	if (!file) {
		WITH_COLOR(RED, printf("Could not open file: %s\n", filename));
		return;
	}

	PLToken *token = PLTokenise(file, 0);
	PLToken *rem = token;
	const PLToken *t = token;
	PLTerm *term = PLParse(&t);
	WITH_COLOR(YELLOW, PLTermPrint(term, stdout));
	printf("\n");

	run_tests(term);
	printf("\n");

	PLTermFree(term);
	PLTokensFree(rem);
	fclose(file);
}

void test_plterm()
{
	WITH_COLOR(GREEN, printf("test_plterm()\n"));
	test_term("test_1.pl");

}


