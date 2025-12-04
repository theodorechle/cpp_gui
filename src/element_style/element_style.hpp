#ifndef STYLE_NODE_HPP
#define STYLE_NODE_HPP

#include "../../cpp_commons/src/node.hpp"
#include "../../cpp_style/src/style_component.hpp"
#include <algorithm>
#include <list>
#include <set>
#include <string>
#include <unordered_map>

namespace gui::elementStyle {
    class ElementStyle {
        style::RulesMap _rules = {};
        std::list<std::string> defaultFontsPath = {};
        std::set<style::StyleComponentData> _selectors = {};

        static bool compareRulesPriorityDescending(const style::StyleRule &rule1, const style::StyleRule &rule2); // TODO: take as references
        static bool compareRulesPriorityAscending(const style::StyleRule &rule1, const style::StyleRule &rule2);

    public:
        void rules(const style::RulesMap &newStyle) { _rules = newStyle; }

        void addDefaultFontPath(std::string path);
        void addSelector(const std::string &selectorName, style::StyleComponentType selectorType);
        bool hasSelector(const style::StyleComponentData &selector);

        void updateStylePriorityFromFile(int oldFileNumber, int newFileNumber);
        void clear();

        void activateModifier(const std::string &modifier);
        void deactivateModifier(const std::string &modifier);

        void deleteStyle(int fileNumber, int ruleNumber);
        void deleteStyleFromFile(int fileNumber);

        std::list<std::string> debugToString();

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

// namespace style {

//     namespace elementStyle {

//         class StyleNode : public commons::Node<StyleNode> {
//             StyleRule *findRule(int fileNumber, int ruleNumber);
//             const StyleRule *findRule(int fileNumber, int ruleNumber) const;

//         public:
//             /*
//             Delete first rule who correspond to the file and rule number.
//             Each rule should have a unique number.
//             If not, which one is deleted cannot be guaranted.

//             returns true if successfully deleted, false else
//             */
//             bool deleteStyle(int fileNumber, int ruleNumber);
//             // returns the number of deleted rules
//             int nbRules() const;

//             bool ruleExists(const std::string &ruleName) const;
//             bool ruleExists(int fileNumber, int ruleNumber) const;
//             void toggleRule(int fileNumber, int ruleNumber);
//             void toggleRule(int fileNumber, int ruleNumber, bool enabled);
//         };

//     } // namespace elementStyle
// } // namespace style

#endif // STYLE_NODE_HPP
