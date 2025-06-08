#include <cstring>
#include <iostream>
#include <string>

#include "JST.hpp"
#include "ast.h"

std::unique_ptr<JSTNode> JstGenerator::generateJST(ASTNode *node)
{
    auto root = std::make_unique<JSTNode>();
    root->parent = nullptr;
    root->name = "violationSettings";

    generateJST(node, *root);
    return root;
}

void JstGenerator::generateJST(ASTNode *node, JSTNode &parent)
{
    switch (node->type)
    {
    case AST_OBJECT:
    {
        // printf("Object\n");
        break;
    }
    case AST_ARRAY:
    {
        // printf("AST_ARRAY\n");
        break;
    }
    case AST_STRING:
    {
        printf("String: %s\n", node->string_value);
        return;
    }
    case AST_NUMBER:
    {
        printf("Number: %s\n", node->string_value);
        return;
    }
    case AST_PAIR:
    {
        // This check allows keywords to also be property names. So "type", or "required" can be objects names.
        if (node->child_count == 1 &&              // Make sure the child we're about to check is valid
            node->children[0]->type == AST_OBJECT) // If the child is an object its not a property of the parent.
        {
            break;
        }

        if (std::string(node->key).compare("\"$schema\"") == 0) { return; }
        else if (std::string(node->key).compare("\"required\"") == 0) { return; }
        else if (std::string(node->key).compare("\"enum\"") == 0)
        {
            parent.hasEnum = true;
            return;
        }
        else if (std::string(node->key).compare("\"title\"") == 0) { return; }
        else if (std::string(node->key).compare("\"$comment\"") == 0) { return; }
        else if (std::string(node->key).compare("\"default\"") == 0) { return; }
        else if (std::string(node->key).compare("\"description\"") == 0) { return; }
        else if (std::string(node->key).compare("\"items\"") == 0) { break; }
        else if (std::string(node->key).compare("\"pattern\"") == 0) { return; }
        else if (std::string(node->key).compare("\"oneOf\"") == 0) { return; } // TODO: Build enum
        else if (std::string(node->key).compare("\"minimum\"") == 0)
        {
            parent.minimum = std::stoi(node->children[0]->string_value);
            return;
        } // TODO: Modify parent
        else if (std::string(node->key).compare("\"maximum\"") == 0)
        {
            parent.maximum = std::stoi(node->children[0]->string_value);
            return;
        } // TODO: Modify parent
        else if (std::string(node->key).compare("\"minLength\"") == 0)
        {
            parent.minimum = std::stoi(node->children[0]->string_value);
            return;
        } // TODO: Modify parent
        else if (std::string(node->key).compare("\"maxLength\"") == 0)
        {
            parent.maximum = std::stoi(node->children[0]->string_value);
            return;
        } // TODO: Modify parent
        else if (std::string(node->key).compare("\"minItems\"") == 0)
        {
            parent.minimum = std::stoi(node->children[0]->string_value);
            return;
        } // TODO: Modify parent
        else if (std::string(node->key).compare("\"maxItems\"") == 0)
        {
            parent.maximum = std::stoi(node->children[0]->string_value);
            return;
        } // TODO: Modify parent
        else if (std::string(node->key).compare("\"description\"") == 0) { return; }
        else if (std::string(node->key).compare("\"docHint\"") == 0) { return; }
        else if (std::string(node->key).compare("\"type\"") == 0) // TODO: Handle multiple types ex. "type": ["array", "null"]
        {
            parent.type = JsonType(std::string(node->children[0]->string_value).substr(1, std::strlen(node->children[0]->string_value) - 2));
            return;
        }
        else if (std::string(node->key).compare("\"properties\"") == 0) { break; }
        else if (std::string(node->key).compare("\"additionalProperties\"") == 0) { return; }
        // printf("Pair: %s\n", node->key);

        break;
    }
    case AST_BOOLEAN:
    {
        printf("Boolean: %s\n", node->string_value);
        return;
    }
    case AST_INTEGER:
    {
        printf("Integer: %s\n", node->string_value);
        return;
    }
    case AST_NULL:
    {
        printf("NULL: %s\n", node->string_value);
        return;
    }
    default:
    {
        printf("default\n");
        return;
    }
    }
    for (int i = 0; i < node->child_count; ++i)
    {
        if (node->type == AST_PAIR &&
            std::string(node->key).compare("\"properties\"") != 0)
        {
            auto child = std::make_shared<JSTNode>();
            child->parent = &parent;
            if (std::string(node->key).compare("\"items\"") == 0) { child->name = parent.name; }
            else { child->name = std::string(node->key).substr(1, std::strlen(node->key) - 2); }

            // std::cout << child->name << " is a child of " << parent->name << '\n';
            parent.children.push_back(std::move(child));
            generateJST(node->children[i], *(parent.children.back()));
        }
        else
        {
            generateJST(node->children[i], parent);
        }
    }
}

void JstGenerator::print_jst(const JSTNode *const node, int indent)
{
    std::string space;
    for (int i = 0; i < indent; ++i) space += '\t';

    std::cout << space << node->type.toString() << ' ' << node->name << '\n';
    for (const auto &child : node->children)
    {
        print_jst(child.get(), indent + 1);
    }
}
