#ifndef STYLE_TESTS_HPP
#define STYLE_TESTS_HPP

#include "../../src/style/style_deserializer.hpp"
#include "../tests.hpp"

class StyleTests : public Tests {
    void tests() override;
    Settings *settings;

public:
    StyleTests();
    ~StyleTests();

    Result checkStyleComponentDataList(const StyleComponentDataList *testedData, const StyleComponentDataList *expectedData);
    Result checkStyleValue(StyleValue *testedValue, StyleValue *expectedValue);
    Result checkStyleRule(const StyleRule *testedRule, const StyleRule *expectedRule);
    Result checkStyleMap(const StyleValuesMap *testedStyleMap, const StyleValuesMap *expectedStyleMap);
    Result checkRuleNumberAndStyleBlocks(int testedRuleNumber, int expectedRuleNumber, const std::list<StyleBlock *> *testedStyleBlocks,
                                         const std::list<StyleBlock *> *expectedStyleBlocks);
    void testDeserializationFromFile(const std::string &fileName, const std::string &testName, const std::list<StyleBlock *> *expectedStyleBlocks);
    void testDeserialization(const std::string &style, const std::string &testName, const std::list<StyleBlock *> *expectedStyleBlocks);

    /**
     * This method will catch std::exception subclass exceptions but will not check if they are similar to T.
     */
    template <typename T>
    void testDeserializationError(const std::string &style, const std::string &testName);
    void testDeserializationNoError(const std::string &style, const std::string &testName);
};

#endif // STYLE_TESTS_HPP
