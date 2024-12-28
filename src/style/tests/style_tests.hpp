#ifndef STYLE_TESTS
#define STYLE_TESTS

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "../../tests/tests.hpp"
#include "../lexer.hpp"
#include "../node.hpp"
#include "../parser.hpp"

class StyleTests : public Tests {
    Settings *settings;

public:
    StyleTests(Settings *settings = new Settings()) : settings{settings} {};

    void tests() override;

    void lexer(bool equal, const std::string &expr, const Node *expected);
    void parser(bool equal, Node *expr, const Node *expected);
    void lexerAndParser(bool equal, const std::string &expr, const Node *expected);

    void invalidExpression(std::string expression);
    void unknownToken(std::string expression);
    void missingToken(std::string expression);
    void unknownValue(std::string epxression);
};

#endif // STYLE_TESTS