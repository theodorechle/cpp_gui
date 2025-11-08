#include "../cpp_tests/src/tests.hpp"
// #include "element_nodes_tests.hpp"

int main() {
    test::Tests tests = test::Tests();
    tests.runTests();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
