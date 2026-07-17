#ifndef STYLE_NODE_TESTS_HPP
#define STYLE_NODE_TESTS_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../cpp_style/src/style_component.hpp"

namespace styleNodesTests {
    void displayValue(style::StyleValue *value);
    bool testValue(bool equal, style::StyleValue *value, style::StyleValue *expected, bool displayDebug = false);
    bool testRule(bool equal, const style::StyleRule &rule, const style::StyleRule &expected);

    void testsStyleNodes(test::Tests *tests);
} // namespace styleNodesTests

#endif // STYLE_NODE_TESTS_HPP
