#include <cstring>
#include <iostream>
#include <string>

#include "JST.hpp"
#include "ast.h"

std::unique_ptr<JSTNode> JstGenerator::generateJST(ASTNode *node)
{
    auto root = std::make_unique<JSTNode>();
    root->name = "settings";

    generateJST(node, root.get());
    handlePlaceholders();

    // std::cout << "\n\n\n";
    // print_jst(root.get(), 0);
    // std::cout << "\n\n\n";

    return root;
}

void JstGenerator::generateJST(ASTNode *node, JSTNode *jNode)
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
        // TODO: need to manage what can be a variable name. Right now nothing can be named dependentRequired
        if (std::string(node->key).compare("dependentRequired") == 0) { return; }
        // This check allows keywords to also be property names. So "type", or "required" can be objects names.
        if (node->child_count == 1 &&              // Make sure the child we're about to check is valid
            node->children[0]->type == AST_OBJECT) // If the child is an object its not a property of the parent.
        {
            break;
        }

        // Need to put these into a map and benchmark. This is probably pretty SLOW.
        if (std::string(node->key).compare("$schema") == 0) { return; }
        if (std::string(node->key).compare("$id") == 0) { return; }
        else if (std::string(node->key).compare("required") == 0) { return; }
        else if (std::string(node->key).compare("enum") == 0)
        {
            jNode->hasEnum = true;
            return;
        }
        else if (std::string(node->key).compare("title") == 0) { return; }
        else if (std::string(node->key).compare("$comment") == 0) { return; }
        else if (std::string(node->key).compare("default") == 0) { return; }
        else if (std::string(node->key).compare("description") == 0) { return; }
        else if (std::string(node->key).compare("items") == 0) { break; }
        else if (std::string(node->key).compare("pattern") == 0) { return; }
        else if (std::string(node->key).compare("oneOf") == 0)
        {
            jNode->hasEnum = true;
            return;
        }
        else if (std::string(node->key).compare("minimum") == 0)
        {
            jNode->minimum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("maximum") == 0)
        {
            jNode->maximum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("minLength") == 0)
        {
            jNode->minimum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("maxLength") == 0)
        {
            jNode->maximum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("minItems") == 0)
        {
            jNode->minimum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("maxItems") == 0)
        {
            jNode->maximum = std::stoi(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("description") == 0) { return; }
        else if (std::string(node->key).compare("docHint") == 0) { return; }
        else if (std::string(node->key).compare("type") == 0) // TODO: Handle multiple types ex. "type": ["array", "null"]
        {
            jNode->type = JsonType(node->children[0]->string_value);
            return;
        }
        else if (std::string(node->key).compare("properties") == 0) { break; }
        else if (std::string(node->key).compare("additionalProperties") == 0) { return; }
        else if (std::string(node->key).compare("$ref") == 0)
        {
            // Change parents name from items to whatever string_value is
            jNode->name = node->children[0]->string_value;
            jNode->type = JsonType::UNKNOWN;
            placeholders.push_back(jNode);

            return;
        }
        //

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
        if (node->type == AST_PAIR && std::string(node->key).compare("properties") != 0)
        {
            const auto tmpName = std::string(node->key);
            // std::cout << tmpName << " is a child of " << jNode->name << '\n';

            // definitions and references will be handled separately because we don't know which will come first.
            if (tmpName == "$defs")
            {
                if (defs != nullptr) { throw std::invalid_argument("multiple $defs"); }
                defs = node;
                continue;
            }

            jNode->children.push_back({std::make_unique<JSTNode>()});
            auto &child = jNode->children.back();
            child->name = tmpName;

            // std::cout << child.name << "(" << &child << ')' << " of " << jNode->name << "(" << &jNode << ')' << '\n';
            generateJST(node->children[i], jNode->children.back().get());
        }
        else
        {
            // std::cout << parent->name << "(" << &parent << ')' << '\n';
            generateJST(node->children[i], jNode);
        }
    }
}

JSTNode *JstGenerator::getPlaceHolder(const std::string &name)
{
    for (auto iter = placeholders.begin(); iter != placeholders.end();)
    {
        std::cout << "placeholder name: " << (*iter)->name << " : " << name << '\n';
        if (name != (*iter)->name)
        {
            std::cout << "not the same!\n";
            iter++;
            continue;
        }
        const auto tmp = *iter;
        placeholders.erase(iter);
        std::cout << "returning " << tmp->name << '\n';
        return tmp;
    }

    return nullptr;
}

void JstGenerator::handlePlaceholders()
{
    if (defs == nullptr) { return; }

    for (int i = 0; i < defs->children[0]->child_count; i++)
    {
        const std::string placeholderName = std::string("#/$defs/") + defs->children[0]->children[i]->key;
        auto jstParent = getPlaceHolder(placeholderName);
        if (jstParent != nullptr)
        {
            jstParent->name = defs->children[0]->children[i]->key;
            for (int j = 0; j < defs->children[0]->children[i]->child_count; j++)
            {
                generateJST(defs->children[0]->children[i]->children[j], jstParent);
            }
        }
    }
}

void JstGenerator::print_jst(const JSTNode *const node, int indent)
{
    std::string space;
    for (int i = 0; i < indent; ++i) space += '\t';

    std::cout << space << node->type.toString() << ' ' << node->name << ' ' << node << '\n';
    for (const auto &child : node->children)
    {
        print_jst(child.get(), indent + 1);
    }
}
