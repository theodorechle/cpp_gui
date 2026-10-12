#include "element_style.hpp"
#include <cppcommons/node.hpp>
#include <sstream>
#include <string>

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

    ElementStyle::~ElementStyle() { delete _rules; }

    void ElementStyle::rules(style::RulesMap *newStyle) {
        delete _rules;
        _rules = newStyle;
    }

    void ElementStyle::addSelector(const std::string &selectorName, style::SelectorType selectorType) {
        _selectors.insert(style::SelectorData(selectorName, selectorType));
    }

    void ElementStyle::removeSelector(const std::string &selectorName, style::SelectorType selectorType) {
        _selectors.erase(style::SelectorData(selectorName, selectorType));
    }

    bool ElementStyle::hasSelector(const style::SelectorData &selector) const { return _selectors.find(selector) != _selectors.cend(); }

    void ElementStyle::updateStylePriorityFromFile(int oldFileNumber, int newFileNumber) {
        for (style::RulesMap::iterator it = _rules->begin(); it != _rules->end(); it++) {
            if (it->second.fileNumber == oldFileNumber) {
                it->second.fileNumber = newFileNumber;
            }
        }
    }

    void ElementStyle::clear() { _rules->clear(); }

    void ElementStyle::activateModifier(const std::string &modifier) { _selectors.insert({modifier, style::SelectorType::Modifier}); }

    void ElementStyle::deactivateModifier(const std::string &modifier) { _selectors.erase({modifier, style::SelectorType::Modifier}); }

    void ElementStyle::deleteStyle(int fileNumber, int ruleNumber) {
        for (style::RulesMap::iterator it = _rules->begin(); it != _rules->end(); it++) {
            if (it->second.fileNumber == fileNumber && it->second.ruleNumber == ruleNumber) {
                _rules->erase(it->first);
                return;
            }
        }
    }

    void ElementStyle::deleteStyleFromFile(int fileNumber) {
        for (style::RulesMap::iterator it = _rules->begin(); it != _rules->end(); it++) {
            if (it->second.fileNumber == fileNumber) {
                _rules->erase(it->first);
                return;
            }
        }
    }

    std::list<std::string> ElementStyle::debugToString() const {
        std::list<std::string> pathFragment = {};
        for (const style::SelectorData &selector : _selectors) {
            char firstChar[2] = "";
            switch (selector.second) {
            case style::SelectorType::Identifier:
                firstChar[0] = '#';
                break;
            case style::SelectorType::Class:
                firstChar[0] = '.';
                break;
            case style::SelectorType::Modifier:
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
        style::RulesMap::const_iterator rule = _rules->find(ruleName);
        if (rule != nullptr && rule->second.enabled) {
            *ruleValue = rule->second.value->copy();
            return true;
        }
        *ruleValue = defaultStyle;
        return (defaultStyle != nullptr);
    }

    bool ElementStyle::getRule(const std::vector<std::string> &ruleNames, style::StyleValue **ruleValue, style::StyleValue *defaultStyle) const {
        const style::StyleRule *currentRule = nullptr;
        for (const std::string &ruleName : ruleNames) {
            style::RulesMap::const_iterator rule = _rules->find(ruleName);
            if (rule != nullptr && rule->second.enabled) {
                if (currentRule == nullptr || compareRulesPriorityAscending(*currentRule, rule->second)) currentRule = &(rule->second);
            }
        }
        if (currentRule != nullptr) {
            *ruleValue = currentRule->value->copy();
            return true;
        }
        *ruleValue = defaultStyle;
        return (defaultStyle != nullptr);
    }

    size_t ElementStyle::nbRules() const { return _rules->size(); }

    std::string ElementStyle::debugValue() {
        std::stringstream stringStream;
        bool first = true;
        for (style::SelectorData selector : _selectors) {
            if (!first) stringStream << ",";
            first = false;
            stringStream << selector.first << "(" << style::selectorTypeToString(selector.second) << ")";
        }
        return stringStream.str();
    }

} // namespace gui::elementStyle
