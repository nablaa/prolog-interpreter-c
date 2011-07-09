#include "plinterpreter.h"

#define UNKNOWN 0
#define SUCCESS 1

void addEnd(PLTerm *t, PLTerm **l)
{
	if (!*l) {
		*l = t;
		return;
	}

	PLTerm *list = *l;
	PLTerm *prev = list;
	for (; list; prev = list, list = list->next);
	prev->next = t;
}

PLTerm *append(PLTerm *l1, PLTerm *l2)
{
	if (!l1) {
		return PLTermsCopy(l2);
	}

	PLTerm *rval = l1;
	PLTerm *prev = l1;
	for (; l1; prev = l1, l1 = l1->next);

	prev->next = PLTermsCopy(l2);
	return rval;
}

void PLHandleInput(FILE *database, FILE *query)
{
	PLTerm *db = PLConsult(database, 0);
	PLTerm *q;

	while (1) {
		q = PLConsult(query, 1);
		assert(q);
		if (PLTermIsConstant(q) && !strcmp(q->datum.compoundTerm.name, "quit")) {
			break;
		}
		PLHandleQuery(q, db, stdin);
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
			fgetc(input);
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
			goal->next = NULL;

			while (f->position) {
				PLTerm *compare = PLTermCopy(f->position);
				PLTermRenameVariables(compare);
				f->position = f->position->next;
				status = UNKNOWN;

				PLUnifier *u;
				int unifies = !PLUnify(goal, compare, &u);

				if (unifies) {
					if (f->position) {
						PLStackFrame *frame = PLStackFrameCreate(f->goal, f->goal, f->position);
						frame->next = *stack;
						*stack = frame;
					}

					PLTerm *body = compare->datum.compoundTerm.body;
					PLUnifierApplyToTerms(&f->resolvent, u);
					PLUnifierApplyToTerms(&f->goal, u);
					PLUnifierApplyToTerms(&body, u);
					f->resolvent = append(f->resolvent, body);
					f->position = (PLTerm *)database;
					status = SUCCESS;

					PLTermFree(compare);
					PLUnifierFree(u);
					break;
				}

				PLTermFree(compare);
				PLUnifierFree(u);
			}

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

		addEnd(term, &list);
	}

	PLTokensFree(tokens);
	return list;
}

