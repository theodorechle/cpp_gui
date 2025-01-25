#include "style_tests/style_tests.hpp"
#include "style_tests_lexer_and_parser/style_tests_lexer_and_parser.hpp"

int main() {
    int nbErrors;
    test::Tests *tests;
    tests = new test::StyleTestsLexerAndParser();
    tests->runTests();
    nbErrors = tests->getNbErrors();

    delete tests;

    tests = new test::StyleTests();
    tests->runTests();
    nbErrors += tests->getNbErrors();

    delete tests;

    return nbErrors;
}
