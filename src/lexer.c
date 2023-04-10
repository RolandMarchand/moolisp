#include "lexer.h"

/* ¯\_(ツ)_/¯ */
#define WHITESPACE ' ' : case '\v' : case '\t' : case '\n' : case '\r'
#define NUMBER									\
	'0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6'	\
	: case '7' : case '8' : case '9'
#define CURRENT_CHAR_PTR (lexer->input.ptr)
#define CURRENT_CHAR (*CURRENT_CHAR_PTR)

extern tgc_t gc;

static void lexer_fixnum(struct lexer *lexer);
static void lexer_string(struct lexer *lexer);
static void lexer_symbol(struct lexer *lexer);

char lexer_next_char(struct lexer *lexer)
{
	char current_char = CURRENT_CHAR;
	switch (current_char) {
	case '\0':
		return '\0';
	case '\n':
	case '\r':
		lexer->input.line++;
		__attribute__((fallthrough));
	default:
		CURRENT_CHAR_PTR++;
		return current_char;
	}
}

void lexer_init(struct lexer *lexer, char *input, unsigned int position)
{
	lexer->input.length = strlen(input);
	assert(position <= lexer->input.length);
	
	lexer->input.data = tgc_alloc(&gc, lexer->input.length * sizeof(char) + 1);
	strcpy(lexer->input.data, input);

	lexer->input.line = 1;
	for (char *i = input; i <= input + position; i++) {
		if (*i == '\n') {
			lexer->input.line++;
		}
	}

	CURRENT_CHAR_PTR = lexer->input.data + position;
	lexer->current_token.lexeme = NULL;
	lexer->current_token.type = TOKEN_ERROR;
}

/* from inclusive, to exclusive */
void lexer_set_token(struct lexer *lexer, Token type, char *from, char *to)
{
	assert(from < to);
	lexer->current_token.type = type;
	size_t lexeme_len = to - from;
	lexer->current_token.lexeme = tgc_alloc(&gc, lexeme_len + 1);
	memcpy(lexer->current_token.lexeme, from, lexeme_len);
	lexer->current_token.lexeme[lexeme_len] = '\0';
}

/* Eat input until whitespace, ')' or EOF and mark it as TOKEN_ERROR */
/* Print unexpected token error */
void lexer_unknown_token(struct lexer *lexer)
{
	char *from = CURRENT_CHAR_PTR;
  next_char:
	switch (CURRENT_CHAR) {
	case WHITESPACE:
	case ')':
	case '\0':
		break;
	default:
		lexer_next_char(lexer);
		goto next_char;
	}
	lexer_set_token(lexer, TOKEN_ERROR, from, CURRENT_CHAR_PTR);
	fprintf(stderr, "Error:%lu: unknown token '%s'\n", lexer->input.line,
		lexer->current_token.lexeme);
}

void lexer_fixnum(struct lexer *lexer)
{
	assert(CURRENT_CHAR >= '0' && CURRENT_CHAR <= '9');
	char *from = CURRENT_CHAR_PTR;
	bool is_float = false;

  next_char:
	switch (CURRENT_CHAR) {
	case '.':
		if (is_float) {
			CURRENT_CHAR_PTR = from;
			lexer_unknown_token(lexer);
			return;
		}
		is_float = true;
		__attribute__((fallthrough));
	case NUMBER:
		lexer_next_char(lexer);
		goto next_char;
	case '\0':
	case ')':
	case WHITESPACE:
		break;
	default:
		CURRENT_CHAR_PTR = from;
		lexer_unknown_token(lexer);
		return;
	}
	lexer_set_token(lexer, TOKEN_NUMBER, from, CURRENT_CHAR_PTR);
}

void lexer_string(struct lexer *lexer)
{
	assert(CURRENT_CHAR == '"');
	char *from = CURRENT_CHAR_PTR;
	do {
		if (CURRENT_CHAR == '\\') {
			lexer_next_char(lexer);
		}
		if (CURRENT_CHAR == '\0') {
			lexer_set_token(lexer, TOKEN_ERROR, from, CURRENT_CHAR_PTR);
			fprintf(stderr, "Error:%lu unfinished string\n",
				lexer->input.line);
			return;
		}
		lexer_next_char(lexer);
	} while (CURRENT_CHAR != '"');
	lexer_next_char(lexer);
	lexer_set_token(lexer, TOKEN_STRING, from, CURRENT_CHAR_PTR);
}

void lexer_symbol(struct lexer *lexer)
{
#define CURRENT_CHAR_IS_SYMBOL ((CURRENT_CHAR >= '*' && CURRENT_CHAR <= '~')	\
				|| (CURRENT_CHAR >= '#' && CURRENT_CHAR <= '&')	\
				|| CURRENT_CHAR == '!')
	assert(CURRENT_CHAR_IS_SYMBOL
	       && !(CURRENT_CHAR >= '0' && CURRENT_CHAR <= '9'));
	char *from = CURRENT_CHAR_PTR;
	while (CURRENT_CHAR_IS_SYMBOL) {
		lexer_next_char(lexer);
	}
	lexer_set_token(lexer, TOKEN_SYMBOL, from, CURRENT_CHAR_PTR);
#undef CURRENT_CHAR_IS_SYMBOL
}

struct token lexer_peek(struct lexer *lexer)
{
	char *save_ptr = CURRENT_CHAR_PTR;
	size_t save_line = lexer->input.line;
	struct token save_token = lexer->current_token;
	struct token t = lexer_scan(lexer);

	CURRENT_CHAR_PTR = save_ptr;
	lexer->input.line = save_line;
	lexer->current_token = save_token;

	return t;
}

struct token lexer_scan(struct lexer *lexer)
{
  next_char:
	switch (CURRENT_CHAR) {
	case WHITESPACE:
		break;
	case NUMBER:
		lexer_fixnum(lexer);
		return lexer->current_token;
	case '(':
		lexer->current_token.lexeme = tgc_alloc(&gc, 2);
		strcpy(lexer->current_token.lexeme, "(");
		lexer->current_token.type = TOKEN_LPAREN;
		lexer_next_char(lexer);
		return lexer->current_token;
	case ')':
		lexer->current_token.lexeme = tgc_alloc(&gc, 2);
		strcpy(lexer->current_token.lexeme, ")");
		lexer->current_token.type = TOKEN_RPAREN;
		lexer_next_char(lexer);
		return lexer->current_token;
	case '\'':
		lexer->current_token.lexeme = tgc_alloc(&gc, 2);
		strcpy(lexer->current_token.lexeme, "'");
		lexer->current_token.type = TOKEN_QUOTE;
		lexer_next_char(lexer);
		return lexer->current_token;
	case '"':
		lexer_string(lexer);
		return lexer->current_token;
	case '\0':
		lexer->current_token.lexeme = tgc_calloc(&gc, 1, 1);
		lexer->current_token.type = TOKEN_EOF;
		return lexer->current_token;
	case ';':
		while (CURRENT_CHAR != '\r'
		       && CURRENT_CHAR != '\n'
		       && CURRENT_CHAR != '\0') {
			lexer_next_char(lexer);
		}
		break;
	default:
		lexer_symbol(lexer);
		return lexer->current_token;
	}
	lexer_next_char(lexer);
	goto next_char;
}
