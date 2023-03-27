#include "common.h"

typedef enum {
	TOKEN_EOF = 0,
	TOKEN_FIXNUM,
	TOKEN_STRING,
	TOKEN_SYMBOL,
	TOKEN_QUOTE,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_ERROR
} Token;

struct lexer {
	char *input;
	char *input_ptr;
	size_t input_length;
	struct {
		char *lexeme;
		Token type;
	} current_token;
};

void lexer_init(struct lexer *, char *input, unsigned int position);
void lexer_scan(struct lexer *);
