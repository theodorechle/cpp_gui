#include "style_manager.hpp"

namespace gui::elementStyle::manager {

    int StyleManager::findFile(const std::string &fileName) {
        for (std::pair<int, std::pair<std::string, int>> file : files) {
            if (file.second.first == fileName) return file.first;
        }
        return -1;
    }

    void StyleManager::updateRulesPrioritiesInElements(int oldFileNumber, int newFileNumber, element::AbstractElement *element) {
        element::AbstractElement *child;
        element->style()->updateStylePriorityFromFile(oldFileNumber, newFileNumber);
        child = element->child();
        while (child != nullptr) {
            updateRulesPrioritiesInElements(oldFileNumber, newFileNumber, child);
            child = child->next();
        }
    }

    void StyleManager::updateRulesPriorities(int fileNumber) {
        std::unordered_map<int, std::pair<std::string, int>>::node_type node;
        if (_rootElement != nullptr) {
            updateRulesPrioritiesInElements(fileNumber, fileCount, _rootElement);
            node = files.extract(fileNumber);
            node.key() = fileCount;
            files.insert(std::move(node));
            fileCount++;
        }
    }

    void StyleManager::applyStyleToElement(element::AbstractElement *elementStyle) {
        if (elementStyle == nullptr) return;
        element::AbstractElement *actualElementStyle = elementStyle;
        const style::StyleValuesMap *styleMap;
        style::RulesMap elementRulesMap = style::RulesMap();
        actualElementStyle->style()->clear();
        for (style::StyleDefinition *styleComponent : style) {
            if (!elementSelectorsCompatibles(actualElementStyle, &styleComponent->first)) continue;

            styleMap = &styleComponent->second;

            for (std::pair<std::string, style::StyleRule> styleRule : *styleMap) {
                style::RulesMap::const_iterator existingRule = elementRulesMap.find(styleRule.first);
                if (existingRule != elementRulesMap.cend() && styleRule.second.specificity <= existingRule->second.specificity) continue;
                // TODO: just need a copy method, no ?
                elementRulesMap[styleRule.first] = style::StyleRule{styleRule.second.value->copy(), true, styleRule.second.specificity,
                                                                    styleRule.second.fileNumber, styleRule.second.ruleNumber};
            }
        }

        // TODO: find more optimized way to get inherited style
        actualElementStyle = actualElementStyle->parent();
        while (actualElementStyle != nullptr) {
            for (style::StyleDefinition *styleComponent : style) {
                if (!elementSelectorsCompatibles(actualElementStyle, &styleComponent->first)) continue;

                styleMap = &styleComponent->second;

                for (std::pair<std::string, style::StyleRule> styleRule : *styleMap) {
                    if (config->inheritableRules.find(styleRule.first) == config->inheritableRules.cend()) continue;
                    style::RulesMap::const_iterator existingRule = elementRulesMap.find(styleRule.first);
                    if (existingRule != elementRulesMap.cend() && styleRule.second.specificity <= existingRule->second.specificity) continue;
                    // TODO: copy method
                    elementRulesMap[styleRule.first] = style::StyleRule{styleRule.second.value->copy(), true, styleRule.second.specificity,
                                                                        styleRule.second.fileNumber, styleRule.second.ruleNumber};
                }
            }
            actualElementStyle = actualElementStyle->parent();
        }

        elementStyle->style()->rules(elementRulesMap);
    }

    StyleManager::~StyleManager() {
        for (style::StyleDefinition *styleDefinition : style) {
            for (std::pair<std::string, style::StyleRule> rule : styleDefinition->second) {
                delete rule.second.value;
            }
            delete styleDefinition;
        }
    }

    void StyleManager::addDefaultFontPath(const std::string &path) { defaultFontsPaths.insert(path); }

    const std::unordered_set<std::string> &StyleManager::getDefaultFontsPaths() const { return defaultFontsPaths; }

