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
#include "plinterpreter.h"

#define UNKNOWN 0
#define SUCCESS 1

PLTerm *append(PLTerm *l1, PLTerm *l2)
{
	if (!l1) {
		return l2;
	}

	PLTerm *rval = l1;
	PLTerm *prev = l1;
	for (; l1; prev = l1, l1 = l1->next);

	prev->next = l2;
	return rval;
}

void PLHandleInput(FILE *database, FILE *query)
{
	PLTerm *db = PLConsult(database, 0);
	PLTerm *q;

	while (1) {
		printf("Query:\n");
		q = PLConsult(query, 1);
		assert(q);
		if (PLTermIsConstant(q) && !strcmp(q->datum.compoundTerm.name, "quit")) {
			break;
		}
		PLHandleQuery(q, db, query);
		PLTermFree(q);
	}

	PLTermFree(db);
	PLTermFree(q);
}

void PLHandleQuery(const PLTerm *query, const PLTerm *database, FILE *input)
{
	PLTerm *solution;
	PLStackFrame *stack = PLStackFrameCreate(query, query, (PLTerm *)database);

	while (1) {
		solution = PLInterpret(&stack, database);
		if (solution) {
			printf("Solution: ");
			PLTermPrint(solution, stdout);
			PLTermFree(solution);
			printf("More solutions (y/n) ?\n");

			int ch = fgetc(input);
			while (isspace(ch)) {
				ch = fgetc(input);
			}

			if (ch == 'y') {
			} else if (ch == 'n') {
				break;
			} else {
				assert(0);
			}
		} else {
			printf("No. \n");
			break;
		}
	}

	PLStackFree(stack);
}

PLTerm *PLInterpret(PLStackFrame **stack, const PLTerm *database)
{
	int status = UNKNOWN;

	while (*stack) {
		PLStackFrame *f = *stack;
		*stack = (*stack)->next;
		f->next = NULL;

		while (f->resolvent) {
			PLTerm *goal = f->resolvent;
			f->resolvent = f->resolvent->next;

			while (f->position) {
				PLTerm *compare = PLTermCopy(f->position);
				PLTermRenameVariables(compare);
				f->position = f->position->next;
				status = UNKNOWN;

				PLUnifier *u;
				int unifies = !PLUnify(goal, compare, &u);

				if (unifies) {
					if (f->position) {
						PLStackFrame *frame = PLStackFrameCreate(goal, f->goal, f->position);
						frame->next = *stack;
						*stack = frame;
					}

					PLTerm *body = compare->datum.compoundTerm.body;
					PLUnifierApplyToTerms(&f->resolvent, u);
					PLUnifierApplyToTerms(&f->goal, u);
					PLUnifierApplyToTerms(&body, u);

					f->resolvent = append(PLTermsCopy(body), f->resolvent);
					f->position = (PLTerm *)database;
					status = SUCCESS;

					PLTermFree(compare);
					PLUnifierFree(u);
					break;
				}

				PLTermFree(compare);
				PLUnifierFree(u);
			}

			goal->next = NULL;
			PLTermFree(goal);
			if (!f->position && status != SUCCESS) {
				break;
			}
		}

		if (!f->resolvent && status == SUCCESS) {
			PLTerm *solution = PLTermCopy(f->goal);
			PLStackFrameFree(f);
			return solution;
		}

		PLStackFrameFree(f);
	}

	return NULL;
}

PLTerm *PLConsult(FILE *file, int stopToFirstPeriod)
{
	PLToken *tokens = PLTokenise(file, stopToFirstPeriod);
	const PLToken *t = tokens;
	PLTerm *list = NULL;
	PLTerm *term = NULL;

	while (1) {
		term = PLParse(&t);
		if (!term) {
			break;
		}

		list = append(list, term);
	}

	PLTokensFree(tokens);
	return list;
}

