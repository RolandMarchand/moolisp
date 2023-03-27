#include "lexer.h"
#include "common.h"
#include "token.h"

int main() {
	const char* str = "(define (factorial n)\
  (if (= n 0)\
      1\
      (* n (factorial (- n 1)))))\
\
(define (map f lst)\
  (if (null? lst)\
      '()\
      (cons (f (car lst)) (map f (cdr lst)))))\
\
(define (filter pred lst)\
  (cond ((null? lst) '())\
        ((pred (car lst)) (cons (car lst) (filter pred (cdr lst))))\
        (else (filter pred (cdr lst)))))\
\
\
(define (fib n)\
  (if (< n 2)\
      n\
      (+ (fib (- n 1)) (fib (- n 2)))))\
\
(define (quicksort lst)\
  (if (null? lst)\
      '()\
      (let ((pivot (car lst))\
            (rest (cdr lst)))\
        (append (quicksort (filter (lambda (x) (< x pivot)) rest))\
                (cons pivot (quicksort (filter (lambda (x) (>= x pivot)) rest)))))))\
\
(display (factorial 5))\
(display (map (lambda (x) (* x x)) '(1 2 3 4 5)))\
(display (filter (lambda (x) (odd? x)) '(1 2 3 4 5)))\
(display (fib 10))\
(display (quicksort '(5 1 4 2 8)))";
	
	int tokens[] = {
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_INT,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_QUOTE,
		TOKEN_LPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_QUOTE,
		TOKEN_LPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_QUOTE,
		TOKEN_LPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_QUOTE,
		TOKEN_LPAREN,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_SYMBOL,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_QUOTE,
		TOKEN_LPAREN,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_LPAREN,
		TOKEN_SYMBOL,
		TOKEN_QUOTE,
		TOKEN_LPAREN,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_INT,
		TOKEN_RPAREN,
		TOKEN_RPAREN,
		TOKEN_RPAREN
	};
	char *lexemes[] = {
		"(",
		"define",
		"(",
		"factorial",
		"n",
		")",
		"(",
		"if",
		"(",
		"=",
		"n",
		"0",
		")",
		"1",
		"(",
		"*",
		"n",
		"(",
		"factorial",
		"(",
		"-",
		"n",
		"1",
		")",
		")",
		")",
		")",
		")",
		"(",
		"define",
		"(",
		"map",
		"f",
		"lst",
		")",
		"(",
		"if",
		"(",
		"null?",
		"lst",
		")",
		"'",
		"(",
		")",
		"(",
		"cons",
		"(",
		"f",
		"(",
		"car",
		"lst",
		")",
		")",
		"(",
		"map",
		"f",
		"(",
		"cdr",
		"lst",
		")",
		")",
		")",
		")",
		")",
		"(",
		"define",
		"(",
		"filter",
		"pred",
		"lst",
		")",
		"(",
		"cond",
		"(",
		"(",
		"null?",
		"lst",
		")",
		"'",
		"(",
		")",
		")",
		"(",
		"(",
		"pred",
		"(",
		"car",
		"lst",
		")",
		")",
		"(",
		"cons",
		"(",
		"car",
		"lst",
		")",
		"(",
		"filter",
		"pred",
		"(",
		"cdr",
		"lst",
		")",
		")",
		")",
		")",
		"(",
		"else",
		"(",
		"filter",
		"pred",
		"(",
		"cdr",
		"lst",
		")",
		")",
		")",
		")",
		")",
		"(",
		"define",
		"(",
		"fib",
		"n",
		")",
		"(",
		"if",
		"(",
		"<",
		"n",
		"2",
		")",
		"n",
		"(",
		"+",
		"(",
		"fib",
		"(",
		"-",
		"n",
		"1",
		")",
		")",
		"(",
		"fib",
		"(",
		"-",
		"n",
		"2",
		")",
		")",
		")",
		")",
		")",
		"(",
		"define",
		"(",
		"quicksort",
		"lst",
		")",
		"(",
		"if",
		"(",
		"null?",
		"lst",
		")",
		"'",
		"(",
		")",
		"(",
		"let",
		"(",
		"(",
		"pivot",
		"(",
		"car",
		"lst",
		")",
		")",
		"(",
		"rest",
		"(",
		"cdr",
		"lst",
		")",
		")",
		")",
		"(",
		"append",
		"(",
		"quicksort",
		"(",
		"filter",
		"(",
		"lambda",
		"(",
		"x",
		")",
		"(",
		"<",
		"x",
		"pivot",
		")",
		")",
		"rest",
		")",
		")",
		"(",
		"cons",
		"pivot",
		"(",
		"quicksort",
		"(",
		"filter",
		"(",
		"lambda",
		"(",
		"x",
		")",
		"(",
		">=",
		"x",
		"pivot",
		")",
		")",
		"rest",
		")",
		")",
		")",
		")",
		")",
		")",
		")",
		"(",
		"display",
		"(",
		"factorial",
		"5",
		")",
		")",
		"(",
		"display",
		"(",
		"map",
		"(",
		"lambda",
		"(",
		"x",
		")",
		"(",
		"*",
		"x",
		"x",
		")",
		")",
		"'",
		"(",
		"1",
		"2",
		"3",
		"4",
		"5",
		")",
		")",
		")",
		"(",
		"display",
		"(",
		"filter",
		"(",
		"lambda",
		"(",
		"x",
		")",
		"(",
		"odd?",
		"x",
		")",
		")",
		"'",
		"(",
		"1",
		"2",
		"3",
		"4",
		"5",
		")",
		")",
		")",
		"(",
		"display",
		"(",
		"fib",
		"10",
		")",
		")",
		"(",
		"display",
		"(",
		"quicksort",
		"'",
		"(",
		"5",
		"1",
		"4",
		"2",
		"8",
		")",
		")",
		")"
	};
	YY_BUFFER_STATE buffer = yy_scan_string(str);
	for (int token = yylex(), i = 0; token != TOKEN_EOF; token = yylex(), i++) {
		assert(tokens[i] == token);
		assert(strcmp(lexemes[i], yytext) == 0);
	}
	yy_delete_buffer(buffer);
}
