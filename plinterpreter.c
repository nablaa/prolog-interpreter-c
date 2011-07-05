#include "plinterpreter.h"

void PLHandleInput(FILE *database, FILE *query)
{

}

void PLHandleQuery(const PLTerm *query, const PLTerm *database, FILE *input)
{

}

PLTerm *PLInterpret(PLStackFrame **stack, const PLTerm *database)
{

}

PLTerm *PLConsult(FILE *file, int stopToFirstPeriod)
{
	PLToken *tokens = PLTokenise(file, stopToFirstPeriod);
	const PLToken *t = tokens;
	PLTerm *list = NULL;
	PLTerm *term = NULL;

	do {
		term = PLParse(&t);
		if (!term) {
			break;
		}
		term->next = list;
		list = term;
	} while (stopToFirstPeriod);

	PLTokensFree(tokens);
	return list;
}

