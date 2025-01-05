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
    StyleTestsLexerAndParser();
    ~StyleTestsLexerAndParser();

    void testLexer(bool equal, const std::string &expr, const Node *expected, const std::string &testName);
    void testParser(bool equal, Node *expr, const Node *expected, const std::string &testName);
    void testLexerAndParser(bool equal, const std::string &expr, const Node *expected, const std::string &testName);

    template <typename T>
    void testLexerAndParserException(const std::string &expression, const std::string &testName);
};

#endif // STYLE_TESTS_LEXER_AND_PARSER