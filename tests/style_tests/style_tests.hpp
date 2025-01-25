#ifndef STYLE_TESTS_HPP
#define STYLE_TESTS_HPP

#include "../../src/style/style_deserializer.hpp"
#include "../tests.hpp"

namespace test {

    class StyleTests : public Tests {
        void tests() override;
        style::Settings *settings;

    public:
        StyleTests();
        ~StyleTests();

        Result checkStyleComponentDataList(const style::StyleComponentDataList *testedData, const style::StyleComponentDataList *expectedData);
        Result checkStyleValue(style::StyleValue *testedValue, style::StyleValue *expectedValue);
        Result checkStyleRule(const style::StyleRule *testedRule, const style::StyleRule *expectedRule);
        Result checkStyleMap(const style::StyleValuesMap *testedStyleMap, const style::StyleValuesMap *expectedStyleMap);
        Result checkStyleBlocks(const std::list<style::StyleBlock *> *testedStyleBlocks, const std::list<style::StyleBlock *> *expectedStyleBlocks);
        void testDeserializationFromFile(const std::string &fileName, const std::string &testName,
                                         const std::list<style::StyleBlock *> *expectedStyleBlocks);
        void testDeserialization(const std::string &style, const std::string &testName, const std::list<style::StyleBlock *> *expectedStyleBlocks);

        /**
         * This method will catch std::exception subclass exceptions but will not check if they are similar to T.
         */
        template <typename T> void testDeserializationError(const std::string &style, const std::string &testName);
        void testDeserializationNoError(const std::string &style, const std::string &testName);
    };

} // namespace test

#endif // STYLE_TESTS_HPP
