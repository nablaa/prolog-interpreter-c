#ifndef PLUNIFY_H
#define PLUNIFY_H

#include <stdio.h>
#include "plterm.h"

typedef struct PLUnifierStruct
{
	PLVariable variable; /* Variable to be unified */
	PLTerm *term;        /* Term to replace the variable */

	struct PLUnifierStruct *next;
} PLUnifier;

/* Attempts to unify the two terms
 * On success zero is returned and the MGU (Most General Unifier)
 *    is stored to the value pointed to by the PLUnifier parameter
 * On failure a non-zero value is returned and the target pointed
 *    by the PLUnifier parameter is NULL */
int PLUnify(const PLTerm*, const PLTerm*, PLUnifier**);

/* Creates a new unifier
 * Copies of term (PLTermCopy) and PLVariable are set to the unifier */
PLUnifier* PLUnifierCreate(const PLTerm*, const PLVariable);

/* Frees all the memory allocated for the unifier */
void PLUnifierFree(PLUnifier*);

/* Stack used by the unification algorithm */
typedef struct PLUnificationStackFrameStruct
{   /* Contains term1 = term2 for the unification */
	PLTerm *term1;
	PLTerm *term2;

	struct PLUnificationStackFrameStruct *next; /* Next frame in stack */
} PLUnificationStackFrame;

/* Creates a new PLUnificationStackFrame
 * Copies (PLTermCopy) of terms are stored to the frame */
PLUnificationStackFrame* PLUnificationStackFrameCreate(const PLTerm*,
                                                       const PLTerm*);
/* Frees all the memory allocated for the frame */
void PLUnificationStackFree(PLUnificationStackFrame*);

/* Apply the list of unifiers to the given term
 * Note the use of the double pointer (**) */
void PLUnifierApplyToTerms(PLTerm**, const PLUnifier*);

/* Apply the list of unifiers to the entire stack */
void PLUnifierApplyToStack(PLUnificationStackFrame*, const PLUnifier*);

/* Apply the given unifier to a list of unifiers */
void PLUnifierApplyToUnifier(PLUnifier*, const PLUnifier*);

#endif
