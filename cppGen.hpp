#ifndef CPP_GENERATOR_HPP
#define CPP_GENERATOR_HPP

#include <algorithm> // std::transform
#include <cctype>
#include <cstdint>
#include <iostream>
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
        STRING,
        ENUM,
        INTEGER,
        NUMBER
    };

    CppType() = default;
    CppType(CPP_TYPES aOperation) : value(aOperation) {}

    CppType(const std::string &s)
    {
        if (s == "string") { value = CppType::STRING; }
        else if (s == "integer") { value = CppType::INTEGER; }
        else if (s == "enum") { value = CppType::ENUM; }
        else if (s == "number") { value = CppType::NUMBER; }
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
        case ENUM:
            return "enum";
        case INTEGER:
            return "int";
        case NUMBER:
            return "double";
        }
        return ""; // unreachable
    }

private:
    CPP_TYPES value = UNKNOWN;
};

class generator
{
public:
    generator();
    generator(ASTNode *node);

    void generateCpp();
    static void generateCpp(ASTNode *node);

private:
    ASTNode *node;
    std::vector<std::string> enumsStrings;
    std::vector<std::string> structStrings;
};
}

#endif