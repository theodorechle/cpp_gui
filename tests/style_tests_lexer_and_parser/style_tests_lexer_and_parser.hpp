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
#include "../../cpp_tests/src/tests.hpp"

namespace test {

    class StyleTestsLexerAndParser : public Tests {
        style::Settings *settings;
        const std::string TESTS_FILES_DIR = "tests/style_tests_lexer_and_parser/tests-files";
        void tests() override;

    public:
        StyleTestsLexerAndParser();
        ~StyleTestsLexerAndParser();

        void testLexer(bool equal, const std::string &expr, const style::Node *expected, const std::string &testName);
        void testParser(bool equal, style::Node *expr, const style::Node *expected, const std::string &testName);
        void testLexerAndParser(bool equal, const std::string &expr, const style::Node *expected, const std::string &testName);

        template <typename T> void testLexerAndParserException(const std::string &expression, const std::string &testName);
    };

} // namespace test

#endif // STYLE_TESTS_LEXER_AND_PARSER