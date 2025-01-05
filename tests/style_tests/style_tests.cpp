#include "style_tests.hpp"

Result StyleTests::checkStyleComponentDataList(const StyleComponentDataList *testedData, const StyleComponentDataList *expectedData) {
    if (testedData == nullptr && expectedData == nullptr) return Result::OK;
    if ((testedData == nullptr && expectedData != nullptr) || (testedData != nullptr && expectedData == nullptr)
        || (testedData->size() != expectedData->size())) {
        std::cerr << "One of the data list is null or the lists doesn't have the same size\n";
        return Result::KO;
    }
    StyleComponentDataList::const_iterator testedDataListIt = testedData->cbegin();
    StyleComponentDataList::const_iterator expectedDataListIt = expectedData->cbegin();
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

Result StyleTests::checkStyleValue(StyleValue *testedValue, StyleValue *expectedValue) {
    Result result;

    if (testedValue == nullptr && expectedValue == nullptr) return Result::OK;
    if ((testedValue == nullptr && expectedValue != nullptr) || (testedValue != nullptr && expectedValue == nullptr)) {
        std::cerr << "One of the rule is null\n";
        return Result::KO;
    }
    if (testedValue->getValue() != expectedValue->getValue()) {
        std::cerr << "The name is different (have " << testedValue->getValue() << ", expected " << expectedValue->getValue() << ")\n";
        return Result::KO;
    }
    if (testedValue->getType() != expectedValue->getType()) {
        std::cerr << "The type is different (have " << styleValueTypeToString(testedValue->getType()) << ", expected "
                  << styleValueTypeToString(expectedValue->getType()) << ")\n";
        return Result::KO;
    }

    result = checkStyleValue(testedValue->getChild(), expectedValue->getChild());
    if (result != Result::OK) return result;

    result = checkStyleValue(testedValue->getNext(), expectedValue->getNext());
    if (result != Result::OK) return result;
    return Result::OK;
}

Result StyleTests::checkStyleRule(const StyleRule *testedRule, const StyleRule *expectedRule) {
    if (testedRule == nullptr && expectedRule == nullptr) return Result::OK;
    if ((testedRule == nullptr && expectedRule != nullptr) || (testedRule != nullptr && expectedRule == nullptr)) {
        std::cerr << "One of the rule is null\n";
        return Result::KO;
    }
    if (testedRule->fileNumber != expectedRule->fileNumber) {
        std::cerr << "The file number is different (have " << testedRule->fileNumber << ", expected " << expectedRule->fileNumber << "\n";
        return Result::KO;
    }
    if (testedRule->ruleNumber != expectedRule->ruleNumber) {
        std::cerr << "The rule number is different (have " << testedRule->ruleNumber << ", expected " << expectedRule->ruleNumber << "\n";
        return Result::KO;
    }
    if (testedRule->priority != expectedRule->priority) {
        std::cerr << "The priority is different (have " << testedRule->priority << ", expected " << expectedRule->priority << "\n";
        return Result::KO;
    }
    if (testedRule->isEnabled != expectedRule->isEnabled) {
        std::cerr << "The enabled status is different (have " << testedRule->isEnabled << ", expected " << expectedRule->isEnabled << "\n";
        return Result::KO;
    }
    return checkStyleValue(testedRule->value, expectedRule->value);
}

Result StyleTests::checkStyleMap(const StyleValuesMap *testedStyleMap, const StyleValuesMap *expectedStyleMap) {
    StyleValuesMap::const_iterator ruleIt;
    Result styleRuleCheckResult;
    if (testedStyleMap == nullptr && expectedStyleMap == nullptr) return Result::OK;
    if ((testedStyleMap == nullptr && expectedStyleMap != nullptr) || (testedStyleMap != nullptr && expectedStyleMap == nullptr)
        || (testedStyleMap->size() != expectedStyleMap->size())) {
        std::cerr << "One of the style maps list is null or the maps doesn't have the same size\n";
        return Result::KO;
    }

    for (const std::pair<std::string, StyleRule> rule : *expectedStyleMap) {
        ruleIt = testedStyleMap->find(rule.first);
        if (ruleIt == testedStyleMap->cend()) {
            std::cerr << "Rule " << rule.first << " can't be found in the tested map\n";
            return Result::KO;
        }
        styleRuleCheckResult = checkStyleRule(&(rule.second), &(ruleIt->second));
        if (styleRuleCheckResult != Result::OK) return styleRuleCheckResult;
    }
    return Result::OK;
}

Result StyleTests::checkRuleNumberAndStyleBlocks(int testedRuleNumber, int expectedRuleNumber, const std::list<StyleBlock *> *testedStyleBlocks,
                                                 const std::list<StyleBlock *> *expectedStyleBlocks) {
    Result result;
    std::list<StyleBlock *>::const_iterator testedStyleBlocksIt;
    std::list<StyleBlock *>::const_iterator expectedStyleBlocksIt;
    if (testedStyleBlocks == nullptr || expectedStyleBlocks == nullptr) {
        std::cerr << "One of the two style blocks is nullptr\n";
        return Result::KO;
    }
    if (testedRuleNumber != expectedRuleNumber) {
        std::cerr << "ruleNumber is " << testedRuleNumber << " instead of " << expectedRuleNumber << "\n";
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
                                             const std::list<StyleBlock *> *expectedStyleBlocks) {
    int fileNumber = 0;
    int ruleNumber;
    std::list<StyleBlock *> *styleBlocks;
    Result result;
    startTest(testName);
    std::cout << "Tested style file:\n" << fileName << "\n";
    styleBlocks = StyleDeserializer::deserializeFromFile(fileName, fileNumber, &ruleNumber, true);
    result = checkRuleNumberAndStyleBlocks(ruleNumber, 1, styleBlocks, expectedStyleBlocks);

    for (StyleBlock *component : *styleBlocks) {
        for (const std::pair<std::string, StyleRule> rule : *(component->getStyleMap())) {
            delete rule.second.value;
        }
        delete component;
    }
    delete styleBlocks;

    endTest(result);
}

void StyleTests::testDeserialization(const std::string &style, const std::string &testName, const std::list<StyleBlock *> *expectedStyleBlocks) {
    int fileNumber = 0;
    int ruleNumber;
    std::list<StyleBlock *> *styleBlocks;
    Result result;
    startTest(testName);
    std::cout << "Tested style:\n" << style << "\n";
    styleBlocks = StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, true);
    result = checkRuleNumberAndStyleBlocks(ruleNumber, 1, styleBlocks, expectedStyleBlocks);

    for (StyleBlock *component : *styleBlocks) {
        for (const std::pair<std::string, StyleRule> rule : *(component->getStyleMap())) {
            delete rule.second.value;
        }
        delete component;
    }
    delete styleBlocks;

    endTest(result);
}

template <typename T> void StyleTests::testDeserializationError(const std::string &style, const std::string &testName) {
    int fileNumber = 0;
    int ruleNumber;
    Result result;
    std::list<StyleBlock *> *styleBlocks;
    startTest(testName);
    std::cout << "Tested style:\n" << style << "\n";
    try {
        styleBlocks = StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, true);
        result = Result::KO;

        for (StyleBlock *component : *styleBlocks) {
            for (const std::pair<std::string, StyleRule> rule : *(component->getStyleMap())) {
                delete rule.second.value;
            }
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

void StyleTests::tests() {
    StyleComponentDataList expectedData = StyleComponentDataList();
    StyleValuesMap expectedStyleMap = StyleValuesMap();
    StyleValue *styleValue;
    StyleBlock *styleBlock;
    std::list<StyleBlock *> expectedStyleBlocks;

    expectedData.push_back(std::pair(std::pair("container", StyleComponentType::Class), StyleRelation::AnyParent));
    expectedData.push_back(std::pair(std::pair("label", StyleComponentType::ElementName), StyleRelation::SameElement));
    expectedData.push_back(std::pair(std::pair("red", StyleComponentType::Identifier), StyleRelation::SameElement));
    styleValue = new StyleValue("ff0000", StyleValueType::Hex);
    expectedStyleMap["text-color"] = StyleRule{styleValue, true, 0, 0, 0};
    styleBlock = new StyleBlock(&expectedData, &expectedStyleMap);
    expectedStyleBlocks = {styleBlock};
    testDeserialization(".container      label#red{text-color : #ff0000;}", "deserializing a single rule", &expectedStyleBlocks);
    delete styleBlock;
    delete styleValue;
    expectedStyleMap.clear();
    expectedData.clear();

    testDeserializationError<MalformedExpression>(".container      label#red{text-color : #ff0000}", "raising an error for missing semi-colon after assignment");

    expectedData.push_back(std::pair(std::pair("container", StyleComponentType::Class), StyleRelation::DirectParent));
    expectedData.push_back(std::pair(std::pair("label", StyleComponentType::ElementName), StyleRelation::SameElement));
    expectedData.push_back(std::pair(std::pair("red", StyleComponentType::Identifier), StyleRelation::SameElement));
    styleValue = new StyleValue("ff0000", StyleValueType::Hex);
    expectedStyleMap["text-color"] = StyleRule{styleValue, true, 0, 0, 0};
    styleBlock = new StyleBlock(&expectedData, &expectedStyleMap);
    expectedStyleBlocks = {styleBlock};
    testDeserialization(".container > label#red{text-color : #ff0000;}", "testing direct parent", &expectedStyleBlocks);
    delete styleBlock;
    delete styleValue;
    expectedStyleMap.clear();
    expectedData.clear();

    expectedData.push_back(std::pair(std::pair("container", StyleComponentType::Class), StyleRelation::DirectParent));
    expectedData.push_back(std::pair(std::pair("label", StyleComponentType::ElementName), StyleRelation::SameElement));
    expectedData.push_back(std::pair(std::pair("red", StyleComponentType::Identifier), StyleRelation::SameElement));
    styleValue = new StyleValue("ff0000", StyleValueType::Hex);
    expectedStyleMap["text-color"] = StyleRule{styleValue, true, 0, 0, 0};
    styleBlock = new StyleBlock(&expectedData, &expectedStyleMap);
    expectedStyleBlocks = {styleBlock};
    testDeserialization(".container>label#red{text-color : #ff0000;}", "testing direct parent without spaces", &expectedStyleBlocks);
    delete styleBlock;
    delete styleValue;
    expectedStyleMap.clear();
    expectedData.clear();

    expectedData.push_back(std::pair(std::pair("red-container", StyleComponentType::Identifier), StyleRelation::AnyParent));
    expectedData.push_back(std::pair(std::pair("label", StyleComponentType::ElementName), StyleRelation::SameElement));
    expectedData.push_back(std::pair(std::pair("red", StyleComponentType::Class), StyleRelation::SameElement));
    expectedData.push_back(std::pair(std::pair("test-label", StyleComponentType::Identifier), StyleRelation::SameElement));
    styleValue = new StyleValue("ff0000", StyleValueType::Hex);
    expectedStyleMap["text-color"] = StyleRule{styleValue, true, 0, 0, 0};
    styleBlock = new StyleBlock(&expectedData, &expectedStyleMap);
    expectedStyleBlocks = {styleBlock};
    testDeserializationFromFile("tests/style_tests/tests-files/main-test.txt", "testing main-test file", &expectedStyleBlocks);
    delete styleBlock;
    delete styleValue;
    expectedStyleMap.clear();
    expectedData.clear();
}