// This %{ %} means this code will be in the source code
%{
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

extern ASTNode *root;

void yyerror(const char *s);
%}

// This %code requires{ } means this code will be in both the header and source code
%code requires {
#ifdef __cplusplus
extern "C" {
#endif
int yyparse(void);
#ifdef __cplusplus
}
#endif
}

%union {
    char *str;
    ASTNode *node;
}

%token LBRACE RBRACE LBRACKET RBRACKET COLON COMMA
%token <str> STRING NUMBER INTEGER BOOLEAN VNULL
%token <str> KEYWORD

%type <node> schema object members pair value array elements

%start schema

%%

schema
    : object                   { $$ = $1; root = $$; }
    | { $$ = NULL; }
    ;

object
    : LBRACE members RBRACE    { $$ = create_node(AST_OBJECT); for (int i = 0; i < $2->child_count; ++i) add_child($$, $2->children[i]); free($2->children); free($2); }
    | LBRACE RBRACE            { }
    ;

members
    : pair                     { $$ = create_node(AST_OBJECT); add_child($$, $1); }
    | members COMMA pair       { $$ = $1; add_child($$, $3); }
    ;

pair
    : STRING COLON value       { $$ = create_pair_node($1, $3); }
    | KEYWORD COLON value      { $$ = create_keyword_pair_node($1, $3); }
    ;

array
    : LBRACKET elements RBRACKET { $$ = create_node(AST_ARRAY); for (int i = 0; i < $2->child_count; ++i) add_child($$, $2->children[i]); free($2->children); free($2); }
    | LBRACKET RBRACKET          { $$ = create_node(AST_ARRAY); }
    ;

elements
    : value                    { $$ = create_node(AST_ARRAY); add_child($$, $1); }
    | elements COMMA value     { $$ = $1; add_child($$, $3); }
    ;

value
    : STRING                   { $$ = create_string_node($1); free($1); }
    | NUMBER                   { $$ = create_number_node($1); free($1); }
    | INTEGER                  { $$ = create_integer_node($1); free($1); }
    | BOOLEAN                  { $$ = create_integer_node($1); free($1); }
    | VNULL                    { $$ = create_null_node(); free($1); }
    | object                   { $$ = $1; }
    | array                    { $$ = $1; }
    ;


%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}