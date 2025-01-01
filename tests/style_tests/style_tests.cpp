#include "style_tests.hpp"

Result StyleTests::testStyleComponentDataList(const StyleComponentDataList *testedData, const StyleComponentDataList *expectedData) {
    if (testedData == nullptr && expectedData == nullptr) return Result::OK;
    if ((testedData == nullptr && expectedData != nullptr) || (testedData != nullptr && expectedData == nullptr) || (testedData->size() != expectedData->size())) {
        std::cerr << "One of the data list is null or the lists doesn't have the same size\n";
        return Result::KO;
    }
    StyleComponentDataList::const_iterator testedDataListIt = testedData->cbegin();
    StyleComponentDataList::const_iterator expectedDataListIt = expectedData->cbegin();
    while (testedDataListIt != testedData->cend()) {
        if (testedDataListIt->second != expectedDataListIt->second) {
            std::cerr << styleRelationToString(testedDataListIt->second) << " instead of relation " << styleRelationToString(expectedDataListIt->second) << "\n";
            return Result::KO;
        }
        if (testedDataListIt->first.first != expectedDataListIt->first.first) {
            std::cerr << testedDataListIt->first.first << " instead of name " << expectedDataListIt->first.first << "\n";
            return Result::KO;
        }
        if (testedDataListIt->first.second != expectedDataListIt->first.second) {
            std::cerr << styleComponentTypeToString(testedDataListIt->first.second) << " instead of type " << styleComponentTypeToString(expectedDataListIt->first.second) << "\n";
            return Result::KO;
        }
        testedDataListIt++;
        expectedDataListIt++;
    }
    return Result::OK;
}

Result StyleTests::testStyleValue(StyleValue *testedValue, StyleValue *expectedValue) {
    Result result;

    if (testedValue == nullptr && expectedValue == nullptr) return Result::OK;
    if ((testedValue == nullptr && expectedValue != nullptr) || (testedValue != nullptr && expectedValue == nullptr)) {
        std::cerr << "One of the rule is null\n";
        return Result::KO;
    }
    if (testedValue->getName() != expectedValue->getName()) {
        std::cerr << "The name is different (have " << testedValue->getName() << ", expected " << expectedValue->getName() << "\n";
        return Result::KO;
    }
    if (testedValue->getType() != expectedValue->getType()) {
        std::cerr << "The type is different (have " << styleValueTypeToString(testedValue->getType()) << ", expected " << styleValueTypeToString(expectedValue->getType()) << "\n";
        return Result::KO;
    }

    result = testStyleValue(testedValue->getChild(), expectedValue->getChild());
    if (result != Result::OK) return result;

    result = testStyleValue(testedValue->getNext(), expectedValue->getNext());
    if (result != Result::OK) return result;
    return Result::OK;
}

Result StyleTests::testStyleRule(const StyleRule *testedRule, const StyleRule *expectedRule) {
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
    return testStyleValue(testedRule->value, expectedRule->value);
}

Result StyleTests::testStyleMap(const StyleValuesMap *testedStyleMap, const StyleValuesMap *expectedStyleMap) {
    StyleValuesMap::const_iterator ruleIt;
    Result styleRuleCheckResult;
    if (testedStyleMap == nullptr && expectedStyleMap == nullptr) return Result::OK;
    if ((testedStyleMap == nullptr && expectedStyleMap != nullptr) || (testedStyleMap != nullptr && expectedStyleMap == nullptr) || (testedStyleMap->size() != expectedStyleMap->size())) {
        std::cerr << "One of the style maps list is null or the maps doesn't have the same size\n";
        return Result::KO;
    }

    for (const std::pair<std::string, StyleRule> rule : *expectedStyleMap) {
        ruleIt = testedStyleMap->find(rule.first);
        if (ruleIt == testedStyleMap->cend()) {
            std::cerr << "Rule " << rule.first << " can't be found in the tested map\n";
            return Result::KO;
        }
        styleRuleCheckResult = testStyleRule(&(rule.second), &(ruleIt->second));
        if (styleRuleCheckResult != Result::OK) return styleRuleCheckResult;
    }
    return Result::OK;
}

void StyleTests::tests() {
    std::string style;
    int fileNumber;
    int ruleNumber;
    Result result;
    StyleComponentDataList expectedData;
    StyleValuesMap expectedStyleMap;
    std::list<StyleBlock *> *components;

    style = ".container      label#red{text-color : #ff0000;}";
    fileNumber = 0;
    expectedData = StyleComponentDataList();
    startTest("deserializing a single rule");
    components = StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, true);
    if (ruleNumber != 1) {
        std::cerr << "ruleNumber is " << ruleNumber << " instead of 1\n";
        setTestResult(Result::KO);
    }
    else if (components->size() != 1) {
        std::cerr << components->size() << " components instead of 1 expected\n";
        setTestResult(Result::KO);
    }
    else {
        expectedData.push_back(std::pair(std::pair("container", StyleComponentType::Class), StyleRelation::DirectParent));
        expectedData.push_back(std::pair(std::pair("label", StyleComponentType::ElementName), StyleRelation::SameElement));
        expectedData.push_back(std::pair(std::pair("red", StyleComponentType::Identifier), StyleRelation::SameElement));
        result = testStyleComponentDataList(components->front()->getComponentsList(), &expectedData);
        if (result != Result::OK) setTestResult(result);
        else {
            expectedStyleMap["text-color"] = StyleRule{new StyleValue("#ff0000", StyleValueType::String), true, 0, 0, 0};
            setTestResult(testStyleMap(components->front()->getStyleMap(), &expectedStyleMap));
        }
    }

    expectedData.clear();
    for (StyleBlock *component : *components) {
        for (const std::pair<std::string, StyleRule> rule : *(component->getStyleMap())) {
            delete rule.second.value;
        }
        delete component;
    }
    delete components;
    for (const std::pair<std::string, StyleRule> rule : expectedStyleMap) {
        delete rule.second.value;
    }

    endTest();

    style = ".container      label#red{text-color : #ff0000}";
    fileNumber = 0;
    expectedData = StyleComponentDataList();
    startTest("raising an error for missing semi-colon after assignment");
    try {
        components = StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, true);
        setTestResult(Result::KO);

        for (StyleBlock *component : *components) {
            delete component;
        }
        delete components;
    }
    catch (const MalformedExpression &e) {
        setTestResult(Result::OK);
    }

    endTest();
}