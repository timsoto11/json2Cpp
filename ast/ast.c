#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

static char *remove_quotes(char *s1)
{
    if (s1[0] == '"' && s1[strlen(s1) - 1] == '"')
    {
        s1[strlen(s1) - 1] = '\0';
        return s1 + 1;
    }
    return s1;
}

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
    ASTNode *node = create_node(AST_STRING);
    node->string_value = strdup(remove_quotes(value));
    return node;
}

ASTNode *create_number_node(char *value)
{
    ASTNode *node = create_node(AST_NUMBER);
    node->string_value = strdup(value);
    return node;
}

ASTNode *create_integer_node(char *value)
{
    ASTNode *node = create_node(AST_NUMBER);
    node->string_value = strdup(value);
    return node;
}

ASTNode *create_boolean_node(char *value)
{
    ASTNode *node = create_node(AST_NUMBER);
    node->string_value = strdup(value);
    return node;
}

ASTNode *create_null_node()
{
    ASTNode *node = create_node(AST_NULL);
    node->string_value = strdup("null");
    return node;
}

ASTNode *create_pair_node(char *key, ASTNode *value)
{
    ASTNode *node = create_node(AST_PAIR);
    node->key = strdup(remove_quotes(key));
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
    case AST_PAIR:
        printf("Pair: %s\n", node->key);
        break;
    case AST_BOOLEAN:
        printf("Boolean: %s\n", node->string_value);
        break;
    case AST_INTEGER:
        printf("Integer: %s\n", node->string_value);
        break;
    case AST_NULL:
        printf("NULL: %s\n", node->string_value);
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