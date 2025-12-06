#include "element_style.hpp"

namespace gui::elementStyle {
    bool ElementStyle::compareRulesPriorityAscending(const style::StyleRule &rule1, const style::StyleRule &rule2) {
        if (rule1.specificity < rule2.specificity) return true;
        if (rule1.specificity == rule2.specificity) {
            if (rule1.fileNumber < rule2.fileNumber) return true;
            if (rule1.fileNumber == rule2.fileNumber) {
                if (rule1.ruleNumber < rule2.ruleNumber) return true;
            }
        }
        return false;
    }

    bool ElementStyle::compareRulesPriorityDescending(const style::StyleRule &rule1, const style::StyleRule &rule2) {
        if (rule1.specificity > rule2.specificity) return true;
        if (rule1.specificity == rule2.specificity) {
            if (rule1.fileNumber > rule2.fileNumber) return true;
            if (rule1.fileNumber == rule2.fileNumber) {
                if (rule1.ruleNumber > rule2.ruleNumber) return true;
            }
        }
        return false;
    }
    
    void ElementStyle::addSelector(const std::string &selectorName, style::StyleComponentType selectorType) {
        _selectors.insert(style::StyleComponentData(selectorName, selectorType));
    }

    bool ElementStyle::hasSelector(const style::StyleComponentData &selector) const { return _selectors.find(selector) != _selectors.cend(); }

    void ElementStyle::updateStylePriorityFromFile(int oldFileNumber, int newFileNumber) {
        for (style::RulesMap::iterator it = _rules.begin(); it != _rules.end(); it++) {
            if (it->second.fileNumber == oldFileNumber) {
                it->second.fileNumber = newFileNumber;
            }
        }
    }

    void ElementStyle::clear() { _rules.clear(); }

    void ElementStyle::activateModifier(const std::string &modifier) { _selectors.insert({modifier, style::StyleComponentType::Modifier}); }

    void ElementStyle::deactivateModifier(const std::string &modifier) { _selectors.erase({modifier, style::StyleComponentType::Modifier}); }

    void ElementStyle::deleteStyle(int fileNumber, int ruleNumber) {
        for (style::RulesMap::iterator it = _rules.begin(); it != _rules.end(); it++) {
            if (it->second.fileNumber == fileNumber && it->second.ruleNumber == ruleNumber) {
                _rules.erase(it->first);
                return;
            }
        }
    }

    void ElementStyle::deleteStyleFromFile(int fileNumber) {
        for (style::RulesMap::iterator it = _rules.begin(); it != _rules.end(); it++) {
            if (it->second.fileNumber == fileNumber) {
                _rules.erase(it->first);
                return;
            }
        }
    }

    std::list<std::string> ElementStyle::debugToString() const {
        std::list<std::string> pathFragment = {};
        for (const style::StyleComponentData &selector : _selectors) {
            char firstChar[2] = "";
            switch (selector.second) {
            case style::StyleComponentType::Identifier:
                firstChar[0] = '#';
                break;
            case style::StyleComponentType::Class:
                firstChar[0] = '.';
                break;
            case style::StyleComponentType::Modifier:
                firstChar[0] = ':';
                break;
            default:
                break;
            }
            pathFragment.push_back(firstChar + selector.first);
        }
        return pathFragment;
    }

    bool ElementStyle::getRule(const std::string &ruleName, style::StyleValue **ruleValue, style::StyleValue *defaultStyle) const {
        style::RulesMap::const_iterator rule = _rules.find(ruleName);
        if (rule != nullptr) {
            if (rule->second.enabled) {
                *ruleValue = rule->second.value;
                return true;
            }
        }
        *ruleValue = defaultStyle;
        return (defaultStyle != nullptr);
    }

    bool ElementStyle::getRule(const std::vector<std::string> &ruleNames, style::StyleValue **ruleValue, style::StyleValue *defaultStyle) const {
        const style::StyleRule *currentRule = nullptr;
        for (const std::string &ruleName : ruleNames) {
            style::RulesMap::const_iterator rule = _rules.find(ruleName);
            if (rule != nullptr && rule->second.enabled) {
                if (currentRule == nullptr || compareRulesPriorityAscending(*currentRule, rule->second)) currentRule = &(rule->second);
            }
        }
        if (currentRule != nullptr) {
            *ruleValue = currentRule->value;
            return true;
        }
        *ruleValue = defaultStyle;
        return (defaultStyle != nullptr);
    }

    int ElementStyle::nbRules() const { return _rules.size(); }

} // namespace gui::elementStyle
