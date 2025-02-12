#include "style_tests.hpp"

namespace test {

    StyleTests::StyleTests() : Tests{"Tests style"}, settings{new style::Settings()} { settings->debug = true; }
    StyleTests::~StyleTests() { delete settings; }

    Result StyleTests::checkStyleComponentDataList(const style::StyleComponentDataList *testedData, const style::StyleComponentDataList *expectedData) {
        if (testedData == nullptr && expectedData == nullptr) return Result::OK;
        if ((testedData == nullptr && expectedData != nullptr) || (testedData != nullptr && expectedData == nullptr)
            || (testedData->size() != expectedData->size())) {
            std::cerr << "One of the data list is null or the lists doesn't have the same size\n";
            return Result::KO;
        }
        style::StyleComponentDataList::const_iterator testedDataListIt = testedData->cbegin();
        style::StyleComponentDataList::const_iterator expectedDataListIt = expectedData->cbegin();
        while (testedDataListIt != testedData->cend()) {
            if (testedDataListIt->second != expectedDataListIt->second) {
                std::cerr << styleRelationToString(testedDataListIt->second) << " instead of relation "
                          << styleRelationToString(expectedDataListIt->second) << "\n";
                return Result::KO;
            }
            if (testedDataListIt->first.first != expectedDataListIt->first.first) {
                std::cerr << testedDataListIt->first.first << " instead of name " << expectedDataListIt->first.first << "\n";
                return Result::KO;
            }
            if (testedDataListIt->first.second != expectedDataListIt->first.second) {
                std::cerr << styleComponentTypeToString(testedDataListIt->first.second) << " instead of type "
                          << styleComponentTypeToString(expectedDataListIt->first.second) << "\n";
                return Result::KO;
            }
            testedDataListIt++;
            expectedDataListIt++;
        }
        return Result::OK;
    }

    Result StyleTests::checkStyleValue(style::StyleValue *testedValue, style::StyleValue *expectedValue) {
        Result result;

        if (testedValue == nullptr && expectedValue == nullptr) return Result::OK;
        if ((testedValue == nullptr && expectedValue != nullptr) || (testedValue != nullptr && expectedValue == nullptr)) {
            std::cerr << "One of the rule is null\n";
            return Result::KO;
        }
        if (testedValue->getValue() != expectedValue->getValue()) {
            std::cerr << "The name is different (have '" << testedValue->getValue() << "', expected '" << expectedValue->getValue() << "')\n";
            return Result::KO;
        }
        if (testedValue->getType() != expectedValue->getType()) {
            std::cerr << "The type is different (have '" << styleValueTypeToString(testedValue->getType()) << "', expected '"
                      << styleValueTypeToString(expectedValue->getType()) << "')\n";
            return Result::KO;
        }

        result = checkStyleValue(testedValue->getChild(), expectedValue->getChild());
        if (result != Result::OK) return result;

        result = checkStyleValue(testedValue->getNext(), expectedValue->getNext());
        if (result != Result::OK) return result;
        return Result::OK;
    }

    Result StyleTests::checkStyleRule(const style::StyleRule *testedRule, const style::StyleRule *expectedRule) {
        if (testedRule == nullptr && expectedRule == nullptr) return Result::OK;
        if ((testedRule == nullptr && expectedRule != nullptr) || (testedRule != nullptr && expectedRule == nullptr)) {
            std::cerr << "One of the rule is null\n";
            return Result::KO;
        }
        if (testedRule->fileNumber != expectedRule->fileNumber) {
            std::cerr << "The file number is different (have '" << testedRule->fileNumber << "', expected '" << expectedRule->fileNumber << "')\n";
            return Result::KO;
        }
        if (testedRule->ruleNumber != expectedRule->ruleNumber) {
            std::cerr << "The rule number is different (have '" << testedRule->ruleNumber << "', expected '" << expectedRule->ruleNumber << "')\n";
            return Result::KO;
        }
        if (testedRule->specificity != expectedRule->specificity) {
            std::cerr << "The specificity is different (have '" << testedRule->specificity << "', expected '" << expectedRule->specificity << "')\n";
            return Result::KO;
        }
        if (testedRule->enabled != expectedRule->enabled) {
            std::cerr << "The enabled status is different (have '" << testedRule->enabled << "', expected '" << expectedRule->enabled << "')\n";
            return Result::KO;
        }
        return checkStyleValue(testedRule->value, expectedRule->value);
    }

