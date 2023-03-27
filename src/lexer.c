#include "lexer.h"

extern tgc_t gc;

static void lexer_fixnum(struct lexer *lexer);
static void lexer_string(struct lexer *lexer);
static void lexer_symbol(struct lexer *lexer);
static void lexer_error(struct lexer *lexer, char *from, size_t nmem);

void lexer_error(struct lexer *lexer, char *from, size_t nmem)
{
	lexer->current_token.lexeme = tgc_alloc(&gc, nmem + 1);
	memcpy(lexer->current_token.lexeme, from, nmem);
	lexer->current_token.lexeme[nmem] = '\0';
	lexer->current_token.type = TOKEN_ERROR;
	fprintf(stderr, "error: invalid token '%s'\n", lexer->current_token.lexeme);
}

void lexer_fixnum(struct lexer *lexer)
{
	assert(*lexer->current_token.lexeme >= '0'
	       && *lexer->current_token.lexeme <= '9');
	char *from = lexer->input_ptr;
	bool is_float = false;
	while (true) {
		switch(*lexer->input_ptr) {
		case '.':
			if (is_float) {
				lexer_error(lexer, from, lexer->input_ptr - from + 1);
				return;
			}
			is_float = true;
			/* pass-through */
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
			goto fixnum_matched;
		}
		lexer->input_ptr++;
	}
  fixnum_matched:
	lexer->current_token.type = TOKEN_FIXNUM;
	size_t lexeme_len = lexer->input_ptr - from;
	lexer->current_token.lexeme = tgc_alloc(&gc, lexeme_len + 1);
	memcpy(lexer->current_token.lexeme, from, lexeme_len);
	lexer->current_token.lexeme[lexeme_len] = '\0';
}

void lexer_string(struct lexer *lexer)
{
	
}

void lexer_symbol(struct lexer *lexer)
{
	
}

void lexer_init(struct lexer *lexer, char *input, unsigned int position)
{
	lexer->input_length = strlen(input);
	assert(position < lexer->input_length);
	
	lexer->input = tgc_alloc(&gc, lexer->input_length * sizeof(char));
	strcpy(lexer->input, input);

	lexer->input_ptr = lexer->input + position;
	lexer->current_token.lexeme = NULL;
	lexer->current_token.type = TOKEN_ERROR;
}

void lexer_scan(struct lexer *lexer)
{
	while (true) {
		switch (*lexer->input_ptr) {
		case ' ':
		case '\v':
		case '\t':
		case '\n':
		case '\r':
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			lexer_fixnum(lexer);
			return;
		case '(':
			lexer->current_token.lexeme = tgc_alloc(&gc, 2);
			strcpy(lexer->current_token.lexeme, "(");
			lexer->current_token.type = TOKEN_LPAREN;
			return;
		case ')':
			lexer->current_token.lexeme = tgc_alloc(&gc, 2);
			strcpy(lexer->current_token.lexeme, ")");
			lexer->current_token.type = TOKEN_RPAREN;
			return;
		case '\'':
			lexer->current_token.lexeme = tgc_alloc(&gc, 2);
			strcpy(lexer->current_token.lexeme, "'");
			lexer->current_token.type = TOKEN_QUOTE;
			return;
		case '"':
			lexer_string(lexer);
			return;
		case '\0':
			lexer->current_token.lexeme = NULL;
			lexer->current_token.type = TOKEN_EOF;
			return;
		default:
			lexer_symbol(lexer);
			return;
		}
		lexer->input_ptr++;
	}
}
