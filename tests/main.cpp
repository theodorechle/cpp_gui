#include "../cpp_tests/src/tests.hpp"
#include "style_node_tests/style_node_tests.hpp"

int main() {
    test::Tests tests = test::Tests();
    styleNodesTests::testsStyleNodes(&tests);
    tests.runTests();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
