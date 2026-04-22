#ifndef APPLY_STYLE_TESTS_HPP
#define APPLY_STYLE_TESTS_HPP

#include "../../cpp_tests/src/tests.hpp"
#include "../../src/element_style/style_manager.hpp"
#include "../../src/elements/ui/ui_element.hpp"
#include "../test_config.hpp"

namespace applyStyleTests {
    test::Result testApplyingStyle(const std::string &style, gui::element::AbstractElement *element, std::vector<std::string> expectedRulesNames,
                                   bool expectToFindValues = true);

    void applyStyleTests(test::Tests *tests);
} // namespace applyStyleTests

#endif // APPLY_STYLE_TESTS_HPP
