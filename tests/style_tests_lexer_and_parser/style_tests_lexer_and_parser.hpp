#ifndef STYLE_TESTS_LEXER_AND_PARSER
#define STYLE_TESTS_LEXER_AND_PARSER

#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "../../src/style/lexer.hpp"
#include "../../src/style/node.hpp"
#include "../../src/style/parser.hpp"
#include "../tests.hpp"

class StyleTestsLexerAndParser : public Tests {
    Settings *settings;
    const std::string TESTS_FILES_DIR = "tests/style_tests_lexer_and_parser/tests-files";
    void tests() override;

public:
    StyleTestsLexerAndParser(Settings *settings = new Settings()) : Tests{"Tests style lexer and parser"}, settings{settings} {};

    void lexer(bool equal, const std::string &expr, const Node *expected);
    void parser(bool equal, Node *expr, const Node *expected);
    void lexerAndParser(bool equal, const std::string &expr, const Node *expected);

    // TODO: mettre en réference const
    void invalidExpression(std::string expression);
    void unknownToken(std::string expression);
    void missingToken(std::string expression);
    void unknownValue(std::string epxression);
};

#endif // STYLE_TESTS_LEXER_AND_PARSER