#ifndef ELEMENT_STYLE_HPP
#define ELEMENT_STYLE_HPP

#include "../style/style_component.hpp"
#include <algorithm>
#include <set>
#include <unordered_map>

namespace gui {
    namespace elementStyle {

        typedef std::list<style::StyleRule> StyleRules;
        typedef std::unordered_map<std::string, StyleRules> AppliedStyleMap;

        class ElementStyle {
            AppliedStyleMap style = {};
            /**
             * key:
             *  modifier name
             * value:
             *  isActive
             *  list: // all rules who are affected
             *   first: fileNumber
             *   second: ruleNumber
             */
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>> modifiers = {};
            std::set<style::StyleComponentData> selectors = {};
            ElementStyle *parent = nullptr;
            ElementStyle *child = nullptr;
            ElementStyle *next = nullptr;

            static bool compareRulesPriorityDescending(style::StyleRule rule1, style::StyleRule rule2);
            static bool compareRulesPriorityAscending(style::StyleRule rule1, style::StyleRule rule2);
            void setParent(ElementStyle *parent) { this->parent = parent; }

        public:
            virtual ~ElementStyle();
            ElementStyle *getParent() { return parent; }
            void addChild(ElementStyle *child);
            ElementStyle *getChild() { return child; }
            void setNext(ElementStyle *next) { this->next = next; }
            ElementStyle *getNext() { return next; }
            void setStyle(const AppliedStyleMap &style) { this->style = style; }
            void addStyle(AppliedStyleMap &newStyle);
            const AppliedStyleMap &getStyle() const;
            // returns true if successfully deleted, false else
            bool deleteStyle(int fileNumber, int ruleNumber);
            // returns the number of deleted rules
            int deleteStyleFromFile(int fileNumber);
            void updateStylePriorityFromFile(int oldFileNumber, int newFileNumber);
            int clear();
            int nbRules() const;
            /**
             * Set the value in the ruleValue parameter.
             * Returns true if found or default value is returned.
             * If canInherit is true and rule is not found, tries to search in parents style.
             * If no defaultStyle is given, returns false if no value were found.
             */
            bool getRule(const std::string &ruleName, style::StyleValue **ruleValue, bool canInherit = false,
                         style::StyleValue *defaultStyle = nullptr) const;
            bool getRule(const std::vector<std::string> &rulesNames, style::StyleValue **ruleValue, bool canInherit = false,
                                       style::StyleValue *defaultStyle = nullptr) const;
            bool ruleExists(const std::string &ruleName) const;
            bool ruleExists(int fileNumber, int ruleNumber) const;
            const std::set<style::StyleComponentData> *getSelectors() { return &selectors; }
            void addSelector(std::string selectorName, style::StyleComponentType selectorType);
            void addModifier(std::string modifierName);
            void setModifierState(std::string modifierName, bool enabled);
            void deactivateAllModifiers();
            void addRuleAffectedByModifier(int fileNumber, int ruleNumber, std::string modifierName);
            void toggleRule(int fileNumber, int ruleNumber);
            void toggleRule(int fileNumber, int ruleNumber, bool enabled);
        };

    } // namespace elementStyle
} // namespace gui

#endif // ELEMENT_STYLE_HPP
