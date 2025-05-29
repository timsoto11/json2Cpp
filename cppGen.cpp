#include <cstring>
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

    case AST_ARRAY:
        printf("Array\n");

    case AST_STRING:
        printf("String: %s\n", node->string_value);
        return;
    case AST_NUMBER:
        printf("Number: %s\n", node->string_value);
        return;
    case AST_PAIR:
        printf("Pair: %s\n", node->key);

    case AST_BOOLEAN:
        printf("Boolean: %s\n", node->string_value);

    case AST_INTEGER:
        printf("Integer: %s\n", node->string_value);

    case AST_NULL:
        printf("NULL: %s\n", node->string_value);
    }
}

generator::generator()
{
    // const std::string name = "violationSettings";
    // // print(node, 0);
    // for (int i = 0; i < node->child_count; i++)
    // {
    //     // printNode(node->children[i], 0);
    //     ASTNode *pair = node->children[i];

    //     if (std::string(pair->key).compare("\"properties\"") != 0) { continue; }
    //     for (int j = 0; j < pair->child_count; j++)
    //     {
    //         generateStruct(node, name);
    //     }
    // }
}

generator::generator(ASTNode *node)
{
    const std::string name = "violationSettings";

    generateStruct(node, name);

    for (const auto &str : structStrings)
    {
        std::cout << str << '\n';
    }
}

CppType map_type(ASTNode *node)
{
    switch (node->type)
    {

    case AST_STRING:
    {
        std::string str = node->string_value;
        str = str.substr(1, str.length() - 2);
        return CppType(str);
    }
    case AST_OBJECT:
    case AST_ARRAY:
    case AST_NUMBER:
    case AST_INTEGER:
    case AST_BOOLEAN:
    case AST_NULL:
    case AST_PAIR:
    default:
        return CppType::UNKNOWN;
    }

    return CppType::UNKNOWN; // unreachable
}

void generator::generateStruct(ASTNode *node, std::string structName)
{
    try
    {
        for (int i = 0; i < node->child_count; i++)
        {
            ASTNode *pair = node->children[i];

            if (std::string(pair->key).compare("\"properties\"") != 0) { continue; }
            for (int j = 0; j < pair->child_count; j++)
            {

                std::string structStr = "struct " + structName + " {\n";

                for (int i = 0; i < node->child_count; ++i)
                {
                    ASTNode *pair = node->children[i];

                    std::string key = pair->key;

                    if (key.compare("\"properties\"") == 0)
                    {
                        ASTNode *properties = pair->children[0]; // value node (object)
                        for (int j = 0; j < properties->child_count; ++j)
                        {
                            ASTNode *prop_pair = properties->children[j];
                            std::string prop_name = std::string(prop_pair->key).substr(1, strlen(prop_pair->key) - 2);

                            ASTNode *prop_object = prop_pair->children[0];
                            CppType type = CppType::UNKNOWN;
                            std::string typeStr;

                            for (int k = 0; k < prop_object->child_count; ++k)
                            {
                                ASTNode *type_pair = prop_object->children[k];

                                if (std::string(type_pair->key).compare("\"type\"") == 0)
                                {
                                    type = map_type(type_pair->children[0]);
                                    typeStr = type.toString();
                                    if (type == CppType::UNKNOWN) // assuming for now this is an object
                                    {
                                        std::string structName = prop_name;
                                        structName[0] = std::toupper(structName[0]);
                                        generateStruct(prop_object, structName);
                                        typeStr = structName;
                                    }
                                }
                            }

                            structStr += '\t' + typeStr + " " + prop_name + ";\n";
                        }
                    }
                }
                structStr += "};\n";
                structStrings.push_back(structStr);
            }
        }
    }
    catch (...)
    {
        std::cout << "Caught\n";
    }
}
