#include "plinterpreter.h"

#define UNKNOWN 0
#define SUCCESS 1

void addEnd(PLTerm *t, PLTerm *list)
{
	assert(list);
	PLTerm *prev = list;
	while (list) {
		prev = list;
		list = list->next;
	}

	prev->next = PLTermCopy(t);
}

void PLHandleInput(FILE *database, FILE *query)
{
	PLTerm *db = PLConsult(database, 0);
	PLTerm *q;

	while (1) {
		q = PLConsult(query, 1);
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
	PLStackFrame *stack = PLStackFrameCreate(query, query, database); // TODO ?

	while (1) {
		solution = PLInterpret(&stack, database);
		if (solution) {
			printf("Solution: ");
			PLTermPrint(solution, stdout);
			PLTermFree(solution);
			printf("More solutions (y/n) ?\n");
			int ch = fgetc(input);
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
			PLTermFree(f->goal);
			f->goal = f->resolvent;
			f->resolvent = f->resolvent->next;
			f->goal->next = NULL;

			while (f->position) {
				PLTerm *compare = PLTermCopy(f->position);
				PLTermRenameVariables(compare);
				f->position = f->position->next;
				status = UNKNOWN;

				PLUnifier *u;
				int unifies = PLUnify(f->goal, compare, &u);

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

					while (body) {
						addEnd(body, f->resolvent);
						body = body->next;
					}

					f->position = database;
					status = SUCCESS;
					break;
				}

				PLUnifierFree(u);
				PLTermFree(compare);
			}

			if (!f->position->next && status != SUCCESS) {
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
		term->next = list;
		list = term;
	};

	PLTokensFree(tokens);
	return list;
}
