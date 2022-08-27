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
#include "plparse.h"

PLTerm *PLParseTerm(const PLToken **tokens);

PLTerm *PLParseArguments(const PLToken **tokens)
{
	PLTerm *t = PLParseTerm(tokens);
	PLTokenType type = (*tokens)->type;

	*tokens = (*tokens)->next;
	if (type == PLTokenComma) {
		t->next = PLParseArguments(tokens);
	} else if (type == PLTokenParenthesisClose) {
	} else {
		assert(0);
	}

	return t;
}

PLTerm *PLParseBody(const PLToken **tokens)
{
	PLTerm *t = PLParseTerm(tokens);
	PLTokenType type = (*tokens)->type;

	*tokens = (*tokens)->next;
	if (type == PLTokenComma) {
		t->next = PLParseBody(tokens);
	} else if (type == PLTokenPeriod) {
	} else {
		assert(0);
	}

	return t;
}

PLTerm *PLParseTerm(const PLToken **tokens)
{
	PLTokenType type = (*tokens)->type;
	PLTerm *t;

	if (type == PLTokenConstant) {
		t = PLTermCreate(PLTermCompound);
		t->datum.compoundTerm.name = (char *)malloc(strlen((*tokens)->value) + 1);
		strcpy(t->datum.compoundTerm.name, (*tokens)->value);
		*tokens = (*tokens)->next;
		type = (*tokens)->type;

		if (type == PLTokenParenthesisOpen) {
			*tokens = (*tokens)->next;
			t->datum.compoundTerm.arguments = PLParseArguments(tokens);
		}
	} else if (type == PLTokenVariable) {
		t = PLTermCreate(PLTermVariable);
		t->datum.variable = (char *)malloc(strlen((*tokens)->value) + 1);
		strcpy(t->datum.variable, (*tokens)->value);
		*tokens = (*tokens)->next;
	} else {
		assert(0);
	}

	return t;
}

PLTerm *PLParse(const PLToken **tokens)
{
	assert(tokens);
	if (!*tokens) {
		return NULL;
	}

	PLTerm *head = PLParseTerm(tokens);
	PLTokenType type = (*tokens)->type;
	*tokens = (*tokens)->next;

	if (type == PLTokenPeriod) {
	} else if (type == PLTokenArrow) {
		assert(PLTermIsCompound(head));
		head->datum.compoundTerm.body = PLParseBody(tokens);
	} else if (type == PLTokenComma) {
		head->next = PLParse(tokens);
	} else {
		assert(0);
	}

	return head;
}

