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

