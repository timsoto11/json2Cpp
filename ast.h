#ifndef AST_H
#define AST_H

typedef enum
{
    AST_OBJECT,
    AST_ARRAY,
    AST_STRING,
    AST_NUMBER,
    AST_INTEGER,
    AST_BOOLEAN,
    AST_NULL,
    AST_KEYWORD_PAIR
} ASTNodeType;

typedef struct ASTNode
{
    ASTNodeType type;
    char *key;                 // For object pairs
    char *string_value;        // For strings/numbers
    struct ASTNode **children; // For arrays/objects
    int child_count;
} ASTNode;

ASTNode *create_node(ASTNodeType type);
void add_child(ASTNode *parent, ASTNode *child);
ASTNode *create_string_node(char *value);
ASTNode *create_number_node(char *value);
ASTNode *create_integer_node(char *value);
ASTNode *create_boolean_node(char *value);
ASTNode *create_null_node();
ASTNode *create_pair_node(char *key, ASTNode *value);
void print_ast(ASTNode *node, int indent);
void free_ast(ASTNode *node);

#endif