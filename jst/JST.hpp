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
        UNKNOWN = 0,
        OBJECT = 1u << 0,
        STRING = 1u << 1,
        ENUM = 1u << 2,
        INTEGER = 1u << 3,
        NUMBER = 1u << 4,
        BOOL = 1u << 5,
        ARRAY = 1u << 6,
        NULLTYPE = 1u << 7,
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

    constexpr bool operator==(const JsonType a) const { return value & a.value; }
    constexpr bool operator!=(const JsonType a) const { return !(value & a.value); }
    constexpr bool operator==(const JSON_TYPES a) const { return value & a; }
    constexpr bool operator!=(const JSON_TYPES a) const { return !(value & a); }
    // friend std::ostream &operator<<(std::ostream &os, const JsonType &a) { return os << a.toString(); }

    JsonType &operator+=(const JsonType &rhs)
    {
        value = static_cast<JSON_TYPES>(this->value | rhs.value);
        return *this;
    }

    friend JsonType operator+(JsonType lhs, const JsonType &rhs)
    {
        lhs += rhs;
        return lhs;
    }

    int numberOfTypes() const
    {
        return __builtin_popcount(value);
    }

    std::string toString() const
    {
        std::string ret;
        if (value == 0) ret += "Unknown ";
        if (value & (1 << 0)) ret += "OBJECT ";
        if (value & (1 << 1)) ret += "STRING ";
        if (value & (1 << 2)) ret += "ENUM ";
        if (value & (1 << 3)) ret += "INTEGER ";
        if (value & (1 << 4)) ret += "NUMBER ";
        if (value & (1 << 5)) ret += "BOOL ";
        if (value & (1 << 6)) ret += "ARRAY ";
        if (value & (1 << 7)) ret += "NULL ";
        return ret;
    }

private:
    JSON_TYPES value = UNKNOWN;
};

class JSTNode
{
public:
    explicit JSTNode(JSTNode *p) : parent(p) {}
    JsonType type;
    std::string name;
    bool hasEnum = false;

    JSTNode *parent = nullptr;
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
    void prunePlaceholders();
    JSTNode *getPlaceHolder(const std::string &name);
    void handleType(ASTNode *node, JSTNode *jNode);

    std::vector<JSTNode *> placeholders;
    ASTNode *defs = nullptr;
};

#endif