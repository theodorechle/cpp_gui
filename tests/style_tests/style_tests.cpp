#include "style_tests.hpp"

Result StyleTests::testStyleComponentDataList(const StyleComponentDataList *testedData, const StyleComponentDataList *expectedData) {
    if (testedData == nullptr && expectedData == nullptr) {
        return Result::OK;
    }
    if ((testedData == nullptr && expectedData != nullptr) || (testedData != nullptr && expectedData == nullptr) || (testedData->size() != testedData->size())) {
        std::cerr << "One of the data list is null or the lists doesn't have the same size\n";
        return Result::KO;
    }
    StyleComponentDataList::const_iterator testedDataListIterator = testedData->cbegin();
    StyleComponentDataList::const_iterator expectedDataListIterator = expectedData->cbegin();
    while (testedDataListIterator != testedData->cend()) {
        if (testedDataListIterator->second != expectedDataListIterator->second) {
            std::cerr << styleRelationToString(testedDataListIterator->second) << " instead of relation " << styleRelationToString(expectedDataListIterator->second) << "\n";
            return Result::KO;
        }
        if (testedDataListIterator->first.first != expectedDataListIterator->first.first) {
            std::cerr << testedDataListIterator->first.first << " instead of name " << expectedDataListIterator->first.first << "\n";
            return Result::KO;
        }
        if (testedDataListIterator->first.second != expectedDataListIterator->first.second) {
            std::cerr << styleComponentTypeToString(testedDataListIterator->first.second) << " instead of type " << styleComponentTypeToString(expectedDataListIterator->first.second) << "\n";
            return Result::KO;
        }
        testedDataListIterator++;
        expectedDataListIterator++;
    }
    return Result::OK;
}

void StyleTests::tests() {
    std::string style;
    int fileNumber;
    int ruleNumber;
    StyleComponentDataList expectedData;
    std::list<StyleComponent *> *components;

    style = ".container      label#red{text-color : #ff0000;}";
    fileNumber = 0;
    expectedData = StyleComponentDataList();
    startTest();
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
        setTestResult(testStyleComponentDataList(components->front()->getComponentsList(), &expectedData));
    }

    expectedData.clear();
    for (StyleComponent *component : *components) {
        delete component;
    }
    delete components;

    endTest();

    style = ".container      label#red{text-color : #ff0000}";
    fileNumber = 0;
    expectedData = StyleComponentDataList();
    startTest();
    try {
        components = StyleDeserializer::deserialize(style, fileNumber, &ruleNumber, true);
        setTestResult(Result::KO);

        for (StyleComponent *component : *components) {
            delete component;
        }
        delete components;
    }
    catch (const MalformedExpression &e) {
        setTestResult(Result::OK);
    }

    endTest();
}