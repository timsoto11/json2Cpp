#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

ASTNode *create_node(ASTNodeType type)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->key = NULL;
    node->string_value = NULL;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

ASTNode *create_string_node(char *value)
{
    printf("Creating string node with value: %s\n", value);
    ASTNode *node = create_node(AST_STRING);
    node->string_value = strdup(value);
    return node;
}

ASTNode *create_number_node(char *value)
{
    printf("Creating number node with value: %s\n", value);
    ASTNode *node = create_node(AST_NUMBER);
    node->string_value = strdup(value);
    return node;
}

ASTNode *create_integer_node(char *value)
{
    printf("Creating integer node with value: %s\n", value);
    ASTNode *node = create_node(AST_NUMBER);
    node->string_value = strdup(value);
    return node;
}

ASTNode *create_boolean_node(char *value)
{
    printf("Creating bool node with value: %s\n", value);
    ASTNode *node = create_node(AST_NUMBER);
    node->string_value = strdup(value);
    return node;
}

ASTNode *create_null_node()
{
    printf("Creating null node with value: null\n");
    ASTNode *node = create_node(AST_NULL);
    node->string_value = strdup("null");
    return node;
}

ASTNode *create_pair_node(char *key, ASTNode *value)
{
    printf("Creating pair node with key: %s\n", key);
    ASTNode *node = create_node(AST_KEYWORD_PAIR);
    node->key = strdup(key);
    add_child(node, value);
    return node;
}

void add_child(ASTNode *parent, ASTNode *child)
{
    parent->children = realloc(parent->children, sizeof(ASTNode *) * (parent->child_count + 1));
    parent->children[parent->child_count++] = child;
}

void print_ast(ASTNode *node, int indent)
{
    for (int i = 0; i < indent; ++i) printf("  ");
    switch (node->type)
    {
    case AST_OBJECT:
        printf("Object\n");
        break;
    case AST_ARRAY:
        printf("Array\n");
        break;
    case AST_STRING:
        printf("String: %s\n", node->string_value);
        return;
    case AST_NUMBER:
        printf("Number: %s\n", node->string_value);
        return;
    case AST_KEYWORD_PAIR:
        printf("Pair: %s\n", node->key);
        break;
    }
    for (int i = 0; i < node->child_count; ++i)
    {
        print_ast(node->children[i], indent + 1);
    }
}

void free_ast(ASTNode *node)
{
    if (!node) return;
    free(node->key);
    free(node->string_value);
    for (int i = 0; i < node->child_count; ++i)
        free_ast(node->children[i]);
    free(node->children);
    free(node);
}