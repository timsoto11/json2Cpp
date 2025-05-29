%{
#include "ast.h"
#include "parser.h"
%}

%option noyywrap

STRING     \"(\\.|[^\\"])*\"
INTEGER     -?[0-9]+
NUMBER     -?[0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?
BOOLEAN    (true|false)
VNULL    (null)

%%

"{"             { return LBRACE; }
"}"             { return RBRACE; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
":"             { return COLON; }
","             { return COMMA; }

{STRING}        { yylval.str = strdup(yytext); return STRING;  }
{INTEGER}       { yylval.str = strdup(yytext); return INTEGER; }
{NUMBER}        { yylval.str = strdup(yytext); return NUMBER; }
{BOOLEAN}       { yylval.str = strdup(yytext);  return BOOLEAN; }
{VNULL}         { yylval.str = strdup(yytext);  return VNULL;}

[ \t\n\r]+      { /* Skip whitespace */ }

.               { printf("UNKNOWN(%s)\n", yytext); }

%%