    int StyleManager::addStyleFile(const std::string &fileName) {
#ifdef DEBUG
        std::cerr << "Adding file '" << fileName << "'\n";
#endif
        int fileNumber = findFile(fileName);
        if (fileNumber != -1) removeStyle(fileNumber);
        std::ifstream file(fileName);
        std::stringstream buffer;
        if (!file.is_open()) {
            std::cerr << "File '" << fileName << "' couldn't be opened\n";
            return -1;
        }
        buffer << file.rdbuf();
        fileNumber = addStyle(buffer.str());
        files[fileNumber].first = fileName;
        return fileNumber;
    }

    int StyleManager::addStyle(const std::string &styleFileContent) {
        int ruleNumber;
        std::list<style::StyleDefinition *> *fileRules;
        fileRules = style::StyleDeserializer::deserialize(styleFileContent, fileCount, &ruleNumber, config);
        if (fileRules == nullptr || fileRules->empty()) return -1;
        style.splice(style.end(), *fileRules);
        // FIXME: should apply to all elements, not only root
        applyStyleToElement(_rootElement);
        delete fileRules;
        files[fileCount] = std::pair<std::string, int>("", ruleNumber);
        fileCount++;
        return fileCount - 1;
    }

    void StyleManager::removeStyleInElements(int fileNumber, element::AbstractElement *element) {
#ifdef DEBUG
        std::cerr << "Removing style from element\n";
#endif
        element::AbstractElement *child;
        element->style()->deleteStyleFromFile(fileNumber);
        child = element->child();
        while (child != nullptr) {
            removeStyleInElements(fileNumber, child);
            child = child->next();
        }
    }

    void StyleManager::removeStyle(int fileNumber) {
        for (std::pair<int, std::pair<std::string, int>> file : files) {
        }
        if (_rootElement != nullptr && files.find(fileNumber) != files.cend()) {
            removeStyleInElements(fileNumber, _rootElement);
            for (style::StyleDefinition *block : style) {
                style::StyleValuesMap &styleMap = block->second;
                for (style::StyleValuesMap::iterator ruleIt = styleMap.begin(); ruleIt != styleMap.end();) {
                    if (ruleIt->second.fileNumber == fileNumber) ruleIt = styleMap.erase(ruleIt);
                    else ruleIt++;
                }
            }
            files.erase(fileNumber);
        }
    }

    bool StyleManager::elementSelectorsCompatibles(element::AbstractElement *elementStyle, const style::StyleComponentDataList *componentsList) {
        if (elementStyle == nullptr || componentsList == nullptr) return false;
        style::StyleComponentDataList::const_reverse_iterator componentDataIt = componentsList->crbegin();
        return elementSelectorsCompatiblesLoop(componentDataIt, componentsList->crend(), elementStyle);
    }

    bool StyleManager::elementSelectorsCompatiblesLoop(style::StyleComponentDataList::const_reverse_iterator componentDataIt,
                                                       style::StyleComponentDataList::const_reverse_iterator componentDataListEndIt,
                                                       element::AbstractElement *elementStyle) {
        element::AbstractElement *element = elementStyle;
        for (style::StyleComponentDataList::const_reverse_iterator it = componentDataIt; it != componentDataListEndIt; it++) {
            if (it->first.second == style::StyleComponentType::StarWildcard) {
                continue;
            }
            switch (it->second) {
            case style::StyleRelation::SameElement:
                if (!element->style()->hasSelector(it->first)) return false;
                break;
            case style::StyleRelation::DirectParent:
                element = element->parent();
                if (!element || !element->style()->hasSelector(it->first)) return false;
                break;
            case style::StyleRelation::AnyParent:
                while (element) {
                    element = element->parent();
                    if (!element) return false;
                    if (element->style()->hasSelector(it->first)) break; // found, go back to for-loop
                    style::StyleComponentDataList::const_reverse_iterator nextIt = std::next(it);
                    if (nextIt == componentDataListEndIt || !elementSelectorsCompatiblesLoop(nextIt, componentDataListEndIt, element)) {
                        return false;
                    }
                }
                break;
            default:
                return false;
            }
        }
        return true;
    }

    void StyleManager::addElementStyle(element::AbstractElement *elementStyle) {
        if (_rootElement == nullptr) rootElement(elementStyle);
        applyStyleToElement(elementStyle);
    }

} // namespace gui::elementStyle::manager
