#include "style_tests/style_tests.hpp"
#include "style_tests_lexer_and_parser/style_tests_lexer_and_parser.hpp"

int main() {
    int nbErrors;
    Tests *tests;
    Settings *settings = new Settings();
    settings->debug = true;
    tests = new StyleTestsLexerAndParser(settings);
    tests->runTests();
    nbErrors = tests->getNbErrors();

    delete tests;
    delete settings;

    tests = new StyleTests();
    tests->runTests();
    nbErrors += tests->getNbErrors();

    delete tests;

    return nbErrors;
}
