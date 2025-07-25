#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "cppGen.hpp"
#include "parser.h"

ASTNode *root;

int main()
{
    yyparse();
    // printf("✅ AST:\n");
    // print_ast(root, 0);
    // printf("\n");

    cpp::generator a(root);

    free_ast(root);

    return 0;
}