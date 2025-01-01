#ifndef STYLE_DESERIALIZER_HPP
#define STYLE_DESERIALIZER_HPP

#include "lexer.hpp"
#include "node_to_style_component_list.hpp"
#include "parser.hpp"
#include "style_component.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

class StyleDeserializer {
public:
    static std::list<StyleBlock *> *deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber, bool debug = false);
    static std::list<StyleBlock *> *deserialize(const std::string &style, int fileNumber, int *ruleNumber, bool debug = false);
};

#endif // STYLE_DESERIALIZER_HPP
