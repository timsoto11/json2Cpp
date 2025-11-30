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
    JstGenerator gen;
    auto root = gen.generateJST(node);

    // std::cout << "Generated Tree:\n";
    // JstGenerator::print_jst(root.get(), 0);
    // std::cout << "\n\n";

    generateStruct(root.get()); // populates structStrings

    // gen.print_jst(root.get(), 0);

    std::ofstream fs("settings-generated.hpp");

    // TODO: Only include when needed
    fs << "#include <cstdint>\n";
    fs << "#include <string>\n";
    fs << "#include <vector>\n";
    fs << "#include <variant>\n";
    fs << '\n';

    for (const auto &str : variantStrings)
    {
        fs << str << '\n';
    }
    if (!variantStrings.empty()) { fs << '\n'; }

    for (const auto &str : structStrings)
    {
        fs << str << '\n';
    }
}

void generator::generateStruct(JSTNode *node)
{
    auto search = map.find(node->name);
    if (search != map.end())
    {
        // Check if identical to found node
        if (sameNode(search->second, node) == true) { return; }

        // const std::string grandParent = node->parent->parent->name;
        // node->name = node->parent->name + '_' + grandParent;
        // node->parent->name = node->parent->name + '_' + grandParent;
    }
    else {
        map.insert({node->name, node});
    }

    std::string structStr;

    structStr += "struct " + underscoreToCamelCase(node->name) + "\n{\n";
    for (auto &child : node->children)
    {
        if (child->type.numberOfTypes() > 1)
        {
            handleVariants(structStr, child.get());
            continue;
        }

        // At this point there is only one type
        if (child->type == JsonType::ENUM)
        {
            handleEnums(structStr, child.get());
            continue;
        }
        else if (child->type == JsonType::OBJECT)
        {
            generateStruct(child.get());
            structStr += "\t" + underscoreToCamelCase(child->name) + ' ' + child->name + ";\n";
        }
        else if (child->type == JsonType::ARRAY)
        {
            // If the type is an object we need to generate a struct, otherwise this is a vector of something already defined (int, std::string...)
            if (child->children[0]->type == JsonType::OBJECT)
            {
                generateStruct(child->children[0].get()); // Generate struct first because it may change child->name, if duplicates are found
                structStr += "\t" + toString(child->type) + '<' + underscoreToCamelCase(child->children[0]->name) + "> " + child->name + ";\n";
            }
            else
            {
                structStr += "\t" + toString(child->type) + '<' + printVectorObject(*child) + "> " + child->name + ";\n";
            }
        }
        else
        {
            structStr += "\t" + toString(child->type) + ' ' + child->name + ";\n";
        }
    }
    structStr += "};\n";
    structStrings.push_back(structStr);
}

std::string generator::printVectorObject(const JSTNode &node)
{
    std::string ret;
    std::string closingAndleBrackets;
    auto child = node.children[0].get();
    auto childType = child->type;

    while (childType == JsonType::ARRAY)
    {
        ret += "std::vector<";
        closingAndleBrackets += '>';

        child = child->children[0].get();
        childType = child->type;
    }

    ret += toString(child->type);

    ret += closingAndleBrackets;

    return ret;
}

void cpp::generator::handleVariants(std::string &structStr, JSTNode *node)
{
    std::string varStr = "using " + underscoreToCamelCase(node->name) + " = std::variant<";
    int numberOfTypes = node->type.numberOfTypes();
    int numberOfTypePrinted = 0;

    for (int i = 0; i < 8; ++i)
    {
        uint8_t typeValue = (1u << i);
        if (node->type & typeValue)
        {
            varStr += toString(JsonType(static_cast<JsonType::JSON_TYPES>(typeValue)),
                               node->minimum,
                               node->maximum);
            numberOfTypePrinted++;
            if (numberOfTypePrinted < numberOfTypes) { varStr += ", "; }
        }
    }

    varStr += ">;";
    variantStrings.push_back(varStr);
    structStr += "\t" + underscoreToCamelCase(node->name) + ' ' + node->name + ";\n";
}

void cpp::generator::handleEnums(std::string &structStr, JSTNode *node)
{
    auto type = node->children.at(0)->type;
    for (auto iter = node->children.begin(); iter != node->children.end(); iter++)
    {
        if ((*iter)->type != type)
        {
            // TODO: We still need to handle enum with multiple types
            return;
        }
    }
    structStr += "\t" + toString(node->children.at(0)->type) + ' ' + node->name + "; // Need to create type for enum.\n";
}

std::string cpp::generator::handleInts(const int64_t &min, const int64_t &max)
{
    if (min == std::numeric_limits<int64_t>::min() &&
        max == std::numeric_limits<int64_t>::max()) { return "int64_t"; } // our default

    if (min >= std::numeric_limits<uint8_t>::min() &&
        max <= std::numeric_limits<uint8_t>::max()) { return "uint8_t"; }

    if (min >= std::numeric_limits<uint16_t>::min() &&
        max <= std::numeric_limits<uint16_t>::max()) { return "uint16_t"; }

    if (min >= std::numeric_limits<uint32_t>::min() &&
        max <= std::numeric_limits<uint32_t>::max()) { return "uint32_t"; }

    if (min >= std::numeric_limits<uint64_t>::min() &&
        max <= std::numeric_limits<uint64_t>::max()) { return "uint64_t"; }

    if (min >= std::numeric_limits<int8_t>::min() &&
        max <= std::numeric_limits<int8_t>::max()) { return "int8_t"; }

    if (min >= std::numeric_limits<int16_t>::min() &&
        max <= std::numeric_limits<int16_t>::max()) { return "int16_t"; }

    if (min >= std::numeric_limits<int32_t>::min() &&
        max <= std::numeric_limits<int32_t>::max()) { return "int32_t"; }

    if (min >= std::numeric_limits<int64_t>::min() &&
        max <= std::numeric_limits<int64_t>::max()) { return "int64_t"; }

    return "// ERROR";
}

std::string cpp::generator::toString(const JsonType &type, const int64_t &min, const int64_t &max)
{
    switch (type)
    {
    case JsonType::STRING:
        return "std::string";
    case JsonType::OBJECT:
        return "struct";
    case JsonType::ENUM:
        return "enum";
    case JsonType::INTEGER:
        return handleInts(min, max);
    case JsonType::NUMBER:
        return "double";
    case JsonType::BOOL:
        return "bool";
    case JsonType::ARRAY:
        return "std::vector";
    }
    return "// Unknown";
}