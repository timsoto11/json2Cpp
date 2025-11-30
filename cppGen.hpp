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
    std::vector<std::string> variantStrings;
    std::vector<std::string> enumsStrings;
    std::vector<std::string> structStrings;
    std::map<std::string, const JSTNode *> map;

    void generateStruct(JSTNode *node);
    void handleVariants(std::string &structStr, JSTNode *node);
    void handleEnums(std::string &structStr, JSTNode *node);
    std::string printVector(const JSTNode &node);

    static std::string handleInts(const JSTNode &node, const uint32_t index);
    static std::string toString(const JSTNode &node, const uint32_t index = 0);
};
}

#endif
