%{
#include "lisp.h"
%}

%option noyywrap
%option yylineno

%%

[[:space:]]+       ; /* ignore whitespace */
[0-9]+             { return TOKEN_INT; } 
"("		   { return TOKEN_LPAREN; }
")"		   { return TOKEN_RPAREN; }
"'"		   { return TOKEN_QUOTE; }
\"([^\"\\]|\\.)*\" { return TOKEN_STRING; }
[^[:space:]()']+ { return TOKEN_SYMBOL; }
<<EOF>> { return TOKEN_EOF; }

%%
