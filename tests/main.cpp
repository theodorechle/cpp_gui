#include "../cpp_tests/src/tests.hpp"
#include "style_node_tests/style_node_tests.hpp"
#include "apply_style_tests/apply_style_tests.hpp"

int main() {
    test::Tests tests = test::Tests(-1);
    styleNodesTests::testsStyleNodes(&tests);
    applyStyleTests::applyStyleTests(&tests);
    tests.runTests();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
