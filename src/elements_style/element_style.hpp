#ifndef ELEMENT_STYLE_HPP
#define ELEMENT_STYLE_HPP

#include "../style/style_component.hpp"
#include <algorithm>
#include <set>
#include <unordered_map>

typedef std::list<StyleRule> StyleRules;
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
    std::set<StyleComponentData> selectors = {};
    ElementStyle *parent = nullptr;
    ElementStyle *child = nullptr;
    ElementStyle *next = nullptr;
    int priority = 0;

    static bool compareRulesLess(StyleRule rule1, StyleRule rule2);
    void setParent(ElementStyle *parent) { this->parent = parent; }

public:
    ElementStyle() { updatePriority(); }
    virtual ~ElementStyle();
    void updatePriority() { this->priority = (parent == nullptr) ? 0 : parent->getPriority() + 1; }
    int getPriority() { return priority; }
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
    /**
     * Set the value in the ruleValue parameter.
     * Returns true if found.
     * If no defaultStyle is given, returns false if no value were found.
     * Else, returns fase if the defaultStyle is returned (can be used in case of dynamically allocated defaultValue to know when it should be
     * deleted).
     */
    bool getRule(const std::string &ruleName, StyleValue **ruleValue, StyleValue *defaultStyle = nullptr) const;
    bool ruleExists(const std::string &ruleName) const;
    bool ruleExists(int fileNumber, int ruleNumber) const;
    const std::set<StyleComponentData> *getSelectors() { return &selectors; }
    void addSelector(std::string selectorName, StyleComponentType selectorType);
    void addModifier(std::string modifierName);
    void addRuleAffectedByModifier(int fileNumber, int ruleNumber, std::string modifierName);
    void toggleRule(int fileNumber, int ruleNumber);
    void toggleRule(int fileNumber, int ruleNumber, bool isEnabled);
};

#endif // ELEMENT_STYLE_HPP
