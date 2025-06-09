#include <cstring>
#include <fstream>  // std::ofstream
#include <iostream> // std::iostream
#include <map>      // std::map
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

    fs << "#include <cstdint>\n";
    fs << "#include <string>\n";
    fs << "#include <vector>\n";
    fs << '\n';

    for (const auto &str : structStrings)
    {
        fs << str << '\n';
    }
}

void generator::generateStruct(JSTNode &node)
{
    auto search = map.find(node.name);
    if (search != map.end())
    {
        // Check if identical to found node
        if (sameNode(search->second, node) == true) { return; }

        const std::string grandParent = node.parent->parent->name;
        node.name = node.parent->name + '_' + grandParent;
        node.parent->name = node.parent->name + '_' + grandParent;
    }
    else { map.insert({node.name, node}); }

    std::string structStr;

    structStr += "struct " + underscoreToCamelCase(node.name) + " {\n";
    for (auto &child : node.children)
    {
        if (child.type == JsonType::OBJECT)
        {
            generateStruct(child);
            structStr += "\t" + underscoreToCamelCase(child.name) + ' ' + child.name + ";\n";
            // generate Struct for this
        }
        else if (child.type == JsonType::ARRAY)
        {
            generateStruct(child.children[0]); // Generate struct first because it may change child->name, if duplicates are found
            structStr += "\t" + toString(child) + '<' + underscoreToCamelCase(child.name) + "> " + child.name + ";\n";
            // generate Struct for this
        }
        else
        {
            structStr += "\t" + toString(child) + ' ' + child.name + ";\n";
        }
    }
    structStr += "};\n";
    structStrings.push_back(structStr);
}

std::string cpp::generator::handleInts(const JSTNode &node)
{
    if (node.type != JsonType::INTEGER) { return "ERROR"; }

    if (node.minimum == std::numeric_limits<int64_t>::min() &&
        node.maximum == std::numeric_limits<int64_t>::max()) { return "int64_t"; } // our default

    if (node.minimum >= std::numeric_limits<uint8_t>::min() &&
        node.maximum <= std::numeric_limits<uint8_t>::max()) { return "uint8_t"; }

    if (node.minimum >= std::numeric_limits<uint16_t>::min() &&
        node.maximum <= std::numeric_limits<uint16_t>::max()) { return "uint16_t"; }

    if (node.minimum >= std::numeric_limits<uint32_t>::min() &&
        node.maximum <= std::numeric_limits<uint32_t>::max()) { return "uint32_t"; }

    if (node.minimum >= std::numeric_limits<uint64_t>::min() &&
        node.maximum <= std::numeric_limits<uint64_t>::max()) { return "uint64_t"; }

    if (node.minimum >= std::numeric_limits<int8_t>::min() &&
        node.maximum <= std::numeric_limits<int8_t>::max()) { return "int8_t"; }

    if (node.minimum >= std::numeric_limits<int16_t>::min() &&
        node.maximum <= std::numeric_limits<int16_t>::max()) { return "int16_t"; }

    if (node.minimum >= std::numeric_limits<int32_t>::min() &&
        node.maximum <= std::numeric_limits<int32_t>::max()) { return "int32_t"; }

    if (node.minimum >= std::numeric_limits<int64_t>::min() &&
        node.maximum <= std::numeric_limits<int64_t>::max()) { return "int64_t"; }

    return "ERROR";
}

std::string generator::toString(const JSTNode &node)
{
    switch (node.type)
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
        return handleInts(node);
    case JsonType::NUMBER:
        return "double";
    case JsonType::BOOL:
        return "bool";
    case JsonType::ARRAY:
        return "std::vector";
    }
    return ""; // unreachable
}