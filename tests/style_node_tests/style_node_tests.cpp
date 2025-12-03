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

        if (!node->deleteStyle(2, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::RulesMap::const_iterator result = node->rules().find("test");
            if (result == nullptr) {
                std::cerr << "rule not found\n";
                testResult = test::Result::FAILURE;
            }
            else {
                style::StyleRule rule = result->second;
                style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
                testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 2, 3)));
            }
        }
        delete node;
        return testResult;
    }

    test::Result testDeleteDisabledRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->rules(style);
        test::Result testResult;

        if (!node->deleteStyle(5, 3)) {
            std::cerr << "deleteStyle returned false instead of true\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::RulesMap::const_iterator result = node->rules().find("test");
            if (result == nullptr) {
                std::cerr << "rule not found\n";
                testResult = test::Result::FAILURE;
            }
            else {
                style::StyleRule rule = result->second;
                style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
                testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 2, 3)));
            }
        }
        delete node;
        return testResult;
    }

    test::Result testDeleteRulesFromFile() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 2, 3)}}};
        node->rules(style);
        test::Result testResult;

        node->deleteStyleFromFile(5);
        style::RulesMap::const_iterator result = node->rules().find("test");
        if (result != nullptr) {
            std::cerr << "deleteStyleFromFile didn't delete the rule\n";
            testResult = test::Result::FAILURE;
        }
        else {
            style::RulesMap::const_iterator result = node->rules().find("test");
            if (result == nullptr) {
                std::cerr << "rule not found\n";
                testResult = test::Result::FAILURE;
            }
            else {
                style::StyleRule rule = result->second;
                style::StyleValue expectedValue = style::StyleValue("abc", style::StyleValueType::String);
                testResult = test::booleanToResult(testRule(true, rule, style::StyleRule(&expectedValue, true, 3, 2, 3)));
            }
        }
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

    test::Result testNbRulesMultipleValuesOnOneRule() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        style::StyleValue value2 = style::StyleValue("yte", style::StyleValueType::String);
        style::RulesMap style2 = {{"test", {style::StyleRule(&value2, true, 3, 5, 3)}}};
        node->rules(style2);
        int result = node->nbRules();
        delete node;
        if (result != 2) {
            std::cerr << "expected 2 rules, got " << result << "\n";
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
        bool result = node->getRule("test", &styleValue, false, nullptr);
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

    test::Result testRuleExistsWithRuleNameIsTrue() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        bool result = node->ruleExists("test");
        if (!result) {
            std::cerr << "ruleExists returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithRuleNameIsTrueDisabled() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, false, 3, 5, 3)}}};
        node->rules(style);
        bool result = node->ruleExists("test");
        if (!result) {
            std::cerr << "ruleExists returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithRuleNameIsFalse() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        bool result = node->ruleExists("test3");
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithRuleNameWithoutRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;

        bool result = node->ruleExists("test");
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithNumbersIsTrue() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};

        bool result = node->ruleExists(5, 3);
        if (!result) {
            std::cerr << "ruleExists returned false instead of expected true\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithNumbersIsFalse() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::StyleValue value2 = style::StyleValue("def", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}, {"test2", {style::StyleRule(&value2, true, 7, 5, 3)}}};

        bool result = node->ruleExists(12, 17);
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testRuleExistsWithNumbersWithoutRules() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;

        bool result = node->ruleExists(1, 2);
        if (result) {
            std::cerr << "ruleExists returned true instead of expected false\n";
            testResult = test::Result::FAILURE;
        }
        else testResult = test::Result::SUCCESS;
        delete node;
        return testResult;
    }

    test::Result testGetSelectorsEmpty() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (!result->empty()) {
            std::cerr << "getSelectors returned list with " << result->size() << " elements instead of empty list\n";
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

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (result->size() != 1) {
            std::cerr << "getSelectors returned list of size " << result->size() << " instead of list of size 1\n";
            testResult = test::Result::FAILURE;
        }
        else
            testResult =
                test::booleanToResult(result->find(style::StyleComponentData{"myClass", style::StyleComponentType::Class}) != result->cend());
        delete node;
        return testResult;
    }

    test::Result testSetAndGetMultipleSelectors() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addSelector("myClass", style::StyleComponentType::Class);
        node->addSelector("myId", style::StyleComponentType::Identifier);

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (result->size() != 2) {
            std::cerr << "getSelectors returned list of size " << result->size() << " instead of list of size 2\n";
            testResult = test::Result::FAILURE;
        }
        else
            testResult =
                test::booleanToResult(result->find(style::StyleComponentData{"myClass", style::StyleComponentType::Class}) != result->cend()
                                      && result->find(style::StyleComponentData{"myId", style::StyleComponentType::Identifier}) != result->cend());
        delete node;
        return testResult;
    }

    test::Result testSetAndGetMultipleSelectorsSameType() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addSelector("myClass", style::StyleComponentType::Class);
        node->addSelector("mySecondClass", style::StyleComponentType::Class);

        const std::set<style::StyleComponentData> *result = node->getSelectors();
        if (result->size() != 2) {
            std::cerr << "getSelectors returned list of size " << result->size() << " instead of list of size 2\n";
            testResult = test::Result::FAILURE;
        }
        else
            testResult = test::booleanToResult(result->find(style::StyleComponentData{"myClass", style::StyleComponentType::Class}) != result->cend()
                                               && result->find(style::StyleComponentData{"mySecondClass", style::StyleComponentType::Class})
                                                      != result->cend());
        delete node;
        return testResult;
    }

    test::Result testToggleRuleDisable() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        node->toggleRule(5, 3);
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

    test::Result testToggleRuleReenable() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        node->toggleRule(5, 3);
        node->toggleRule(5, 3);
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

    test::Result testToggleRuleForceEnable() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        node->toggleRule(5, 3, true);
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

    test::Result testToggleRuleForceDisable() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        node->toggleRule(5, 3, false);
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

    test::Result testToggleRuleForceReenable() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        style::StyleValue value = style::StyleValue("abc", style::StyleValueType::String);
        style::RulesMap style = {{"test", {style::StyleRule(&value, true, 3, 5, 3)}}};
        node->rules(style);
        node->toggleRule(5, 3, false);
        node->toggleRule(5, 3, true);
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

    test::Result testFontsPathEmpty() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "/");
        delete node;
        return testResult;
    }

    test::Result testFontsPath() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addDefaultFontPath("path");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path/");
        delete node;
        return testResult;
    }

    test::Result testFontsPathWithTrailingSlash() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addDefaultFontPath("path/");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path/");
        delete node;
        return testResult;
    }

    test::Result testFontsPathOverride() {
        gui::elementStyle::ElementStyle *node = new gui::elementStyle::ElementStyle();
        test::Result testResult;
        node->addDefaultFontPath("path");
        node->addDefaultFontPath("path2");
        std::string path = node->getFontsPath();
        testResult = test::booleanToResult(path == "path2/");
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
        tests->addTest(testNbRulesMultipleValuesOnOneRule, "nb rules multiple values on one rule");
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
        tests->beginTestBlock("check if rules exists");
        tests->addTest(testRuleExistsWithRuleNameIsTrue, "rule exists with rule name is true");
        tests->addTest(testRuleExistsWithRuleNameIsTrueDisabled, "rule exists with rule name is true disabled");
        tests->addTest(testRuleExistsWithRuleNameIsFalse, "rule exists with rule name is false");
        tests->addTest(testRuleExistsWithRuleNameWithoutRules, "rule exists with rule name without rules");
        tests->addTest(testRuleExistsWithNumbersIsTrue, "rule exists with numbers is true");
        tests->addTest(testRuleExistsWithNumbersIsFalse, "rule exists with numbers is false");
        tests->addTest(testRuleExistsWithNumbersWithoutRules, "rule exists with numbers without rules");
        tests->endTestBlock();
        tests->beginTestBlock("get and set selectors");
        tests->addTest(testGetSelectorsEmpty, "get selectors empty");
        tests->addTest(testSetAndGetSelector, "set and get selector");
        tests->addTest(testSetAndGetMultipleSelectors, "set and get multiple selectors");
        tests->addTest(testSetAndGetMultipleSelectorsSameType, "set and get multiple selectors of same type");
        tests->endTestBlock();
        tests->beginTestBlock("modifiers");
        tests->endTestBlock();
        tests->beginTestBlock("toggle rules");
        tests->addTest(testToggleRuleDisable, "toggle rule disable");
        tests->addTest(testToggleRuleReenable, "toggle rule reenable");
        tests->addTest(testToggleRuleForceEnable, "toggle rule force enable");
        tests->addTest(testToggleRuleForceDisable, "toggle rule force disable");
        tests->addTest(testToggleRuleForceReenable, "toggle rule force reenable");
        tests->endTestBlock();
        tests->beginTestBlock("fonts path");
        tests->addTest(testFontsPathEmpty, "fonts path empty");
        tests->addTest(testFontsPath, "fonts path");
        tests->addTest(testFontsPathWithTrailingSlash, "fonts path with trailing slash");
        tests->addTest(testFontsPathOverride, "fonts path override");
        tests->endTestBlock();
        tests->endTestBlock();
    }

} // namespace styleNodesTests