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

    void generateStruct(const JSTNode &node);

    std::map<std::string, JSTNode> map;

    static std::string handleInts(const JSTNode &node);
    static std::string toString(const JSTNode &value);
};
}

#endif
