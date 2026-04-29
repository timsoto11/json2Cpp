#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "cppGen.hpp"
#include "parser.h"

ASTNode *root; // parser.c expects this to be a global

int main()
{
    // Profile how long parsing takes. This is the slowest part of the process by far.
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    yyparse();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Parsing took " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds.\n";
    // printf("✅ AST:\n");
    // print_ast(root, 0);
    // printf("\n");

    cpp::generator a(root);

    free_ast(root);

    return 0;
}