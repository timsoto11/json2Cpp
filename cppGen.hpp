#ifndef CPP_GENERATOR_HPP
#define CPP_GENERATOR_HPP

#include <algorithm> // std::transform
#include <cctype>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "ast.h"

namespace cpp {

class CppType
{
public:
    enum CPP_TYPES : uint8_t
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

    CppType() = default;
    CppType(CPP_TYPES aOperation) : value(aOperation) {}

    CppType(const std::string &s)
    {
        if (s == "string") { value = CppType::STRING; }
        else if (s == "object") { value = CppType::OBJECT; }
        else if (s == "integer") { value = CppType::INTEGER; }
        else if (s == "enum") { value = CppType::ENUM; }
        else if (s == "number") { value = CppType::NUMBER; }
        else if (s == "boolean") { value = CppType::BOOL; }
        else if (s == "array") { value = CppType::ARRAY; }
        else { value = CppType::UNKNOWN; }
    }

    // Allow switch and comparisons.
    constexpr operator CPP_TYPES() const { return value; }

    // Prevent usage: if(OPERATION_MODE)
    explicit operator bool() const = delete;

    constexpr bool operator==(const CppType a) const { return value == a.value; }
    constexpr bool operator!=(const CppType a) const { return value != a.value; }
    friend std::ostream &operator<<(std::ostream &os, const CppType &a) { return os << a.toString(); }

    constexpr bool operator==(const CPP_TYPES a) const { return value == a; }
    constexpr bool operator!=(const CPP_TYPES a) const { return value != a; }

    std::string toString() const
    {
        switch (value)
        {
        case UNKNOWN:
            return "Unknown";
        case STRING:
            return "std::string";
        case OBJECT:
            return "struct";
        case ENUM:
            return "enum";
        case INTEGER:
            return "int";
        case NUMBER:
            return "double";
        case BOOL:
            return "bool";
        case ARRAY:
            return "std::vector";
        }
        return ""; // unreachable
    }

private:
    CPP_TYPES value = UNKNOWN;
};

class CSTNode
{
public:
    CppType type = CppType::UNKNOWN;
    std::string name;
    bool hasEnum = false;
    CSTNode *parent;
    std::vector<std::unique_ptr<CSTNode>> children; // For arrays/objects
    int64_t minimum = std::numeric_limits<int64_t>::min();
    int64_t maximum = std::numeric_limits<int64_t>::max();
};

class generator
{
public:
    generator(ASTNode *node);
    void generateCST(ASTNode *node, CSTNode &cstNode);
    void print_cst(const CSTNode *const node, int indent);

private:
    ASTNode *node;
    std::vector<std::string> enumsStrings;
    std::vector<std::string> structStrings;

    void generateStruct(CSTNode &node);
};
}

#endif
