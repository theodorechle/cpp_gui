#include "../cpp_tests/src/tests.hpp"
// #include "element_nodes_tests.hpp"

int main() {
    test::Tests tests = test::Tests();
    tests.start();
    tests.stop();
    tests.displaySummary();
    return !tests.allTestsPassed();
}
