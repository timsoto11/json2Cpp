#include <cstring>
#include <fstream>  // std::ofstream
#include <iostream> // std::iostream
#include <memory>   // unique and make_unique
#include <string>
#include <vector>

#include "JST.hpp"
#include "cppGen.hpp"
#include "generationUtils.hpp"

using namespace cpp;

generator::generator(ASTNode *node)
{
    auto root = JstGenerator::generateJST(node);

    // std::cout << "Generated Tree:\n";
    // JstGenerator::print_jst(root.get(), 0);
    // std::cout << "\n\n";
    generateStruct(*root);

    std::ofstream fs("violationSettings.hpp");

    fs << "#include <string>\n";
    fs << "#include <vector>\n";
    fs << '\n';

    for (const auto &str : structStrings)
    {
        fs << str << '\n';
    }
}

void cpp::generator::generateStruct(JSTNode &node)
{
    std::string structStr;

    structStr += "struct " + underscoreToCamelCase(node.name) + " {\n";
    for (const auto &child : node.children)
    {
        if (child->type == JsonType::OBJECT)
        {
            structStr += "\t" + underscoreToCamelCase(child->name) + ' ' + child->name + ";\n";
            generateStruct(*child);
            // generate Struct for this
        }
        else if (child->type == JsonType::ARRAY)
        {
            structStr += "\t" + toString(child->type) + '<' + underscoreToCamelCase(child->name) + "> " + child->name + ";\n";
            generateStruct(*(child->children[0]));
            // generate Struct for this
        }
        else
        {
            structStr += "\t" + toString(child->type) + ' ' + child->name + ";\n";
        }
    }
    structStr += "};\n";
    structStrings.push_back(structStr);
}

std::string generator::toString(const JsonType &value)
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