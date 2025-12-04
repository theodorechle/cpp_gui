#include "style_node_tests.hpp"

namespace styleNodesTests {
    void displayValue(style::StyleValue *value) {
        if (value == nullptr) {
            std::cerr << "null";
            return;
        }
        std::cerr << value->getValue() << "<" << style::styleValueTypeToString(value->getType()) << ">";
        style::StyleValue *next = value->getNext();
        if (next != nullptr) {
            std::cerr << ",";
            displayValue(next);
        }
        style::StyleValue *child = value->getChild();
        if (child != nullptr) {
            std::cerr << "(";
            while (child != nullptr) {
                displayValue(child);
                child = child->getChild();
            }
            std::cerr << ")";
        }
    }

    bool testValue(bool equal, style::StyleValue *value, style::StyleValue *expected, bool displayDebug) {
        bool result = (value == nullptr || expected == nullptr)
                          ? (value == expected) == equal
                          : ((value->getValue() == expected->getValue() && value->getType() == expected->getType()) == equal);
        if (!result && displayDebug) {
            std::cerr << "attribute -> given\t|\texpected:\n";
            std::cerr << "value -> ";
            displayValue(value);
            std::cerr << "\t|\t";
            displayValue(expected);
            std::cerr << "\n";
        }
        return result;
    }

    bool testRule(bool equal, const style::StyleRule &rule, const style::StyleRule &expected) {
        std::cout << "test if two rules are " << (equal ? "equal" : "not equal") << "\n";
        if ((rule.enabled != expected.enabled
             || rule.fileNumber != expected.fileNumber
             || rule.ruleNumber != expected.ruleNumber
             || rule.specificity != expected.specificity
             || testValue(equal, rule.value, expected.value))
            != equal) {
            std::cerr << "rules are " << (equal ? "not equal" : "equal") << "\n";
            std::cerr << "attribute -> given\t|\texpected:\n";
            std::cerr << "-----\n";
            std::cerr << "enabled -> " << rule.enabled << "\t\t|\t" << expected.enabled << "\n";
            std::cerr << "fileNumber -> " << rule.fileNumber << "\t|\t" << expected.fileNumber << "\n";
            std::cerr << "ruleNumber -> " << rule.ruleNumber << "\t|\t" << expected.ruleNumber << "\n";
            std::cerr << "specificity -> " << rule.specificity << "\t|\t" << expected.specificity << "\n";
            std::cerr << "value -> ";
            displayValue(rule.value);
            std::cerr << "\t|\t";
            displayValue(expected.value);
            std::cerr << "\n";
            return false;
        }
        return true;
    }

