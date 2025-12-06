#ifndef STYLE_NODE_HPP
#define STYLE_NODE_HPP

#include "../../cpp_commons/src/node.hpp"
#include "../../cpp_style/src/style_component.hpp"
#include <set>
#include <string>
#include <unordered_set>

namespace gui::elementStyle {
    class ElementStyle {
        style::RulesMap _rules = {};
        std::set<style::StyleComponentData> _selectors = {};

        static bool compareRulesPriorityDescending(const style::StyleRule &rule1, const style::StyleRule &rule2);
        static bool compareRulesPriorityAscending(const style::StyleRule &rule1, const style::StyleRule &rule2);

    public:
        void rules(const style::RulesMap &newStyle) { _rules = newStyle; }

        void addSelector(const std::string &selectorName, style::StyleComponentType selectorType);
        bool hasSelector(const style::StyleComponentData &selector) const;

        void updateStylePriorityFromFile(int oldFileNumber, int newFileNumber);
        void clear();

        void activateModifier(const std::string &modifier);
        void deactivateModifier(const std::string &modifier);

        void deleteStyle(int fileNumber, int ruleNumber);
        void deleteStyleFromFile(int fileNumber);

        std::list<std::string> debugToString() const;

        const style::RulesMap &rules() { return _rules; }

        /**
         * Set the value in the ruleValue parameter.
         * Returns true if found or default value is returned.
         * If no defaultStyle is given, returns false if no value were found.
         */
        bool getRule(const std::string &ruleName, style::StyleValue **ruleValue, style::StyleValue *defaultStyle = nullptr) const;
        /**
         * Set the value in the ruleValue parameter.
         * Returns true if found or default value is returned.
         * If no defaultStyle is given, returns false if no value were found.
         *
         * Return the value with biggest specificity among all matching rules.
         * If two values have same specificity, first in the rulesNames list will be prioritized.
         */
        bool getRule(const std::vector<std::string> &ruleNames, style::StyleValue **ruleValue, style::StyleValue *defaultStyle = nullptr) const;

        int nbRules() const;
    };
} // namespace gui::elementStyle

#endif // STYLE_NODE_HPP
