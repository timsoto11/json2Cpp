#include <cstring>
#include <iostream>
#include <string>

#include "JST.hpp"
#include "ast.h"

std::unique_ptr<JSTNode> JstGenerator::generateJST(ASTNode *node)
{
    auto root = std::make_unique<JSTNode>();
    root->parent = nullptr;
    root->name = "settings";

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
        if (std::string(node->key).compare("\"$id\"") == 0) { return; }
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
        else if (std::string(node->key).compare("\"oneOf\"") == 0)
        {
            parent.hasEnum = true;
            // return;
        }
        else if (std::string(node->key).compare("\"minimum\"") == 0)
        {
            parent.minimum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("\"maximum\"") == 0)
        {
            parent.maximum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("\"minLength\"") == 0)
        {
            parent.minimum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("\"maxLength\"") == 0)
        {
            parent.maximum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("\"minItems\"") == 0)
        {
            parent.minimum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("\"maxItems\"") == 0)
        {
            parent.maximum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("\"description\"") == 0) { return; }
        else if (std::string(node->key).compare("\"docHint\"") == 0) { return; }
        else if (std::string(node->key).compare("\"type\"") == 0) // TODO: Handle multiple types ex. "type": ["array", "null"]
        {
            parent.type = JsonType(std::string(node->children[0]->string_value).substr(1, std::strlen(node->children[0]->string_value) - 2));
            return;
        }
        else if (std::string(node->key).compare("\"properties\"") == 0) { break; }
        else if (std::string(node->key).compare("\"additionalProperties\"") == 0) { return; }
        else if (std::string(node->key).compare("\"$ref\"") == 0)
        {
            std::cout << "Placeholder found " << node->children[0]->string_value << "\n";
            parent.name = node->children[0]->string_value;

            parent.type = JsonType::UNKNOWN;
            placeholders.push_back(&parent);

            return;
        }
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
            const auto tmpName = std::string(node->key).substr(1, std::strlen(node->key) - 2);
            // std::cout << tmpName << " is a child of " << parent.name << '\n';

            if (tmpName == "$defs")
            {
                // print_ast(node->children[i], 0);
                auto jstParent = getPlaceHolder(node->children[i]);
                if (jstParent != nullptr)
                {

                    const std::string reference = jstParent->parent->children.back().name;
                    const std::size_t pos1 = reference.find_last_of('/') + 1;
                    const std::size_t pos2 = reference.find_last_of('\"');
                    jstParent->parent->name = reference.substr(pos1, pos2 - pos1);
                    jstParent->parent->children.pop_back();

                    generateJST(node->children[i], *jstParent->parent);
                }
            }
            else
            {
                auto child = JSTNode();
                child.parent = &parent;
                if (std::string(node->key).compare("\"items\"") == 0) { child.name = parent.name; }
                else { child.name = tmpName; }

                parent.children.push_back(child);
                generateJST(node->children[i], parent.children.back());
            }
        }
        else
        {
            generateJST(node->children[i], parent);
        }
    }
}

JSTNode *JstGenerator::getPlaceHolder(ASTNode *node)
{

    for (auto iter = placeholders.begin(); iter != placeholders.end();)
    {
        auto tmp = iter;
        std::cout << "placeholder name: " << (*iter)->name << '\n';
        std::cout << "definition name: " << node->children[0]->key << '\n';

        iter = placeholders.erase(iter);
        return *tmp;
    }

    return nullptr;
}

void JstGenerator::print_jst(const JSTNode *const node, int indent)
{
    std::string space;
    for (int i = 0; i < indent; ++i) space += '\t';

    std::cout << space << node->type.toString() << ' ' << node->name << '\n';
    for (const auto &child : node->children)
    {
        print_jst(&child, indent + 1);
    }
}
