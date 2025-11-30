#include <cstring>
#include <iostream>
#include <string>

#include "JST.hpp"
#include "ast.h"

std::unique_ptr<JSTNode> JstGenerator::generateJST(ASTNode *node)
{
    auto root = std::make_unique<JSTNode>(nullptr);
    root->name = "settings";

    generateJST(node, root.get());
    handlePlaceholders();
    if (placeholders.empty() == false)
    {
        std::cout << "Unresolvable references.\n";
        prunePlaceholders();
    }

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
        // if (jNode->type.at(0) == JsonType::ENUM) { printf("AST_ARRAY\n"); }
        break;
    }
    case AST_STRING:
    {
        if (jNode->type.at(0) == JsonType::ENUM)
        {
            // printf("String: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type.at(0) = JsonType::STRING;
            jNode->children.back()->name = node->string_value;
        }
        return;
    }
    case AST_NUMBER:
    {
        if (jNode->type.at(0) == JsonType::ENUM)
        {
            // printf("Number: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type.at(0) = JsonType::NUMBER;
            jNode->children.back()->name = node->string_value;
        }
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
            jNode->type.at(0) = JsonType("enum");
            break;
            // return;
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
            handleType(node->children[0], jNode);
            return;
        }
        else if (std::string(node->key).compare("properties") == 0) { break; }
        else if (std::string(node->key).compare("additionalProperties") == 0) { return; }
        else if (std::string(node->key).compare("$ref") == 0)
        {
            // Change parents name from items to whatever string_value is
            jNode->name = node->children[0]->string_value;
            jNode->type.push_back(JsonType::UNKNOWN);
            placeholders.push_back(jNode);

            return;
        }
        //

        break;
    }
    case AST_BOOLEAN:
    {
        if (jNode->type.at(0) == JsonType::ENUM)
        {
            // printf("Boolean: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type.at(0) = JsonType::BOOL;
            jNode->children.back()->name = node->string_value;
        }
        return;
    }
    case AST_INTEGER:
    {
        if (jNode->type.at(0) == JsonType::ENUM)
        {
            // printf("Integer: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type.at(0) = JsonType::INTEGER;
            jNode->children.back()->name = node->string_value;
        }
        return;
    }
    case AST_NULL:
    {
        if (jNode->type.at(0) == JsonType::ENUM)
        {
            // printf("NULL: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type.at(0) = JsonType::NULLTYPE;
            jNode->children.back()->name = node->string_value;
        }
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
            std::string(node->key).compare("properties") != 0 &&
            std::string(node->key).compare("enum") != 0)
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

            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
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

/* For Type each JSON node has a vector with the 0th index initialized to UNKNOWN
 * So we have to make sure we overwrite that first before adding to the vector.
 */
void JstGenerator::handleType(ASTNode *node, JSTNode *jNode)
{
    if (node->type == AST_STRING) // Handle "type" : "number"
    {
        jNode->type.at(0) = JsonType(node->string_value);
    }
    else if (node->type == AST_ARRAY) // Handle "type" : ["number"]
    {
        for (int i = 0; i < node->child_count; i++)
        {
            if (i == 0) { jNode->type.at(0) = JsonType(node->children[i]->string_value); }
            else
            {
                jNode->type.push_back(JsonType(node->children[i]->string_value));
            }
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

// This goes through the unresolved placeholders and removes from the Json tree we've built.
void JstGenerator::prunePlaceholders()
{
    for (auto iter = placeholders.begin(); iter != placeholders.end(); iter++)
    {
        auto parent = (*iter)->parent;
        auto childPtrToRemove = *iter;
        for (auto child = parent->children.begin(); child != parent->children.end();)
        {
            if (child->get() == childPtrToRemove)
            {
                child = parent->children.erase(child);
                continue;
            }
            child++;
        }
    }
}

void JstGenerator::print_jst(const JSTNode *const node, int indent)
{
    std::string space;
    for (int i = 0; i < indent; ++i) space += '\t';

    std::cout << space;
    for (uint64_t i = 0; i < node->type.size(); i++)
    {
        const auto &type = node->type.at(i);
        std::cout << type.toString();
        if (i < node->type.size() - 1) { std::cout << ','; }
    }
    std::cout << ' ' << node->name << ' ' << node << '\n';
    for (const auto &child : node->children)
    {
        print_jst(child.get(), indent + 1);
    }
}
