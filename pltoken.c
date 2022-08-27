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
#include "pltoken.h"

#define MAX_LEN 128

PLToken *PLTokenise(FILE *file, int stopToFirstPeriod)
{
	assert(file);
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
		} else if (ch == EOF) {
			break;
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
			ungetc(ch, file);
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

