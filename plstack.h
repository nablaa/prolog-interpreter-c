#ifndef PLSTACK_H
#define PLSTACK_H

#include "plterm.h"

typedef struct PLStackFrameStruct
{
	PLTerm* resolvent; /* List of resolvent terms */
	PLTerm* goal;      /* Goal term (query) */
	PLTerm* position;  /* Position in the database */

	struct PLStackFrameStruct* next; /* Next frame in stack */
} PLStackFrame;

/* Creates a new PLStackFrame
 * Copies (PLTermsCopy) of resolvent and goal are stored to the frame
 * Value of position is simply set to the frame */
PLStackFrame* PLStackFrameCreate(const PLTerm* resolvent,
                                 const PLTerm* goal,
                                 PLTerm* position);

/* Frees one PLStackFrame (does not alter the next field) */
void PLStackFrameFree(PLStackFrame*);

/* Frees the entire stack */
void PLStackFree(PLStackFrame*);

#endif
