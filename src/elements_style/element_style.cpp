#include "element_style.hpp"

bool ElementStyle::compareRulesLess(StyleRule rule1, StyleRule rule2) {
    if (rule1.priority < rule2.priority) return true;
    if (rule1.priority == rule2.priority) {
        if (rule1.fileNumber < rule2.fileNumber) return true;
        if (rule1.fileNumber == rule2.fileNumber) {
            if (rule1.ruleNumber <= rule2.ruleNumber) return true;
        }
    }
    return false;
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
                style.erase(it);
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
                style.erase(it);
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

StyleValue *ElementStyle::getRule(const std::string &ruleName, StyleValue *defaultValue) {
    for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
        if (it->first == ruleName) {
            for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) { // find first enabled rule
                if (listIt->isEnabled) return listIt->value;
            }
        }
    }
    if (parent == nullptr) return defaultValue;
    return parent->getRule(ruleName, defaultValue); // cascade style, if not found, check parent
}

bool ElementStyle::ruleExists(const std::string &ruleName) {
    for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
        if (it->first == ruleName) return true;
    }
    return false;
}

bool ElementStyle::ruleExists(int fileNumber, int ruleNumber) {
    for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
        for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
            if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                return true;
            }
        }
    }
    return false;
}

void ElementStyle::addSelector(std::string selectorName, StyleComponentType selectorType) {
    selectors.insert(StyleComponentData(selectorName, selectorType));
}

void ElementStyle::addModifier(std::string modifierName) {
    modifiers.emplace(modifierName, std::pair<bool, std::list<std::pair<int, int>>>(false, {}));
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
                listIt->isEnabled = !listIt->isEnabled;
            }
        }
    }
}

void ElementStyle::toggleRule(int fileNumber, int ruleNumber, bool isEnabled) {
    for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
        for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
            if (listIt->fileNumber == fileNumber && listIt->ruleNumber == ruleNumber) {
                listIt->isEnabled = isEnabled;
            }
        }
    }
}