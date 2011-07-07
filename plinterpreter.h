#ifndef PLINTERPRETER_H
#define PLINTERPRETER_H

#include <stdio.h>

#include "plterm.h"
#include "plstack.h"
#include "pltoken.h"
#include "plparse.h"
#include "plunify.h"

/* Primary interaction loop.
 * Reads one term from the query input and if the term is the constant
 * quit terminates the interpreter, otherwise calls PLHandleQuery() */
void PLHandleInput(FILE* database, FILE* query);

/* Given a query calls PLInterpret() for it
 * Gives the user the possibility to have more solutions outputted. */
void PLHandleQuery(const PLTerm* query, PLTerm* database, FILE* input);

/* Interprets the given stack using the database
 * On success returns the solution term
 *            stack contains the stack
 * On failure returns NULL and the value of the stack is invalid */
PLTerm* PLInterpret(PLStackFrame **stack, PLTerm* database);

/* Returns a list of terms that can be parsed from the file
 * stopToFirstPeriod is used in interactive mode and passed to tokeniser */
PLTerm* PLConsult(FILE*, int stopToFirstPeriod);

#endif
