#ifndef _GENERATION_UTILS_HPP_
#define _GENERATION_UTILS_HPP_

#include <string>

#include "JST.hpp"

std::string underscoreToCamelCase(const std::string &str);
bool sameNode(const JSTNode &node1, const JSTNode &node2);

#endif