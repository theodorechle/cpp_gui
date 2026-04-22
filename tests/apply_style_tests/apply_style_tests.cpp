#include "apply_style_tests.hpp"

namespace applyStyleTests {
    test::Result testApplyingStyle(const std::string &style, gui::element::AbstractElement *element, std::vector<std::string> expectedRulesNames,
                                   bool expectToFindValues) {
        style::config::Config *config = testConfig();
        gui::elementStyle::manager::StyleManager styleManager = gui::elementStyle::manager::StyleManager(config);
        style::StyleValue *value;

        styleManager.addStyle(style);

        styleManager.applyStyleToElement(element);
        delete config;

        if (element->style()->nbRules() != expectedRulesNames.size() && expectToFindValues) {
            std::cerr << "expected " << expectedRulesNames.size() << " rules, but got " << element->style()->nbRules() << "\n";
            return test::Result::FAILURE;
        }

        for (std::string ruleName : expectedRulesNames) {
            if (element->style()->getRule(ruleName, &value) != expectToFindValues) {
                std::cerr << "rule " << ruleName;
                if (expectToFindValues) std::cerr << " not";
                std::cerr << " not found\n";
                return test::Result::FAILURE;
            }
        }

        return test::Result::SUCCESS;
    }

    test::Result testEmptyStyleDoesNothing() {
        gui::element::UiElement element = gui::element::UiElement("element");
        return testApplyingStyle("", &element, {}, false);
    }

    test::Result testStyleNotAppliedIfNotMatching() {
        gui::element::UiElement element = gui::element::UiElement("element");
        return testApplyingStyle("button {text-color: #ff0000;}", &element, {}, false);
    }

    test::Result testStyleAppliedByElementName() {
        gui::element::UiElement element = gui::element::UiElement("button");
        return testApplyingStyle("button {text-color: #ff0000;}", &element, {"text-color"}, true);
    }

    test::Result testStyleAppliedById() {
        gui::element::UiElement element = gui::element::UiElement("button", nullptr, {}, "an-id");
        return testApplyingStyle("#an-id {text-color: #ff0000;}", &element, {"text-color"}, true);
    }

    test::Result testStyleAppliedByClass() {
        gui::element::UiElement element = gui::element::UiElement("button", nullptr, {"a-class"});
        return testApplyingStyle(".a-class {text-color: #ff0000;}", &element, {"text-color"}, true);
    }

    test::Result testStyleAppliedByClassAndElementName() {
        gui::element::UiElement element = gui::element::UiElement("button", nullptr, {"a-class"});
        return testApplyingStyle("button.a-class {text-color: #ff0000;}", &element, {"text-color"}, true);
    }

    test::Result testStyleAppliedByClassDirectParentOfElementName() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element", nullptr, {"a-class"});
        parentElement.addChild(&element);
        test::Result result = testApplyingStyle(".a-class>button {text-color: #ff0000;}", &element, {"text-color"}, true);
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleAppliedByClassAnyParentOfElementNameWithDirectMatchingParent() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element", nullptr, {"a-class"});
        parentElement.addChild(&element);
        test::Result result = testApplyingStyle(".a-class button {text-color: #ff0000;}", &element, {"text-color"}, true);
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleAppliedByClassAnyParentOfElementNameWithIndirectMatchingParent() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element");
        gui::element::UiElement parentParentElement = gui::element::UiElement("element", nullptr, {"a-class"});
        parentElement.addChild(&element);
        parentParentElement.addChild(&parentElement);
        test::Result result = testApplyingStyle(".a-class button {text-color: #ff0000;}", &element, {"text-color"}, true);
        parentParentElement.removeChilds();
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleNotAppliedByClassDirectParentOfElementName() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element");
        parentElement.addChild(&element);
        test::Result result = testApplyingStyle(".a-class>button {text-color: #ff0000;}", &element, {"text-color"}, false);
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleNotAppliedByClassAnyParentOfElementNameWithDirectParent() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element");
        parentElement.addChild(&element);
        test::Result result = testApplyingStyle(".a-class button {text-color: #ff0000;}", &element, {"text-color"}, false);
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleNotAppliedByClassAnyParentOfElementNameWithIndirectParent() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element");
        gui::element::UiElement parentParentElement = gui::element::UiElement("element");
        parentElement.addChild(&element);
        parentParentElement.addChild(&parentElement);
        test::Result result = testApplyingStyle(".a-class button {text-color: #ff0000;}", &element, {"text-color"}, false);
        parentParentElement.removeChilds();
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleAppliedByWildcard() {
        gui::element::UiElement element = gui::element::UiElement("button");
        return testApplyingStyle("* {text-color: #ff0000;}", &element, {"text-color"}, true);
    }

