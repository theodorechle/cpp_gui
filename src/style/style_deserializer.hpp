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
    std::list<StyleComponent *> *deserialize(const std::string &fileName);
};

#endif // STYLE_DESERIALIZER_HPP
