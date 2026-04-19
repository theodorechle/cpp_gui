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

    void StyleManager::applyStyleToElement(element::AbstractElement *element) {
        if (element == nullptr) return;
        element::AbstractElement *actualElement = element;
        const style::StyleValuesMap *styleMap;
        style::RulesMap elementRulesMap = style::RulesMap();
        actualElement->style()->clear();
        for (style::StyleDefinition *styleComponent : style) {
            if (!elementSelectorsCompatibles(actualElement, &styleComponent->first)) continue;

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
        actualElement = actualElement->parent();
        while (actualElement != nullptr) {
            for (style::StyleDefinition *styleComponent : style) {
                if (!elementSelectorsCompatibles(actualElement, &styleComponent->first)) continue;

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
            actualElement = actualElement->parent();
        }

        element->style()->rules(elementRulesMap);

#ifdef DEBUG_DEBUG
        std::clog << "element " << element->debugValue() << " has the following style rules:\n";
        for (std::pair<std::string, style::StyleRule> rule : element->style()->rules()) {
            std::clog << rule.first << ": ";
            rule.second.value->debugDisplay();
        }
#endif
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
        std::clog << "Adding file '" << fileName << "'\n";
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
        // I don't know why, but it already seems to work
        // maybe because the other elements get refreshed and inherits the style? But all style is not inheritable
        applyStyleToElement(_rootElement);
        delete fileRules;
        files[fileCount] = std::pair<std::string, int>("", ruleNumber);
        fileCount++;
        return fileCount - 1;
    }

    void StyleManager::removeStyleInElements(int fileNumber, element::AbstractElement *element) {
#ifdef DEBUG
        std::clog << "Removing style from element\n";
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

    bool StyleManager::elementMatchSelector(element::AbstractElement *element, const style::StyleComponentData &selector) {
        return selector.second == style::StyleComponentType::StarWildcard || element->style()->hasSelector(selector);
    }

    bool StyleManager::elementSelectorsCompatibles(element::AbstractElement *element, const style::StyleComponentDataList *componentsList) {
        if (element == nullptr || componentsList == nullptr) return false;
        style::StyleComponentDataList::const_reverse_iterator componentDataIt = componentsList->crbegin();
        return elementSelectorsCompatibles(componentDataIt, componentsList->crend(), element);
    }

    bool StyleManager::elementSelectorsCompatibles(style::StyleComponentDataList::const_reverse_iterator componentDataIt,
                                                   style::StyleComponentDataList::const_reverse_iterator componentDataListEndIt,
                                                   element::AbstractElement *element) {
        element::AbstractElement *currentElement = element;
        for (style::StyleComponentDataList::const_reverse_iterator it = componentDataIt; it != componentDataListEndIt; it++) {
            switch (it->second) {
            case style::StyleRelation::SameElement:
                if (!elementMatchSelector(currentElement, it->first)) return false;
                break;
            case style::StyleRelation::DirectParent:
                currentElement = currentElement->parent();
                if (!currentElement || !elementMatchSelector(currentElement, it->first)) return false;
                break;
            case style::StyleRelation::AnyParent:
                while (currentElement) {
                    currentElement = currentElement->parent();
                    if (!currentElement) return false;

                    if (!elementMatchSelector(currentElement, it->first)) continue;
                    if (elementSelectorsCompatibles(std::next(it), componentDataListEndIt, currentElement)) return true;
                }
                break;
            default:
                return false;
            }
        }
        return true;
    }
} // namespace gui::elementStyle::manager
