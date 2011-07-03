#include "pltoken.h"

#define MAX_LEN 128

PLToken *PLTokenise(FILE *file, int stopToFirstPeriod)
{
	if (!file) {
		return NULL;
	}

	int stop = 0;
	int ch;
	PLToken *tokens = NULL;
	PLToken *first = NULL;
	PLToken *t;

	while ((ch = fgetc(file)) != EOF) {
		while (isspace(ch)) {
			ch = fgetc(file);
		}

		if (ch == '(') {
			t = PLTokenCreate(PLTokenParenthesisOpen, NULL);
		} else if (ch == ')') {
			t = PLTokenCreate(PLTokenParenthesisClose, NULL);
		} else if (ch == ',') {
			t = PLTokenCreate(PLTokenComma, NULL);
		} else if (ch == '.') {
			t = PLTokenCreate(PLTokenPeriod, NULL);
			stop = stopToFirstPeriod;
		} else if (ch == ':') {
			if ((ch = fgetc(file)) != '-') {
				PLTokensFree(tokens);
				return NULL;
			}
			t = PLTokenCreate(PLTokenArrow, NULL);
		} else if (ch == '%') {
			while ((ch = fgetc(file)) != '\n');
			continue;
		} else {
			if (!isalpha(ch) && !isdigit(ch)) {
				PLTokensFree(tokens);
				return NULL;
			}

			PLTokenType type = isupper(ch) ? PLTokenVariable : PLTokenConstant;
			char buf[MAX_LEN];
			int i = 0;
			do {
				buf[i++] = ch;
				ch = fgetc(file);
			} while (isalpha(ch) || isdigit(ch));
			buf[i] = '\0';
			fseek(file, -1, SEEK_CUR);
			t = PLTokenCreate(type, buf);
		}

		if (!tokens) {
			tokens = t;
			first = tokens;
		} else {
			tokens->next = t;
			tokens = tokens->next;
		}

		if (stop) {
			break;
		}
	}

	return first;
}

void PLTokensFree(PLToken *token)
{
	if (!token) {
		return;
	}

	PLToken *t = token->next;
	free(token->value);
	free(token);
	while (t) {
		token = t;
		t = t->next;
		free(token->value);
		free(token);
	}
}

PLToken *PLTokenCreate(PLTokenType type, const char *lexem)
{
	PLToken *t = (PLToken *)malloc(sizeof(PLToken));
	t->type = type;
	t->next = NULL;
	t->value = NULL;

	if (type == PLTokenConstant || type == PLTokenVariable) {
		t->value = (char *)malloc(strlen(lexem) + 1);
		strcpy(t->value, lexem);
	}
	return t;
}

