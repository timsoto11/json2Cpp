#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.h"

ASTNode *root;

int main()
{
    yyparse();
    printf("✅ AST:\n");
    print_ast(root, 0);
    free_ast(root);
    return 0;
}