#ifndef PLPARSE_H
#define PLPARSE_H

#include "pltoken.h"
#include "plterm.h"
#include <assert.h>

/* Parses one entity (something which ends with a period)
 * Returns the parsed entity and sets the PLToken** parameter
 *   to the position of the token list where the parsing stopped
 * Note that const PLToken** ptr; means that **ptr is const
 *      not that *ptr is const */
PLTerm* PLParse(const PLToken**);

#endif
