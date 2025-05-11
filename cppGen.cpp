#include <iostream>
#include <string>
#include <unordered_map>

#include "cppGen.hpp"

using namespace cpp;

static void generate(ASTNode *node, uint32_t level);
static void generateStruct(ASTNode *node, std::string structName);
std::string map_type(const std::string &type);

void printNode(ASTNode *node, int level)
{
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
}

cppGenerator::cppGenerator(ASTNode *node) : node(node)
{
}

cppGenerator::~cppGenerator()
{
}

void cppGenerator::generate()
{
    static std::string name;
    for (int i = 0; i < node->child_count; i++)
    {
        ASTNode *pair = node->children[i];
        if (!pair || pair->type == AST_PAIR) { name = pair->key; }

        if (std::string(pair->key).compare("\"properties\"") != 0) { continue; }
        for (int j = 0; j < pair->child_count; j++)
        {
            generateStruct(node, name);
        }
    }
}

std::string map_type(const std::string &type)
{
    if (type == "\"string\"") return "std::string";
    if (type == "\"number\"" || type == "\"integer\"") return "double"; // TODO: determine type based off of minimum or maximun if provided
    if (type == "\"boolean\"") return "bool";
    // TODO: Support vectors later, maybe even std::array
    return "/* unknown */";
}

void generateStruct(ASTNode *node, std::string structName)
{
    try
    {
        std::cout << "struct " << structName << " {\n";

        for (int i = 0; i < node->child_count; ++i)
        {
            ASTNode *pair = node->children[i];
            // if (!pair || pair->type != AST_KEYWORD_PAIR) { continue; }

            std::string key = pair->key;

            if (key.compare("\"properties\"") == 0)
            {
                ASTNode *properties = pair->children[0]; // value node (object)
                for (int j = 0; j < properties->child_count; ++j)
                {
                    ASTNode *prop_pair = properties->children[j];
                    std::string prop_name = prop_pair->key;

                    ASTNode *prop_object = prop_pair->children[0];
                    std::string typeStr = "/* unknown */";

                    for (int k = 0; k < prop_object->child_count; ++k)
                    {
                        ASTNode *type_pair = prop_object->children[k];
                        if (std::string(type_pair->key).compare("\"type\"") == 0)
                        {
                            // ASTNode *type_node = type_pair->children[0];
                            // typeStr = map_type(type_node->string_value);
                        }
                    }

                    std::cout << "    " << typeStr << " " << prop_name.substr(1, prop_name.length() - 2) << ";\n"; // remove quotes
                }
            }
        }

        std::cout << "};\n";
    }
    catch (...)
    {
        std::cout << "Caught\n";
    }
}
