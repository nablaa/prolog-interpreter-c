#ifndef PLTOKEN_H
#define PLTOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Known tokens */
typedef enum
{
	PLTokenConstant,         /* A string starting with a minuscule */
	PLTokenVariable,         /* A string starting with a majuscule */
	PLTokenParenthesisOpen,  /* '(' */
	PLTokenParenthesisClose, /* ')' */
	PLTokenComma,            /* ',' */
	PLTokenPeriod,           /* '.' */
	PLTokenArrow             /* ":-" */
} PLTokenType;

typedef struct PLTokenStruct
{
	PLTokenType type; /* Type of this token */

	char *value; /* Value of this token, applicable to PLTokenConstant
	              * and PLTokenVariable, NULL otherwise */

	struct PLTokenStruct *next; /* Next token in list */
} PLToken;

/* Tokenise the given file
 * stopToFirstPeriod is a boolean value indicating that the tokenising
 * should stop when the first period is discovered (reads only one term)
 * Everything after the symbol % to the end of the line is a comment
 * and must be discarded by the tokeniser
 * On success returns the list of tokens
 * On failure returns NULL (A list of zero tokens) */
PLToken* PLTokenise(FILE*, int stopToFirstPeriod);

/* Frees the list of tokens */
void PLTokensFree(PLToken*);

/* Creates a new token of the given type, the string parameter
 * can be NULL if the token is not PLTokenConstant or PLTokenVariable */
PLToken* PLTokenCreate(PLTokenType, const char*);

#endif
