#include <string>

#include "JST.hpp"
#include "generationUtils.hpp"

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

bool sameNode(const JSTNode &node1, const JSTNode &node2)
{
    if (node1.name != node2.name) { return false; }
    if (node1.type != node2.type) { return false; }
    if (node1.minimum != node2.minimum) { return false; }
    if (node1.maximum != node2.maximum) { return false; }

    if (node1.children.size() != node2.children.size()) { return false; }

    for (int i = 0; i < node1.children.size(); i++)
    {
        const auto &child1 = *node1.children[i];
        const auto &child2 = *node2.children[i];

        const bool isSame = sameNode(child1, child2);
        if (isSame == false) { return false; }
    }

    return true;
}