    test::Result testStyleAppliedByWildcardDirectParentOfElementName() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element");
        parentElement.addChild(&element);
        test::Result result = testApplyingStyle("*>button {text-color: #ff0000;}", &element, {"text-color"}, true);
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleAppliedByWildcardAnyParentOfElementNameWithDirectMatchingParent() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element");
        parentElement.addChild(&element);
        test::Result result = testApplyingStyle("* button {text-color: #ff0000;}", &element, {"text-color"}, true);
        parentElement.removeChilds();
        return result;
    }

    test::Result testStyleAppliedByWildcardAnyParentOfElementNameWithIndirectMatchingParent() {
        gui::element::UiElement element = gui::element::UiElement("button");
        gui::element::UiElement parentElement = gui::element::UiElement("element");
        gui::element::UiElement parentParentElement = gui::element::UiElement("element");
        parentElement.addChild(&element);
        parentParentElement.addChild(&parentElement);
        test::Result result = testApplyingStyle("* button {text-color: #ff0000;}", &element, {"text-color"}, true);
        parentParentElement.removeChilds();
        parentElement.removeChilds();
        return result;
    }

    void applyStyleTests(test::Tests *tests) {
        tests->beginTestBlock("apply style");
        tests->addTest(testEmptyStyleDoesNothing, "empty style does nothing");
        tests->addTest(testStyleNotAppliedIfNotMatching, "style not applied if not matching");
        tests->addTest(testStyleAppliedByElementName, "style applied by element name");
        tests->addTest(testStyleAppliedById, "style applied by id");
        tests->addTest(testStyleAppliedByClass, "style applied by class");
        tests->addTest(testStyleAppliedByClassAndElementName, "style applied by class and element name");
        tests->addTest(testStyleAppliedByClassDirectParentOfElementName, "style applied by class direct parent of element name");
        tests->addTest(testStyleAppliedByClassAnyParentOfElementNameWithDirectMatchingParent,
                       "style applied by class any parent of element name with direct matching parent");
        tests->addTest(testStyleAppliedByClassAnyParentOfElementNameWithIndirectMatchingParent,
                       "style applied by class any parent of element name with indirect matching parent");
        tests->addTest(testStyleNotAppliedByClassDirectParentOfElementName, "style not applied by class direct parent of element name");
        tests->addTest(testStyleNotAppliedByClassAnyParentOfElementNameWithDirectParent,
                       "style not applied by class any parent of element name with direct parent");
        tests->addTest(testStyleNotAppliedByClassAnyParentOfElementNameWithIndirectParent,
                       "style not applied by class any parent of element name with indirect parent");

        tests->beginTestBlock("wildcard");
        tests->addTest(testStyleAppliedByWildcard, "style applied by wildcard");
        tests->addTest(testStyleAppliedByWildcardDirectParentOfElementName, "style applied by wildcard direct parent of element name");
        tests->addTest(testStyleAppliedByWildcardAnyParentOfElementNameWithDirectMatchingParent,
                       "style applied by wildcard any parent of element name with direct matching parent");
        tests->addTest(testStyleAppliedByWildcardAnyParentOfElementNameWithIndirectMatchingParent,
                       "style applied by wildcard any parent of element name with indirect matching parent");
        tests->endTestBlock();

        tests->endTestBlock();
    }

} // namespace applyStyleTests
