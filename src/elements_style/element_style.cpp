#include "element_style.hpp"

namespace gui {
    namespace elementStyle {

        bool ElementStyle::compareRulesLess(style::StyleRule rule1, style::StyleRule rule2) {
            if (rule1.priority > rule2.priority) return true;
            if (rule1.priority == rule2.priority) {
                if (rule1.fileNumber > rule2.fileNumber) return true;
                if (rule1.fileNumber == rule2.fileNumber) {
                    if (rule1.ruleNumber > rule2.ruleNumber) return true;
                }
            }
            return false;
        }

        ElementStyle::~ElementStyle() {}

        void ElementStyle::addChild(ElementStyle *child) {
            ElementStyle *nextChild = getChild();
            ElementStyle *selfChild;
            if (nextChild == nullptr) {
                this->child = child;
            }
            else {
                do {
                    selfChild = nextChild;
                    nextChild = selfChild->getNext();
                } while (nextChild != nullptr);
                selfChild->setNext(child);
            }
            if (child == nullptr) return;
            child->setParent(this);
        }

        void ElementStyle::addStyle(AppliedStyleMap &newStyle) {
            AppliedStyleMap::iterator actualStyleIt;
            for (AppliedStyleMap::iterator it = newStyle.begin(); it != newStyle.end(); it++) {
                actualStyleIt = style.find(it->first);
                if (actualStyleIt == style.cend()) { // not found in existing element's style or new style has bigger priority
                    style[it->first] = it->second;
                }
                // same priority, more recent file or same file and more recent rule
                else {
                    actualStyleIt->second.splice(actualStyleIt->second.end(), it->second);
                    actualStyleIt->second.sort(compareRulesLess);
                }
            }
        }

        bool ElementStyle::deleteStyle(int fileNumber, int ruleNumber) {
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                        it->second.erase(listIt);
                        return true; // should not have multiple rules with same file and rule number
                    }
                }
            }
            return false;
        }

        int ElementStyle::deleteStyleFromFile(int fileNumber) {
            int nbDeletedRules = 0;
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == fileNumber) {
                        it->second.erase(listIt);
                        nbDeletedRules++;
                    }
                }
            }
            return nbDeletedRules;
        }

        void ElementStyle::updateStylePriorityFromFile(int oldFileNumber, int newFileNumber) {
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == oldFileNumber) {
                        listIt->fileNumber = newFileNumber;
                    }
                }
                it->second.sort(compareRulesLess);
            }
        }

        int ElementStyle::clear() {
            int nbDeletedRules = 0;
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                nbDeletedRules += it->second.size();
                it->second.clear();
            }
            style.clear();
            return nbDeletedRules;
        }

        bool ElementStyle::getRule(const std::string &ruleName, style::StyleValue **ruleValue, style::StyleValue *defaultStyle) const {
            for (AppliedStyleMap::const_iterator it = style.cbegin(); it != style.cend(); it++) {
                if (it->first == ruleName) {
                    for (StyleRules::const_iterator listIt = it->second.cbegin(); listIt != it->second.cend(); listIt++) { // find first enabled rule
                        if (listIt->enabled) {
                            *ruleValue = listIt->value;
                            return true;
                        }
                    }
                }
            }
            if (parent == nullptr) {
                *ruleValue = defaultStyle;
                return false;
            }
            return parent->getRule(ruleName, ruleValue, defaultStyle); // cascade style, if not found, check parent
        }

        bool ElementStyle::ruleExists(const std::string &ruleName) const {
            for (AppliedStyleMap::const_iterator it = style.cbegin(); it != style.cend(); it++) {
                if (it->first == ruleName) return true;
            }
            return false;
        }

        bool ElementStyle::ruleExists(int fileNumber, int ruleNumber) const {
            for (AppliedStyleMap::const_iterator it = style.cbegin(); it != style.cend(); it++) {
                for (StyleRules::const_iterator listIt = it->second.cbegin(); listIt != it->second.cend(); listIt++) {
                    if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                        return true;
                    }
                }
            }
            return false;
        }

        void ElementStyle::addSelector(std::string selectorName, style::StyleComponentType selectorType) {
            selectors.insert(style::StyleComponentData(selectorName, selectorType));
        }

        void ElementStyle::addModifier(std::string modifierName) {
            modifiers.emplace(modifierName, std::pair<bool, std::list<std::pair<int, int>>>(false, {}));
        }

        void ElementStyle::toggleModifier(std::string modifierName, bool enabled) {
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>>::iterator modifier;
            modifier = modifiers.find(modifierName);
            if (modifier == modifiers.end() || modifier->second.first == enabled) return;
            modifier->second.first = enabled;
            for (std::pair<int, int> ruleIds : modifier->second.second) {
                toggleRule(ruleIds.first, ruleIds.second, enabled);
            }
        }

        void ElementStyle::deactivateAllModifiers() {
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>>::const_iterator modifiersIt;
            for (modifiersIt = modifiers.cbegin(); modifiersIt != modifiers.cend(); modifiersIt++) {
                toggleModifier(modifiersIt->first, false);
            }
        }

        void ElementStyle::addRuleAffectedByModifier(int fileNumber, int ruleNumber, std::string modifierName) {
            std::unordered_map<std::string, std::pair<bool, std::list<std::pair<int, int>>>>::iterator modifier;
            modifier = modifiers.find(modifierName);
            if (modifier == modifiers.end()) return;
            if (!ruleExists(fileNumber, ruleNumber)) return;
            modifier->second.second.push_back(std::pair<int, int>(fileNumber, ruleNumber));
        }

        void ElementStyle::toggleRule(int fileNumber, int ruleNumber) {
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                        listIt->enabled = !listIt->enabled;
                    }
                }
            }
        }

        void ElementStyle::toggleRule(int fileNumber, int ruleNumber, bool enabled) {
            for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
                for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
                    if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                        listIt->enabled = enabled;
                    }
                }
            }
        }

    } // namespace elementStyle
} // namespace gui
