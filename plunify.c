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
#include "plunify.h"

int Identical(const PLTerm *t1, const PLTerm *t2)
{
	if (PLTermIsConstant(t1) && PLTermIsConstant(t2) && PLTermEqual(t1, t2)) {
		return 1;
	}
	if (PLTermIsVariable(t1) && PLTermIsVariable(t2) && PLTermEqual(t1, t2)) {
		return 1;
	}
	return 0;
}

int PLUnify(const PLTerm *t1, const PLTerm *t2, PLUnifier **unifier)
{
	PLUnifier *mgu = PLUnifierCreate(NULL, "");
	PLUnificationStackFrame *stack = PLUnificationStackFrameCreate(t1, t2);

	while (stack) {
		PLUnificationStackFrame *xy = stack;
		stack = stack->next;
		xy->next = NULL;
		PLTerm *x = xy->term1;
		PLTerm *y = xy->term2;

		#define UNIFY(X, Y)                                           \
			PLUnifier *u = PLUnifierCreate(X, Y->datum.variable); \
			PLUnifierApplyToUnifier(mgu, u);                      \
			PLUnifierApplyToStack(stack, u);                      \
			u->next = mgu;                                        \
			mgu = u;

		if (PLTermIsVariable(x) && !PLVariableOccurs(x->datum.variable, y)) {
			UNIFY(y, x);
		} else if (PLTermIsVariable(y) && !PLVariableOccurs(y->datum.variable, x)) {
			UNIFY(x, y);
		} else if (Identical(x, y)) {
		} else if (PLTermCompatible(x, y)) {
			const PLTerm *am = x->datum.compoundTerm.arguments;
			const PLTerm *bm = y->datum.compoundTerm.arguments;

			for (int i = 0; i < PLTermArity(x); ++i) {
				PLUnificationStackFrame *f = PLUnificationStackFrameCreate(am, bm);
				f->next = stack;
				stack = f;
				am = am->next;
				bm = bm->next;
			}
		} else {
			PLUnificationStackFree(xy);
			PLUnificationStackFree(stack);
			PLUnifierFree(mgu);
			*unifier = NULL;
			return 1;
		}

		#undef UNIFY
		PLUnificationStackFree(xy);
	}

	PLUnificationStackFree(stack);
	*unifier = mgu;
	return 0;
}

PLUnifier *PLUnifierCreate(const PLTerm *t, const PLVariable v)
{
	PLUnifier *u = (PLUnifier *)malloc(sizeof(PLUnifier));
	u->variable = NULL;
	if (v) {
		u->variable = (char *)malloc(strlen(v) + 1);
		strcpy(u->variable, v);
	}
	u->term = PLTermCopy(t);
	u->next = NULL;
	return u;
}

void PLUnifierFree(PLUnifier *u)
{
	if (!u) {
		return;
	}
	free(u->variable);
	PLTermFree(u->term);
	PLUnifierFree(u->next);
	free(u);
}

PLUnificationStackFrame *PLUnificationStackFrameCreate(const PLTerm *t1,
                                                       const PLTerm *t2)
{
	PLUnificationStackFrame *frame = (PLUnificationStackFrame *)malloc(sizeof(PLUnificationStackFrame));
	frame->term1 = PLTermCopy(t1);
	frame->term2 = PLTermCopy(t2);
	frame->next = NULL;
	return frame;
}

void PLUnificationStackFree(PLUnificationStackFrame *f)
{
	if (!f) {
		return;
	}
	PLTermFree(f->term1);
	PLTermFree(f->term2);
	PLUnificationStackFree(f->next);
	free(f);
}

void PLUnifierApplyToTerms(PLTerm **t, const PLUnifier *u)
{
	if (!*t) {
		return;
	}

	if (PLTermIsVariable(*t)) {
		PLTerm *term = *t;
		char *var = term->datum.variable;
		if (!strcmp(var, u->variable)) {
			PLTerm *head = PLTermCopy(u->term);
			head->next = term->next;
			term->next = NULL;
			*t = head;
			PLTermFree(term);
		}
	} else {
		PLTerm *term = *t;
		if (term->datum.compoundTerm.arguments) {
			PLUnifierApplyToTerms(&term->datum.compoundTerm.arguments, u);
		}
		if (term->datum.compoundTerm.body) {
			PLUnifierApplyToTerms(&term->datum.compoundTerm.body, u);
		}
	}

	if ((*t)->next) {
		PLUnifierApplyToTerms(&(*t)->next, u);
	}

	if (u->next) {
		PLUnifierApplyToTerms(t, u->next);
	}
}

void PLUnifierApplyToStack(PLUnificationStackFrame *f, const PLUnifier *u)
{
	while (f) {
		PLUnifierApplyToTerms(&(f->term1), u);
		PLUnifierApplyToTerms(&(f->term2), u);
		f = f->next;
	}
}

void PLUnifierApplyToUnifier(PLUnifier *list, const PLUnifier *u)
{
	while (list) {
		PLUnifierApplyToTerms(&(list->term), u);
		list = list->next;
	}
}

