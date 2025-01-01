#include "style_tests.hpp"

void StyleTests::testStyleComponentDataList(const StyleComponentDataList *testedData, const StyleComponentDataList *expected) {
    startTest();
    endTest();
}

void StyleTests::tests() {
    std::string style = ".container label#red{text-color:#ff0000;}";
    int fileNumber = 0;
    int ruleNumber;
    std::list<StyleComponent *> *components = StyleDeserializer::deserialize(style, fileNumber, &ruleNumber);
    if (ruleNumber != 1) {
        startTest();
        std::cerr << "ruleNumber is " << ruleNumber << " instead of 1\n";
        setTestResult(Result::KO);
        endTest();
    }
    else if (components->size() != 1) {
        startTest();
        std::cerr << components->size() << " components instead of 1 expected\n";
        setTestResult(Result::KO);
        endTest();
    }
    else {
        // testStyleComponentDataList(components->front()->getComponentsList(), );
    }

    for (StyleComponent *component : *components) {
        delete component;
    }
    components->clear();
    delete components;
}