    Result StyleTests::checkStyleMap(const style::StyleValuesMap *testedStyleMap, const style::StyleValuesMap *expectedStyleMap) {
        style::StyleValuesMap::const_iterator ruleIt;
        Result styleRuleCheckResult;
        if (testedStyleMap == nullptr && expectedStyleMap == nullptr) return Result::OK;
        if ((testedStyleMap == nullptr && expectedStyleMap != nullptr) || (testedStyleMap != nullptr && expectedStyleMap == nullptr)
            || (testedStyleMap->size() != expectedStyleMap->size())) {
            std::cerr << "One of the style maps list is null or the maps doesn't have the same size\n";
            return Result::KO;
        }

        for (const std::pair<std::string, style::StyleRule> rule : *expectedStyleMap) {
            ruleIt = testedStyleMap->find(rule.first);
            if (ruleIt == testedStyleMap->cend()) {
                std::cerr << "Rule " << rule.first << " can't be found in the tested map\n";
                return Result::KO;
            }
            styleRuleCheckResult = checkStyleRule(&(ruleIt->second), &(rule.second));
            if (styleRuleCheckResult != Result::OK) return styleRuleCheckResult;
        }
        return Result::OK;
    }

    Result StyleTests::checkStyleBlocks(const std::list<style::StyleBlock *> *testedStyleBlocks, const std::list<style::StyleBlock *> *expectedStyleBlocks) {
        Result result;
        std::list<style::StyleBlock *>::const_iterator testedStyleBlocksIt;
        std::list<style::StyleBlock *>::const_iterator expectedStyleBlocksIt;

        if (testedStyleBlocks == nullptr || expectedStyleBlocks == nullptr) {
            std::cerr << "One of the two style blocks is nullptr\n";
            return Result::KO;
        }

        else if (testedStyleBlocks == nullptr || testedStyleBlocks->size() != expectedStyleBlocks->size()) {
            std::cerr << testedStyleBlocks->size() << " styleBlocks instead of " << expectedStyleBlocks->size() << " expected\n";
            return Result::KO;
        }
        else {
            testedStyleBlocksIt = testedStyleBlocks->cbegin();
            expectedStyleBlocksIt = expectedStyleBlocks->cbegin();
            while (testedStyleBlocksIt != testedStyleBlocks->cend()) {
                result = checkStyleComponentDataList((*testedStyleBlocksIt)->getComponentsList(), (*expectedStyleBlocksIt)->getComponentsList());
                if (result != Result::OK) return result;
                result = checkStyleMap((*testedStyleBlocksIt)->getStyleMap(), (*expectedStyleBlocksIt)->getStyleMap());
                if (result != Result::OK) return result;
                testedStyleBlocksIt++;
                expectedStyleBlocksIt++;
            }
        }
        return Result::OK;
    }

    void StyleTests::testDeserializationFromFile(const std::string &fileName, const std::string &testName,
                                                 const std::list<style::StyleBlock *> *expectedStyleBlocks) {
        int fileNumber = 0;
        int ruleNumber;
        std::list<style::StyleBlock *> *styleBlocks;
        Result result;
        startTest("DESERIALIZATION FROM FILE: " + testName);
        std::cout << "Tested style file:\n" << fileName << "\n";
        styleBlocks = style::StyleDeserializer::deserializeFromFile(fileName, fileNumber, &ruleNumber, true);
        result = checkStyleBlocks(styleBlocks, expectedStyleBlocks);

        for (style::StyleBlock *component : *styleBlocks) {
            delete component;
        }
        delete styleBlocks;

        endTest(result);
    }

    void StyleTests::testDeserialization(const std::string &style, const std::string &testName, const std::list<style::StyleBlock *> *expectedStyleBlocks) {
        int fileNumber = 0;
        int ruleNumber;
        std::list<style::StyleBlock *> *styleBlocks;
        Result result;
        startTest("DESERIALIZATION: " + testName);
        std::cout << "Tested style:\n" << style << "\n";
        styleBlocks = style::StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, settings->debug);
        result = checkStyleBlocks(styleBlocks, expectedStyleBlocks);

        for (style::StyleBlock *component : *styleBlocks) {
            delete component;
        }
        delete styleBlocks;

