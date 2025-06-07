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

    static std::string toString(const JsonType &value);
};
}

#endif
