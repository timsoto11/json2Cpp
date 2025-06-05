#include <cstring>
#include <fstream>  // std::ofstream
#include <iostream> // std::iostream
#include <memory>   // unique and make_unique
#include <string>
#include <vector>

#include "cppGen.hpp"

using namespace cpp;

std::string underscoreToCamelCase(const std::string &str)
{
    std::string result;
    bool nextUpper = true;

    for (char ch : str)
    {
        if (ch == '_') { nextUpper = true; }
        else
        {
            if (nextUpper)
            {
                result += std::toupper(ch);
                nextUpper = false;
            }
            else { result += ch; }
        }
    }

    return result;
}

generator::generator(ASTNode *node)
{
    auto root = std::make_unique<CSTNode>();
    root->parent = nullptr;
    root->name = "violationSettings";

    generateCST(node, *root);

    std::cout << "Generated Tree:\n";
    // print_cst(root.get(), 0);
    // std::cout << "\n\n";
    generateStruct(*root);

    std::ofstream fs("violationSettings.hpp");

    fs << "#include <string>\n";
    fs << "#include <vector>\n";
    fs << '\n';

    for (const auto &str : structStrings)
    {
        fs << str << '\n';
    }
}

void generator::print_cst(const CSTNode *const node, int indent)
{
    std::string space;
    for (int i = 0; i < indent; ++i) space += '\t';

    std::cout << space << node->type.toString() << ' ' << node->name << '\n';
    for (const auto &child : node->children)
    {
        print_cst(child.get(), indent + 1);
    }
}

void cpp::generator::generateStruct(CSTNode &node)
{
    std::string structStr;

    structStr += "struct " + underscoreToCamelCase(node.name) + " {\n";
    for (const auto &child : node.children)
    {
        if (child->type == CppType::OBJECT)
        {
            structStr += "\t" + underscoreToCamelCase(child->name) + ' ' + child->name + ";\n";
            generateStruct(*child);
            // generate Struct for this
        }
        else if (child->type == CppType::ARRAY)
        {
            structStr += "\t" + child->type.toString() + '<' + underscoreToCamelCase(child->name) + "> " + child->name + ";\n";
            generateStruct(*(child->children[0]));
            // generate Struct for this
        }
        else
        {
            structStr += "\t" + child->type.toString() + ' ' + child->name + ";\n";
        }
    }
    structStr += "};\n";
    structStrings.push_back(structStr);
}

void generator::generateCST(ASTNode *node, CSTNode &parent)
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
            parent.type = CppType(std::string(node->children[0]->string_value).substr(1, strlen(node->children[0]->string_value) - 2));
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
            auto child = std::make_unique<CSTNode>();
            child->parent = &parent;
            if (std::string(node->key).compare("\"items\"") == 0) { child->name = parent.name; }
            else { child->name = std::string(node->key).substr(1, strlen(node->key) - 2); }

            // std::cout << child->name << " is a child of " << parent->name << '\n';
            parent.children.push_back(std::move(child));
            generateCST(node->children[i], *(parent.children.back()));
        }
        else
        {
            generateCST(node->children[i], parent);
        }
    }
}