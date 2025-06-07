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

bool sameNode(JSTNode *node1, JSTNode *node2)
{
    return false;
}
