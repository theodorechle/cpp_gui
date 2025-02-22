#include "style_deserialization_tests/style_deserialization_tests.hpp"
#include "style_tests_lexer_and_parser/style_tests_lexer_and_parser.hpp"

int main() {
    test::Tests tests = test::Tests();
    tests.start();
    styleTestsLexerAndParser::testsLexerAndParser(&tests);
    styleDeserializationTests::testsDeserialization(&tests);
    tests.stop();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