        endTest(result);
    }

    template <typename T> void StyleTests::testDeserializationError(const std::string &style, const std::string &testName) {
        int fileNumber = 0;
        int ruleNumber;
        Result result;
        std::list<style::StyleBlock *> *styleBlocks;
        startTest("DESERIALIZATION ERROR: " + testName);
        std::cout << "Tested style:\n" << style << "\n";
        try {
            styleBlocks = style::StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, settings->debug);
            result = Result::KO;

            for (style::StyleBlock *component : *styleBlocks) {
                delete component;
            }
            delete styleBlocks;
        }
        catch (std::exception &exception) {
            if (dynamic_cast<T *>(&exception)) result = Result::OK;
            else {
                std::cerr << "Throwed invalid exception '" << exception.what() << "\n";
                result = Result::KO;
            }
        }
        catch (...) {
            std::cerr << "Throwed invalid exception who were not a subclass of std::exception\n";
            result = Result::KO;
        }
        endTest(result);
    }

    void StyleTests::testDeserializationNoError(const std::string &style, const std::string &testName) {
        int fileNumber = 0;
        int ruleNumber;
        Result result;
        std::list<style::StyleBlock *> *styleBlocks;
        startTest("DESERIALIZATION NO ERROR: " + testName);
        std::cout << "Tested style:\n" << style << "\n";
        try {
            styleBlocks = style::StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, settings->debug);
            result = Result::OK;

            for (style::StyleBlock *component : *styleBlocks) {
                delete component;
            }
            delete styleBlocks;
        }
        catch (std::exception &exception) {
            std::cerr << "Throwed invalid exception '" << exception.what() << "\n";
            result = Result::KO;
        }
        catch (...) {
            std::cerr << "Throwed invalid exception who were not a subclass of std::exception\n";
            result = Result::KO;
        }
        endTest(result);
    }

    void StyleTests::tests() {
        style::StyleComponentDataList expectedData = style::StyleComponentDataList();
        style::StyleValuesMap expectedStyleMap = style::StyleValuesMap();
        style::StyleValue *styleValue;
        style::StyleBlock *styleBlock;
        std::list<style::StyleBlock *> expectedStyleBlocks;

        expectedData.push_back(std::pair(std::pair("container", style::StyleComponentType::Class), style::StyleRelation::AnyParent));
        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::StyleComponentType::Identifier), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::StyleValueType::Hex);
        expectedStyleMap["text-color"] = style::StyleRule{styleValue, true, 111, 0, 0};
        styleBlock = new style::StyleBlock(&expectedData, &expectedStyleMap);
        expectedStyleBlocks = {styleBlock};
        testDeserialization(".container      label#red{text-color : #ff0000;}", "deserializing a single rule", &expectedStyleBlocks);
        delete styleBlock;
        expectedStyleMap.clear();
        expectedData.clear();

        expectedData.push_back(std::pair(std::pair("container", style::StyleComponentType::Class), style::StyleRelation::DirectParent));
        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::StyleComponentType::Identifier), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::StyleValueType::Hex);
        expectedStyleMap["text-color"] = style::StyleRule{styleValue, true, 111, 0, 0};
        styleBlock = new style::StyleBlock(&expectedData, &expectedStyleMap);
        expectedStyleBlocks = {styleBlock};
        testDeserialization(".container > label#red{text-color : #ff0000;}", "direct parent", &expectedStyleBlocks);
        delete styleBlock;
        expectedStyleMap.clear();
        expectedData.clear();

        expectedData.push_back(std::pair(std::pair("container", style::StyleComponentType::Class), style::StyleRelation::DirectParent));
        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", style::StyleComponentType::Identifier), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("ff0000", style::StyleValueType::Hex);
        expectedStyleMap["text-color"] = style::StyleRule{styleValue, true, 111, 0, 0};
        styleBlock = new style::StyleBlock(&expectedData, &expectedStyleMap);
        expectedStyleBlocks = {styleBlock};
        testDeserialization(".container>label#red{text-color : #ff0000;}", "direct parent without spaces", &expectedStyleBlocks);
        delete styleBlock;
        expectedStyleMap.clear();
        expectedData.clear();

        expectedData.push_back(std::pair(std::pair("label", style::StyleComponentType::ElementName), style::StyleRelation::SameElement));
        styleValue = new style::StyleValue("", style::StyleValueType::PixelUnit);
        style::StyleValue *styleValue2 = new style::StyleValue("100", style::StyleValueType::Int);
        styleValue->setChild(styleValue2);
        expectedStyleMap["padding"] = style::StyleRule{styleValue, true, 1, 0, 0};
        styleBlock = new style::StyleBlock(&expectedData, &expectedStyleMap);
        expectedStyleBlocks = {styleBlock};
        testDeserialization("label {padding:100px;}", "style name and value sticked to the assignment colon", &expectedStyleBlocks);
        delete styleBlock;
        expectedStyleMap.clear();
        expectedData.clear();

        testDeserializationError<style::MalformedExpression>(".container      label#red{text-color : #ff0000}", "missing semi-colon after assignment");
        testDeserializationError<style::MalformedExpression>(".container>label#red{text-color;}", "missing style value");
        testDeserializationError<style::MalformedExpression>(".container>label#red{: value}", "missing style name");
        testDeserializationError<style::MalformedExpression>(".container>label#red{:}", "missing style name and value");
        testDeserializationError<style::MalformedExpression>("{text-color: #ffffff;}", "missing block declaration");
        testDeserializationError<style::MissingToken>(">label#red{text-color: #ffffff;}", "missing block declaration component before relation '>'");
    }

} // namespace test
