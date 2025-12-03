#ifndef element_style_TESTS_HPP
#define element_style_TESTS_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../cpp_style/src/style_component.hpp"
#include "../../src/element_style/element_style.hpp"
#include "../test_config.hpp"

namespace styleNodesTests {
    void displayValue(style::StyleValue *value);
    bool testValue(bool equal, style::StyleValue *value, style::StyleValue *expected, bool disaplyDebug = false);
    bool testRule(bool equal, const style::StyleRule &rule, const style::StyleRule &expected);

    void testsStyleNodes(test::Tests *tests);
} // namespace styleNodesTests

#endif // element_style_TESTS_HPP