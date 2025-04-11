%{
#include "ast.h"
#include "jsonSchema.tab.h"
%}

%option noyywrap

STRING     \"(\\.|[^\\"])*\"
INTEGER     -?[0-9]+
NUMBER     -?[0-9]+(\.[0-9]+)?([eE][-+]?[0-9]+)?
BOOLEAN    (true|false)
VNULL    (null)
KEYWORD    \"(type|properties|required|items|description|title|enum|default|format|$schema)\"

%%

"{"             { printf("LBRACE\n"); return LBRACE; }
"}"             { printf("RBRACE\n"); return RBRACE; }
"["             { printf("LBRACKET\n"); return LBRACKET; }
"]"             { printf("RBRACKET\n"); return RBRACKET; }
":"             { printf("COLON\n");return COLON; }
","             { printf("COMMA\n"); return COMMA; }

{KEYWORD}       { printf("KEYWORD(%s)\n", yytext);  yylval.str = strdup(yytext); return STRING; }
{STRING}        { printf("STRING(%s)\n", yytext);  yylval.str = strdup(yytext); return STRING;  }
{INTEGER}       { printf("INTEGER(%s)\n", yytext);  yylval.str = strdup(yytext); return INTEGER; }
{NUMBER}        { printf("NUMBER(%s)\n", yytext); yylval.str = strdup(yytext); return NUMBER; }
{BOOLEAN}       { printf("BOOLEAN(%s)\n", yytext); yylval.str = strdup(yytext);  return BOOLEAN; }
{VNULL}          { printf("NULL(%s)\n", yytext); yylval.str = strdup(yytext);  return VNULL;}

[ \t\n\r]+      { /* Skip whitespace */ }

.               { printf("UNKNOWN(%s)\n", yytext); }

%%