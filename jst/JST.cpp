#include <cstring>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

#include "JST.hpp"
#include "ast.h"

enum class TokenType
{
    DEPENDENT_REQUIRED,
    SCHEMA,
    ID,
    REQUIRED,
    ENUM,
    TITLE,
    DEFAULT,
    COMMENT,
    ITEMS,
    PATTERN,
    ONE_OF,
    MINIMUM,
    MAXIMUM,
    MIN_LENGTH,
    MAX_LENGTH,
    MIN_ITEMS,
    MAX_ITEMS,
    DESCRIPTION,
    DOC_HINT,
    TYPE,
    PROPERTIES,
    PATTERN_PROPERTIES,
    ADDITIONAL_PROPERTIES,
    REF,
    // DEFS,
    IF,
    ELSE,
    THEN,
    NOT_A_KEYWORD
};

static TokenType GetKeywordToken(std::string_view token)
{
    static const std::unordered_map<std::string_view, TokenType> keywords = {
        {"dependentRequired", TokenType::DEPENDENT_REQUIRED},
        {"$schema", TokenType::SCHEMA},
        {"$id", TokenType::ID},
        {"required", TokenType::REQUIRED},
        {"enum", TokenType::ENUM},
        {"title", TokenType::TITLE},
        {"default", TokenType::DEFAULT},
        {"$comment", TokenType::COMMENT},
        {"items", TokenType::ITEMS},
        {"pattern", TokenType::PATTERN},
        {"oneOf", TokenType::ONE_OF},
        {"minimum", TokenType::MINIMUM},
        {"maximum", TokenType::MAXIMUM},
        {"minLength", TokenType::MIN_LENGTH},
        {"maxLength", TokenType::MAX_LENGTH},
        {"minItems", TokenType::MIN_ITEMS},
        {"maxItems", TokenType::MAX_ITEMS},
        {"description", TokenType::DESCRIPTION},
        {"docHint", TokenType::DOC_HINT},
        {"type", TokenType::TYPE},
        {"properties", TokenType::PROPERTIES},
        {"patternProperties", TokenType::PATTERN_PROPERTIES},
        {"additionalProperties", TokenType::ADDITIONAL_PROPERTIES},
        {"$ref", TokenType::REF},
        // {"$defs", TokenType::DEFS}, // not sure what to do with this yet.
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"then", TokenType::THEN}

    };

    auto it = keywords.find(token);
    return it != keywords.end() ? it->second : TokenType::NOT_A_KEYWORD;
}

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
        if (jNode->type == JsonType::ENUM)
        {
            // printf("String: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type = JsonType::STRING;
            jNode->children.back()->name = node->string_value;
        }
        return;
    }
    case AST_NUMBER:
    {
        if (jNode->type == JsonType::ENUM)
        {
            // printf("Number: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type = JsonType::NUMBER;
            jNode->children.back()->name = node->string_value;
        }
        return;
    }
    case AST_PAIR:
    {
        TokenType token = GetKeywordToken(node->key);
        switch (token)
        {
        case TokenType::DEPENDENT_REQUIRED:
            return;
        case TokenType::SCHEMA:
            return;
        case TokenType::ID:
            return;
        case TokenType::REQUIRED:
            return;
        case TokenType::ENUM:
            jNode->hasEnum = true;
            jNode->type = JsonType::ENUM;
            break;
        case TokenType::TITLE:
            return;
        case TokenType::COMMENT:
            return;
        case TokenType::DEFAULT:
            return;
        case TokenType::ITEMS:
            break;
        case TokenType::PATTERN:
            return;
        case TokenType::ONE_OF:
            jNode->hasEnum = true;
            return;
        case TokenType::MINIMUM:
            jNode->minimum = std::stoi(node->children[0]->string_value);
            return;
        case TokenType::MAXIMUM:
            jNode->maximum = std::stoi(node->children[0]->string_value);
            return;
        case TokenType::MIN_LENGTH:
            jNode->minimum = std::stoi(node->children[0]->string_value);
            return;
        case TokenType::MAX_LENGTH:
            jNode->maximum = std::stoi(node->children[0]->string_value);
            return;
        case TokenType::MIN_ITEMS:
            jNode->minimum = std::stoi(node->children[0]->string_value);
            return;
        case TokenType::MAX_ITEMS:
            jNode->maximum = std::stoi(node->children[0]->string_value);
            return;
        case TokenType::DESCRIPTION:
            return;
        case TokenType::DOC_HINT:
            return;
        case TokenType::TYPE:
            handleType(node->children[0], jNode);
            return;
        case TokenType::PROPERTIES:
            break;
        case TokenType::PATTERN_PROPERTIES:
            break;
        case TokenType::ADDITIONAL_PROPERTIES:
            return;
        case TokenType::REF:
            // Change parents name from items to whatever string_value is
            jNode->name = node->children[0]->string_value;
            placeholders.push_back(jNode);
            return;
        case TokenType::IF:
            return;
        case TokenType::THEN:
            return;
        case TokenType::ELSE:
            return;
        case TokenType::NOT_A_KEYWORD:
            break;
        }

        break;
    }
    case AST_BOOLEAN:
    {
        if (jNode->type == JsonType::ENUM)
        {
            // printf("Boolean: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type = JsonType::BOOL;
            jNode->children.back()->name = node->string_value;
        }
        return;
    }
    case AST_INTEGER:
    {
        if (jNode->type == JsonType::ENUM)
        {
            // printf("Integer: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type = JsonType::INTEGER;
            jNode->children.back()->name = node->string_value;
        }
        return;
    }
    case AST_NULL:
    {
        if (jNode->type == JsonType::ENUM)
        {
            // printf("NULL: %s\n", node->string_value);
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            jNode->children.back()->type = JsonType::NULLTYPE;
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
            std::string(node->key).compare("patternProperties") != 0 &&
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
        else if (node->type == AST_PAIR &&
                 std::string(node->key).compare("patternProperties") == 0)
        {
            /* For pattern properties we're going to create the JSON tree such that both
             * key and value are variables. This is because when parsing the JSON the key
             * may hold information that needs to be known at runtime.
             */
            jNode->children.push_back({std::make_unique<JSTNode>(jNode)});
            auto &child = jNode->children.back();
            child->type = JsonType::ARRAY;
            child->name = "items" + std::to_string(++dynamicPatternsCounter);
            child->children.push_back({std::make_unique<JSTNode>(jNode)});
            auto &newChild1 = child->children.back();
            newChild1->type = JsonType::OBJECT;
            newChild1->name = child->name;
            newChild1->children.push_back({std::make_unique<JSTNode>(jNode)});
            auto &newChild2 = newChild1->children.back();
            newChild2->type = JsonType::STRING;
            newChild2->name = "key";
            newChild2->regex = node->children[i]->children[0]->key;
            // Now we need to add the fields that object that corresponds to the dynamic object
            newChild1->children.push_back({std::make_unique<JSTNode>(jNode)});
            auto &newChild3 = newChild1->children.back();
            newChild3->name = "value";

            // TODO: FIX THIS UNSAFE MADNESS
            generateJST(node->children[i]->children[0]->children[0]->children[0], newChild1->children.back().get());
            // print_ast(node, 0);
        }
        else
        {
            // std::cout << parent->name << "(" << &parent << ')' << '\n';
            generateJST(node->children[i], jNode);
        }
    }
}

void JstGenerator::handleType(ASTNode *node, JSTNode *jNode)
{
    if (node->type == AST_STRING) // Handle "type" : "number"
    {
        jNode->type = JsonType(node->string_value);
    }
    else if (node->type == AST_ARRAY) // Handle "type" : ["number"]
    {
        for (int i = 0; i < node->child_count; i++)
        {
            jNode->type += JsonType(node->children[i]->string_value);
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
    for (int i = 0; i < indent; ++i) std::cout << '\t';
    std::cout << node->type.toString();
    std::cout << ' ' << node->name << '\n';
    for (const auto &child : node->children)
    {
        print_jst(child.get(), indent + 1);
    }
}
