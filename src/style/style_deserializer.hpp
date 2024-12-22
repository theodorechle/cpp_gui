#ifndef STYLE_DESERIALIZER_HPP
#define STYLE_DESERIALIZER_HPP

#include "lexer.hpp"
#include "parser.hpp"
#include "node_to_style_component_list.hpp"
#include "style_component.hpp"
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

class StyleDeserializer {
public:
    static std::list<StyleComponent *> *deserializeFromFile(const std::string &fileName, int fileNumber, int *ruleNumber);
    static std::list<StyleComponent *> *deserialize(const std::string &style, int fileNumber, int *ruleNumber);
};

#endif // STYLE_DESERIALIZER_HPP
