#ifndef _GENERATION_UTILS_HPP_
#define _GENERATION_UTILS_HPP_

#include <string>

#include "JST.hpp"

std::string underscoreToCamelCase(const std::string &str);
bool sameNode(JSTNode *node1, JSTNode *node2);

#endif