%{
#include "ast.h"
#include "jsonSchema.tab.h"
%}

%option noyywrap

STRING     \"(\\.|[^\\"])*\"
INTEGER     -?[0-9]+
NUMBER     -?[0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?
BOOLEAN    (true|false)
NULL    (null)
KEYWORD    \"(type|properties|required|items|description|title|enum|default|format)\"

%%

"{"             { printf("LBRACE\n"); }
"}"             { printf("RBRACE\n"); }
"["             { printf("LBRACKET\n"); }
"]"             { printf("RBRACKET\n"); }
":"             { printf("COLON\n"); }
","             { printf("COMMA\n"); }

{KEYWORD}       { printf("KEYWORD(%s)\n", yytext); }
{STRING}        { printf("STRING(%s)\n", yytext); }
{INTEGER}        { printf("INTEGER(%s)\n", yytext); }
{NUMBER}        { printf("NUMBER(%s)\n", yytext); }
{BOOLEAN}       { printf("BOOLEAN(%s)\n", yytext); }
{NULL}       { printf("NULL(%s)\n", yytext); }

[ \t\n\r]+      { /* Skip whitespace */ }

.               { printf("UNKNOWN(%s)\n", yytext); }

%%