    test::Result testGetRuleWithoutRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::RulesMap::const_iterator result = node->rules().find("test");
        test::Result testResult;
        if (result != nullptr) {
            std::cerr << "rule found but shouldn't exist\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testAddAndGetDefaultRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::RulesMap style = {{"test", {style::StyleRule()}}};
        node->rules(style);
        style::RulesMap::const_iterator result = node->rules().find("test");
        test::Result testResult;
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second;
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule()));
        }
        delete node;
        return testResult;
    }

    test::Result testAddDisabledRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->rules(style);
        style::RulesMap::const_iterator result = node->rules().find("test");
        test::Result testResult;
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second;
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, false, 3, 5, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testAddCustomRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        style::RulesMap::const_iterator result = node->rules().find("test");
        test::Result testResult;
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second;
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 5, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testDeleteRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->rules(style);
        test::Result testResult;

        node->deleteStyle(2, 3);
        if (node->rules().find("test") != nullptr) {
            std::cerr << "deleteStyle didn't delete\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;

        delete node;
        return testResult;
    }

    test::Result testDeleteDisabledRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->rules(style);
        test::Result testResult;

        node->deleteStyle(2, 3);
        if (node->rules().find("test") != nullptr) {
            std::cerr << "deleteStyle didn't delete\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;

        delete node;
        return testResult;
    }

    test::Result testDeleteRulesFromFile() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->rules(style);
        test::Result testResult;

        node->deleteStyleFromFile(2);
        if (node->rules().find("test") != nullptr) {
            std::cerr << "deleteStyleFromFile didn't delete\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testUpdateRulePriorityFromFile() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        test::Result testResult;

        node->updateStylePriorityFromFile(5, 7);
        style::RulesMap::const_iterator result = node->rules().find("test");
        if (result == nullptr) {
            std::cerr << "rule not found\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleRule rule = result->second;
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 7, 3)));
        }
        delete node;
        return testResult;
    }

    test::Result testClear() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);

        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testClearWithDisabledRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->rules(style);

        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testClearMultipleRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("pou", style::StyleValueType::Int);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, false, 7, 2, 18)}}};
        node->rules(style);

        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesEmpty() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();

        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesOneRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        int result = node->nbRules();
        delete node;
        if (result != 1) {
            std::cerr << "expected 1 rule, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesOneDisabledRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->rules(style);
        int result = node->nbRules();
        delete node;
        if (result != 1) {
            std::cerr << "expected 1 rule, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesMultipleRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value3 = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value4 = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value5 = style::StyleValue("abc", style::StyleValueType::String);

        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}},
                                 {"aze", {style::StyleRule(&value2, true, 3, 5, 3)}},
                                 {"poi", {style::StyleRule(&value3, true, 3, 5, 3)}},
                                 {"ergh", {style::StyleRule(&value4, true, 3, 5, 3)}},
                                 {"hrg", {style::StyleRule(&value5, true, 3, 5, 3)}}};
        node->rules(style);

        int result = node->nbRules();
        delete node;
        if (result != 5) {
            std::cerr << "expected 5 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testNbRulesAfterCleared() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        node->clear();
        int result = node->nbRules();
        delete node;
        if (result != 0) {
            std::cerr << "expected 0 rules, got " << result << "\n";
            return test::Result::FAILURE;
        }
        return test::Result::SUCCESS;
    }

    test::Result testGetOneRuleOfOne() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetOneRuleOfOneDisabled() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->rules(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetOneRuleOfMultiple() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("oiuy", style::StyleValueType::String);

        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->rules(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetRuleWithNoMatchingRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test3", &styleValue, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetRuleWithNoRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;

        style::StyleValue *styleValue;
        bool result = node->getRule("test", &styleValue, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;

        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithListOfRuleNamesWithSameSpecificity() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->rules(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test", "test2"};
        bool result = node->getRule(ruleNames, &styleValue, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithListOfRuleNames() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};
        node->rules(style);
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test", "test2"};
        bool result = node->getRule(ruleNames, &styleValue, nullptr);
        if (!result) {
            std::cerr << "getRule returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::StyleValue expectedValue = style::StyleValue("def", style::StyleValueType::String);
            testResult = test::booleanToResult(testValue(true, styleValue, &expectedValue, true));
        }
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueInEmptyListOfRuleNames() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};

        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {};
        bool result = node->getRule(ruleNames, &styleValue, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithNoMatchingRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};

        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test3"};
        bool result = node->getRule(ruleNames, &styleValue, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetRuleValueWithNoRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;

        style::StyleValue *styleValue;
        const std::vector<std::string> ruleNames = {"test"};
        bool result = node->getRule(ruleNames, &styleValue, nullptr);
        if (result) {
            std::cerr << "getRule returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testDoesntHasSelector() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;

        if (node->hasSelector({"selector", style::StyleComponentType::Class})) {
            std::cerr << "selector found but wasn't created\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testSetAndGetSelector() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addSelector("myClass", style::StyleComponentType::Class);

        if (!node->hasSelector({"myClass", style::StyleComponentType::Class})) {
            std::cerr << "selector not found\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;

        delete node;
        return testResult;
    }

    test::Result testSetAndGetMultipleSelectors() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult = test::Result::SUCCESS;
        node->addSelector("myClass", style::StyleComponentType::Class);
        node->addSelector("myId", style::StyleComponentType::Identifier);

        if (!node->hasSelector({"myClass", style::StyleComponentType::Class})) {
            std::cerr << "first selector not found\n";
            testResult = test::Result::FAILURE;
        }
        if (!node->hasSelector({"myId", style::StyleComponentType::Identifier})) {
            std::cerr << "second selector not found\n";
            testResult = test::Result::FAILURE;
        }
        delete node;
        return testResult;
    }

    test::Result testSetAndGetMultipleSelectorsSameType() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult = test::Result::SUCCESS;
        node->addSelector("myClass", style::StyleComponentType::Class);
        node->addSelector("mySecondClass", style::StyleComponentType::Class);

        if (!node->hasSelector({"myClass", style::StyleComponentType::Class})) {
            std::cerr << "first selector not found\n";
            testResult = test::Result::FAILURE;
        }
        if (!node->hasSelector({"mySecondClass", style::StyleComponentType::Class})) {
            std::cerr << "second selector not found\n";
            testResult = test::Result::FAILURE;
        }
        delete node;
        return testResult;
    }

    test::Result testFontsPathEmpty() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        testResult = test::booleanToResult(node->getDefaultFontsPaths().empty());
        delete node;
        return testResult;
    }

    test::Result testFontsPath() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addDefaultFontPath("path");
        const std::unordered_set<std::string> &paths = node->getDefaultFontsPaths();
        testResult = test::booleanToResult(paths.size() == 1 && paths.find("path") != nullptr);
        delete node;
        return testResult;
    }

    test::Result testMultipleFontsPaths() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addDefaultFontPath("path");
        node->addDefaultFontPath("path2");
        const std::unordered_set<std::string> &paths = node->getDefaultFontsPaths();
        testResult = test::booleanToResult(paths.size() == 2 && paths.find("path") != nullptr && paths.find("path2") != nullptr);
        delete node;
        return testResult;
    }

    void testsStyleNodes(test::Tests *tests) {
        tests->beginTestBlock("Tests style nodes");
        tests->beginTestBlock("Add rules");
        tests->addTest(testGetRuleWithoutRules, "get rule without rules");
        tests->addTest(testAddAndGetDefaultRule, "add and get default rule");
        tests->addTest(testAddDisabledRule, "add disabled rule");
        tests->addTest(testAddCustomRule, "add custom rule");
        tests->endTestBlock();
        tests->beginTestBlock("Delete rules");
        tests->addTest(testDeleteRule, "delete rule");
        tests->addTest(testDeleteDisabledRule, "delete disabled rule");
        tests->addTest(testDeleteRulesFromFile, "delete rules from file");
        tests->addTest(testUpdateRulePriorityFromFile, "update rule priority from file");
        tests->addTest(testClear, "clear");
        tests->addTest(testClearWithDisabledRule, "clear with disabled rule");
        tests->addTest(testClearMultipleRules, "clear mutliple rules");
        tests->endTestBlock();
        tests->beginTestBlock("Get nb rules");
        tests->addTest(testNbRulesEmpty, "nb rules empty");
        tests->addTest(testNbRulesOneRule, "nb rules one rule");
        tests->addTest(testNbRulesOneDisabledRule, "nb rules one disabled rule");
        tests->addTest(testNbRulesMultipleRules, "nb rules multiple rules");
        tests->addTest(testNbRulesAfterCleared, "nb rules after cleared");
        tests->endTestBlock();
        tests->beginTestBlock("get rule values");
        tests->addTest(testGetOneRuleOfOne, "get one rule of one");
        tests->addTest(testGetOneRuleOfOneDisabled, "get one rule of one disabled");
        tests->addTest(testGetOneRuleOfMultiple, "get one rule of multiple");
        tests->addTest(testGetRuleWithNoMatchingRules, "get rule with no matching rules");
        tests->addTest(testGetRuleWithNoRules, "get rule with no rules");
        tests->addTest(testGetRuleValueWithListOfRuleNamesWithSameSpecificity, "get rule value with list of rule names with same specificity");
        tests->addTest(testGetRuleValueWithListOfRuleNames, "get rule value with list of rule names");
        tests->addTest(testGetRuleValueInEmptyListOfRuleNames, "get rule value in empty list of rule names");
        tests->addTest(testGetRuleValueWithNoMatchingRules, "get rule value with no matching rules");
        tests->addTest(testGetRuleValueWithNoRules, "get rule value with no rules");
        tests->endTestBlock();
        tests->beginTestBlock("get and set selectors");
        tests->addTest(testDoesntHasSelector, "get selectors empty");
        tests->addTest(testSetAndGetSelector, "set and get selector");
        tests->addTest(testSetAndGetMultipleSelectors, "set and get multiple selectors");
        tests->addTest(testSetAndGetMultipleSelectorsSameType, "set and get multiple selectors of same type");
        tests->endTestBlock();
        tests->beginTestBlock("default fonts paths");
        tests->addTest(testFontsPathEmpty, "fonts path empty");
        tests->addTest(testFontsPath, "fonts path");
        tests->addTest(testMultipleFontsPaths, "fonts path override");
        tests->endTestBlock();
        tests->endTestBlock();
    }

} // namespace styleNodesTests