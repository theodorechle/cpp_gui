#ifndef STYLE_TESTS_HPP
#define STYLE_TESTS_HPP

#include "../../src/style/style_deserializer.hpp"
#include "../tests.hpp"

class StyleTests : public Tests {

    void tests() override;

public:
    StyleTests() : Tests{"Tests style"} {}

    Result testStyleComponentDataList(const StyleComponentDataList *testedData, const StyleComponentDataList *expectedData);
    Result testStyleValue(StyleValue *testedValue, StyleValue *expectedValue);
    Result testStyleRule(const StyleRule *testedRule, const StyleRule *expectedRule);
    Result testStyleMap(const StyleValuesMap *testedStyleMap, const StyleValuesMap *expectedStyleMap);
};

#endif // STYLE_TESTS_HPP
