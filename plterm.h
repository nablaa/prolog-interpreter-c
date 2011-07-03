#ifndef PLTERM_H
#define PLTERM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Type of PLVariable, a normal string */
typedef char* PLVariable;

/* Type of PLCompoundTerm - can form a tree */
typedef struct PLCompoundStruct
{
	char *name; /* Name of this compound term */

	struct PLTermStruct *arguments;
	/* Terms constituting the argument list of this compound term.
	 * If the term has no arguments (ie. is an atom) then this value is NULL */

	struct PLTermStruct *body;
	/* Terms constituting the body of the compound term.
	 * If the term has no body (ie. is a fact) then this value is NULL */
} PLCompoundTerm;

typedef struct PLTermStruct
{   /* Type of the term */
	enum PLTermType
	{
		PLTermCompound,        /* Compound Term or an atom */
		PLTermVariable         /* An unbound variable */
	} type;
	/* Datum stored to this term */
	union
	{
		PLCompoundTerm compoundTerm;
		PLVariable variable;
	} datum;

	struct PLTermStruct *next; /* Next term following this in either:
	                            * compound term body, compound term arguments
	                            * or in a list of terms */
} PLTerm;

/* Returns a non-zero value if the term is a compound term
 *         zero if it is not */
int PLTermIsCompound(const PLTerm*);

/* Returns a non-zero value if the term is a variable
 *         zero if it is not */
int PLTermIsVariable(const PLTerm*);

/* Returns a non-zero value if the term is a constant
 *          (A compound term with no arguments and no body)
 *         zero if it is not */
int PLTermIsConstant(const PLTerm*);

/* Returns the arity (number of parameters) of the term
 * If called with a term other than a compound term, the behaviour
 * is undefined */
int PLTermArity(const PLTerm*);

/* Returns a non zero value if the variable occurs in the term
 *         zero if it does not */
int PLVariableOccurs(PLVariable, const PLTerm*);

/* Returns a non-zero value if the two terms are equal
 *         zero if they are not */
int PLTermEqual(const PLTerm*, const PLTerm*);

/* Returns a non-zero value if the two terms are compound terms
 *          and have the same functor (same name) and arity
 *          (number of arguments).
 *         zero if the functors differ */
int PLTermCompatible(const PLTerm*, const PLTerm*);

/* Creates a new term of the given type */
PLTerm* PLTermCreate(enum PLTermType);

/* Creates a deep copy of only the given term
 * Ignores the next field */
PLTerm* PLTermCopy(const PLTerm*);

/* Creates a deep copy of a list of terms */
PLTerm* PLTermsCopy(const PLTerm*);

/* Prints a term to the given file descriptor
 * Example output: sum(0,s(0),s(s(0))).\n */
void PLTermPrint(const PLTerm*, FILE*);

/* Frees all memory reserved for the list of terms */
void PLTermFree(PLTerm*);

/* Renames all variables of the term with a unique name. Recommendation
 * is that the variables be named with a suffix of a running index.
 * The index can be stored to a static variable or a global variable
 * Example: VarName -> VarName#index */
void PLTermRenameVariables(PLTerm*);

#endif
