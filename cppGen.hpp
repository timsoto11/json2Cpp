#ifndef CPP_GENERATOR_HPP
#define CPP_GENERATOR_HPP

#include <map> // std::map

#include <string> // std::string

#include "JST.hpp"

namespace cpp {

class generator
{
public:
    generator(ASTNode *node);

private:
    ASTNode *node;
    std::vector<std::string> enumsStrings;
    std::vector<std::string> structStrings;

    void generateStruct(JSTNode &node);

    std::map<std::string, JSTNode> map;

    std::string toString(const JsonType &value) const
    {
        switch (value)
        {
        case JsonType::UNKNOWN:
            return "Unknown";
        case JsonType::STRING:
            return "std::string";
        case JsonType::OBJECT:
            return "struct";
        case JsonType::ENUM:
            return "enum";
        case JsonType::INTEGER:
            return "int";
        case JsonType::NUMBER:
            return "double";
        case JsonType::BOOL:
            return "bool";
        case JsonType::ARRAY:
            return "std::vector";
        }
        return ""; // unreachable
    }
};
}

#endif
