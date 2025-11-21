#ifndef _JSTNODE_HPP_
#define _JSTNODE_HPP_

#include <cstdint> // uint8_t ...
#include <limits>  // std::numeric_limits
#include <memory>  // std::unique_ptr
#include <string>  // std::string
#include <vector>  // std::vector

#include "ast.h"

class JsonType
{
public:
    enum JSON_TYPES : uint8_t
    {
        UNKNOWN,
        OBJECT,
        STRING,
        ENUM,
        INTEGER,
        NUMBER,
        BOOL,
        ARRAY
    };

    JsonType() = default;
    JsonType(JSON_TYPES aOperation) : value(aOperation) {}

    JsonType(const std::string &s)
    {
        if (s == "string") { value = JsonType::STRING; }
        else if (s == "object") { value = JsonType::OBJECT; }
        else if (s == "integer") { value = JsonType::INTEGER; }
        else if (s == "enum") { value = JsonType::ENUM; }
        else if (s == "number") { value = JsonType::NUMBER; }
        else if (s == "boolean") { value = JsonType::BOOL; }
        else if (s == "array") { value = JsonType::ARRAY; }
        else {
            value = JsonType::UNKNOWN;
        }
    }

    // Allow switch and comparisons.
    constexpr operator JSON_TYPES() const { return value; }

    // Prevent usage: if(OPERATION_MODE)
    explicit operator bool() const = delete;

    constexpr bool operator==(const JsonType a) const { return value == a.value; }
    constexpr bool operator!=(const JsonType a) const { return value != a.value; }
    // friend std::ostream &operator<<(std::ostream &os, const JsonType &a) { return os << a.toString(); }

    constexpr bool operator==(const JSON_TYPES a) const { return value == a; }
    constexpr bool operator!=(const JSON_TYPES a) const { return value != a; }

    std::string toString() const
    {
        switch (value)
        {
        case UNKNOWN:
            return "Unknown";
        case STRING:
            return "STRING";
        case OBJECT:
            return "OBJECT";
        case ENUM:
            return "ENUM";
        case INTEGER:
            return "INTEGER";
        case NUMBER:
            return "NUMBER";
        case BOOL:
            return "BOOL";
        case ARRAY:
            return "ARRAY";
        }
        return ""; // unreachable
    }

private:
    JSON_TYPES value = UNKNOWN;
};

class JSTNode
{
public:
    // JsonType type = JsonType::UNKNOWN;
    std::vector<JsonType> type{JsonType()};
    std::string name;
    bool hasEnum = false;

    std::vector<std::unique_ptr<JSTNode>> children;
    int64_t minimum = std::numeric_limits<int64_t>::min();
    int64_t maximum = std::numeric_limits<int64_t>::max();
};

class JstGenerator
{
public:
    JstGenerator() = default;
    [[nodiscard]] std::unique_ptr<JSTNode> generateJST(ASTNode *node);
    void print_jst(const JSTNode *const node, int indent);

private:
    void generateJST(ASTNode *node, JSTNode *cstNode);
    void handlePlaceholders();
    std::vector<JSTNode *> placeholders;
    ASTNode *defs = nullptr;
    JSTNode *getPlaceHolder(const std::string &name);
    void handleType(ASTNode *node, JSTNode *jNode);
};

#endif