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
#include <assert.h>
#include "plterm.h"

#define EXTRA_SIZE 16

int PLTermIsCompound(const PLTerm *t)
{
	assert(t);
	return t->type == PLTermCompound;
}

int PLTermIsVariable(const PLTerm *t)
{
	assert(t);
	return t->type == PLTermVariable;
}

int PLTermIsConstant(const PLTerm *t)
{
	assert(t);
	return (PLTermIsCompound(t) && !PLTermArity(t) && !t->datum.compoundTerm.body);
}

int PLTermArity(const PLTerm *t)
{
	assert(t);
	int n = 0;
	for (t = t->datum.compoundTerm.arguments; t; t = t->next, ++n);
	return n;
}

int PLVariableOccurs(PLVariable v, const PLTerm *t)
{
	assert(t);
	assert(v);
	if (PLTermIsVariable(t)) {
		return !strcmp(t->datum.variable, v);
	}

	for (PLTerm *arg = t->datum.compoundTerm.arguments; arg; arg = arg->next) {
		if (PLVariableOccurs(v, arg)) {
			return 1;
		}
	}
	for (PLTerm *body = t->datum.compoundTerm.body; body; body = body->next) {
		if (PLVariableOccurs(v, body)) {
			return 1;
		}
	}

	return 0;
}

int PLTermEqual(const PLTerm *t1, const PLTerm *t2)
{
	assert(t1);
	assert(t2);
	if (t1->type != t2->type) {
		return 0;
	}

	if (PLTermIsVariable(t1)) {
		return !strcmp(t1->datum.variable, t2->datum.variable);
	}

	if (PLTermIsConstant(t1)) {
		if (!PLTermIsConstant(t2)) {
			return 0;
		}
		return !strcmp(t1->datum.compoundTerm.name, t2->datum.compoundTerm.name);
	}

	if (strcmp(t1->datum.compoundTerm.name, t2->datum.compoundTerm.name)) {
		return 0;
	}

	if (PLTermArity(t1) != PLTermArity(t2)) {
		return 0;
	}

	PLTerm *arg1 = t1->datum.compoundTerm.arguments;
	PLTerm *arg2 = t2->datum.compoundTerm.arguments;
	while (arg1) {
		if (!PLTermEqual(arg1, arg2)) {
			return 0;
		}
		arg1 = arg1->next;
		arg2 = arg2->next;
	}

	PLTerm *body1 = t1->datum.compoundTerm.body;
	PLTerm *body2 = t2->datum.compoundTerm.body;
	while (body1) {
		if (!PLTermEqual(body1, body2)) {
			return 0;
		}
		body1 = body1->next;
		body2 = body2->next;
		if ((!body1 && body2) || (body1 && !body2)) {
			return 0;
		}
	}

	return 1;
}

int PLTermCompatible(const PLTerm *t1, const PLTerm *t2)
{
	assert(t1);
	assert(t2);
	return (PLTermIsCompound(t1) && PLTermIsCompound(t2)
	        && !strcmp(t1->datum.compoundTerm.name, t2->datum.compoundTerm.name)
	        && PLTermArity(t1) == PLTermArity(t2));
}

PLTerm *PLTermCreate(enum PLTermType type)
{
	PLTerm *t = (PLTerm *)malloc(sizeof(PLTerm));
	t->type = type;
	t->next = NULL;

	if (PLTermIsCompound(t)) {
		t->datum.compoundTerm.name = NULL;
		t->datum.compoundTerm.arguments = NULL;
		t->datum.compoundTerm.body = NULL;
	} else {
		t->datum.variable = NULL;
	}

	return t;
}

PLTerm *PLTermCopy(const PLTerm *t)
{
	if (!t) {
		return NULL;
	}

	PLTerm *copy = PLTermCreate(t->type);
	if (PLTermIsCompound(t)) {
		copy->datum.compoundTerm.arguments = PLTermsCopy(t->datum.compoundTerm.arguments);
		copy->datum.compoundTerm.body = PLTermsCopy(t->datum.compoundTerm.body);
		copy->datum.compoundTerm.name = (char *)malloc(strlen(t->datum.compoundTerm.name) + 1);
		strcpy(copy->datum.compoundTerm.name, t->datum.compoundTerm.name);
	} else {
		copy->datum.variable = (char *)malloc(strlen(t->datum.variable) + 1);
		strcpy(copy->datum.variable, t->datum.variable);
	}

	return copy;
}

PLTerm *PLTermsCopy(const PLTerm *t)
{
	if (!t) {
		return NULL;
	}

	PLTerm *first = PLTermCopy(t);
	if (t->next) {
		first->next = PLTermsCopy(t->next);
	}
	return first;
}

void PLTermPrint_(const PLTerm *t, FILE *file)
{
	if (!t) {
		return;
	}

	if (PLTermIsVariable(t)) {
		fprintf(file, "%s", t->datum.variable);
		return;
	}

	fprintf(file, "%s", t->datum.compoundTerm.name);
	if (PLTermArity(t) > 0) {
		fprintf(file, "(");
		PLTermPrint_(t->datum.compoundTerm.arguments, file);

		for (PLTerm *p = t->datum.compoundTerm.arguments->next; p; p = p->next) {
			fprintf(file, ",");
			PLTermPrint_(p, file);
		}
		fprintf(file, ")");
	}

	if (t->datum.compoundTerm.body) {
		fprintf(file, ":-");
		PLTermPrint_(t->datum.compoundTerm.body, file);

		for (PLTerm *p = t->datum.compoundTerm.body->next; p; p = p->next) {
			fprintf(file, ",");
			PLTermPrint_(p, file);
		}
	}
}

void PLTermPrint(const PLTerm *t, FILE *file)
{
	PLTermPrint_(t, file);
	fprintf(file, ".\n");
}

void PLTermFree(PLTerm *t)
{
	if (!t) {
		return;
	}

	if (PLTermIsCompound(t)) {
		free(t->datum.compoundTerm.name);
		PLTermFree(t->datum.compoundTerm.arguments);
		PLTermFree(t->datum.compoundTerm.body);
	} else {
		free(t->datum.variable);
	}

	PLTermFree(t->next);
	free(t);
}

void PLTermRenameVariablesIndex(PLTerm *t, int index, int renameList)
{
	if (!t) {
		return;
	}

	if (PLTermIsCompound(t)) {
		PLTermRenameVariablesIndex(t->datum.compoundTerm.arguments, index, 1);
		PLTermRenameVariablesIndex(t->datum.compoundTerm.body, index, 1);
	} else {
		t->datum.variable = (PLVariable)realloc(t->datum.variable, strlen(t->datum.variable) + EXTRA_SIZE);
		char buf[EXTRA_SIZE];
		sprintf(buf, "#%d", index);
		t->datum.variable = strcat(t->datum.variable, buf);
	}

	if (renameList) {
		PLTermRenameVariablesIndex(t->next, index, 1);
	}
}

void PLTermRenameVariables(PLTerm *t)
{
	assert(t);
	static int index = 0;
	PLTermRenameVariablesIndex(t, index++, 0);
}

