#pragma once

#include "common.h"

typedef enum {
	TOKEN_EOF = 0,
	TOKEN_NUMBER,
	TOKEN_STRING,
	TOKEN_SYMBOL,
	TOKEN_QUOTE,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_ERROR
} Token;

struct token {
	char *lexeme;
	Token type;
};

struct lexer {
	struct {
		char *data;
		char *ptr;
		size_t length;
		size_t line;
	} input;
	struct token current_token;
};

void lexer_init(struct lexer *, char *input, unsigned int position);
struct token lexer_scan(struct lexer *);
struct token lexer_peek(struct lexer *);
