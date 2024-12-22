#include "element_style.hpp"

bool ElementStyle::compareRulesLess(std::tuple<StyleValue *, int, int, int> rule1, std::tuple<StyleValue *, int, int, int> rule2) {
    if (std::get<1>(rule1) < std::get<1>(rule2)) return true; // compare priority
    if (std::get<1>(rule1) == std::get<1>(rule2)) {
        if (std::get<2>(rule1) < std::get<2>(rule2)) return true; // compare file number
        if (std::get<2>(rule1) == std::get<2>(rule2)) {
            if (std::get<3>(rule1) <= std::get<3>(rule2)) return true; // compare rule number
        }
    }
    return false;
}

void ElementStyle::addStyle(AppliedStyleMap &newStyle) {
    AppliedStyleMap::iterator actualStyleIt;
    for (AppliedStyleMap::iterator it = newStyle.begin(); it != newStyle.end(); it++) {
        actualStyleIt = style.find(it->first);
        if (actualStyleIt == style.cend()) { // not found in existing element's style or new style has bigger priority
            actualStyleIt->second = it->second;
        }
        // same priority, more recent file or same file and more recent rule
        else {
            actualStyleIt->second.merge(it->second);
            actualStyleIt->second.sort(compareRulesLess);
        }
    }
}

bool ElementStyle::deleteStyle(int fileNumber, int ruleNumber) {
    for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
        for (StyleRules::iterator listIt = it->second.begin(); listIt != it->second.end(); listIt++) {
            if (std::get<2>(*listIt) == fileNumber && std::get<3>(*listIt) == ruleNumber) {
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
            if (std::get<1>(*listIt) == fileNumber) {
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
            if (std::get<1>(*listIt) == oldFileNumber) {
                std::get<1>(*listIt) = newFileNumber;
            }
        }
        it->second.sort(compareRulesLess);
    }
}

StyleRule *ElementStyle::getRule(const std::string &ruleName) {
    for (AppliedStyleMap::iterator it = style.begin(); it != style.end(); it++) {
        if (it->first == ruleName) return &(it->second.front());
    }
    return nullptr;
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
            if (std::get<2>(*listIt) == fileNumber && std::get<3>(*listIt) == ruleNumber) {
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
