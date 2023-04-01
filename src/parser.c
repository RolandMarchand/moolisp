#include "common.h"
#include "lexer.h"
#include "parser.h"

extern tgc_t gc;

struct parser {
	struct var cons;
	struct lexer lexer;
	bool valid;
};

static struct var parse_string(struct token str);
static struct var parse_expression(struct parser *parser);
static struct var parse_quote(struct parser *parser);
static struct var parse_list(struct parser *parser);

struct var parse_string(struct token str)
{
	assert(str.type == TOKEN_STRING);
	/* Remove the lexeme's quotes */
	str.lexeme++;
	str.lexeme[strlen(str.lexeme) - 1] = '\0';
	char *s = tgc_alloc(&gc, strlen(str.lexeme) + 1);
	strcpy(s, str.lexeme);
	return string(s);
}

struct var parse_quote(struct parser *parser)
{
	struct var next_expr = parse_expression(parser);
	if (nilp(&next_expr)) {
		fprintf(stderr,
			"error:%lu: expected expression, received '%s'\n",
			parser->lexer.input.line,
			parser->lexer.current_token.lexeme);
	}
	return quote(next_expr);
}

struct var parse_list(struct parser *parser)
{
	struct var list = nil();
	struct var *list_tail = &list;
	while (lexer_peek(&parser->lexer).type != TOKEN_RPAREN) {
		*list_tail = cons(parse_expression(parser), nil());
		list_tail = &list_tail->as.cons->y;
	}
	lexer_scan(&parser->lexer);
	return list;
}

struct var parse_expression(struct parser *parser)
{
	struct token t = lexer_scan(&parser->lexer);
	switch (t.type) {
	case TOKEN_STRING:
		return parse_string(t);
	case TOKEN_SYMBOL:
		return symbol(t.lexeme);
	case TOKEN_NUMBER:
		return number(atof(t.lexeme));
	case TOKEN_QUOTE:
		return parse_quote(parser);
	case TOKEN_LPAREN:
		return parse_list(parser);
	case TOKEN_ERROR:
		exit(EXIT_FAILURE);
	case TOKEN_RPAREN:
		fprintf(stderr,
			"error:%lu: expected expression, received ')'\n",
			parser->lexer.input.line);
		exit(EXIT_FAILURE);
	case TOKEN_EOF:
		fprintf(stderr, "error:%lu: EOF reached\n",
			parser->lexer.input.line);
		exit(EXIT_FAILURE);
	default:
		fprintf(stderr, "error:%lu: unexpected token '%s'\n",
			parser->lexer.input.line,
			parser->lexer.current_token.lexeme);
		exit(EXIT_FAILURE);
	}
}

struct var parse(char *data)
{
	assert(data);
	struct parser parser;
	parser.valid = true;
	lexer_init(&parser.lexer, data, 0);
	memset(&parser.cons, 0, sizeof(struct var));
	struct var *tail = &parser.cons;
	while (lexer_peek(&parser.lexer).type != TOKEN_EOF) {
		*tail = cons(parse_expression(&parser), nil());
		tail = &tail->as.cons->y;
	}        
	return parser.cons;
}
