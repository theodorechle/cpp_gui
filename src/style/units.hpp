#ifndef UNITS_HPP
#define UNITS_HPP

#include <unordered_map>
#include <string>

#include "node.hpp"

int calculateParentPercentage(void*);
int calculateParentWidthPercentage(void*);
int calculateParentHeightPercentage(void*);
int calculateWindowWidthPercentage(void*);
int calculateWindowHeightPercentage(void*);
int calculatePixels(void*);

typedef int (*function)(void*);

const std::unordered_map<std::string, function> UNITS {
    {"%", calculateParentPercentage},
    {"pw", calculateParentWidthPercentage},
    {"ph", calculateParentHeightPercentage},
    {"ww", calculateWindowWidthPercentage},
    {"wh", calculateWindowHeightPercentage},
    {"px", calculatePixels},
};

#endif // UNITS_HPP