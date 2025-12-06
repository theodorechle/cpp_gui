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
        if (rootElement != nullptr) {
            updateRulesPrioritiesInElements(fileNumber, fileCount, rootElement);
            node = files.extract(fileNumber);
            node.key() = fileCount;
            files.insert(std::move(node));
            fileCount++;
        }
    }

    void StyleManager::applySpecificStyleToElement(std::list<style::StyleBlock *> specificStyle, element::AbstractElement *elementStyle,
                                                   bool recursive) {
        if (elementStyle == nullptr) return;
        element::AbstractElement *actualElementStyle = elementStyle;
        const style::StyleValuesMap *styleMap;
        style::RulesMap elementRulesMap;
        actualElementStyle->style()->clear();
        for (style::StyleBlock *styleComponent : specificStyle) {
            const style::StyleComponentDataList *componentsList = styleComponent->getComponentsList();

            if (!elementSelectorsCompatibles(actualElementStyle, componentsList)) continue;

            styleMap = styleComponent->getStyleMap();
            elementRulesMap = style::RulesMap();

            // TODO: add parent style
            // TODO: add config for inheritance of rules
            for (std::pair<std::string, style::StyleRule> styleRule : *styleMap) {
                elementRulesMap[styleRule.first] = style::StyleRule{styleRule.second.value->copy(), true, styleRule.second.specificity,
                                                                    styleRule.second.fileNumber, styleRule.second.ruleNumber};
            }
            actualElementStyle->style()->rules(elementRulesMap);
        }

        if (recursive) {
            actualElementStyle = actualElementStyle->child();
            while (actualElementStyle != nullptr) {
                applySpecificStyleToElement(specificStyle, actualElementStyle, recursive);
                actualElementStyle = actualElementStyle->next();
            }
        }
    }

    StyleManager::~StyleManager() {
        for (style::StyleBlock *styleBlock : style) {
            delete styleBlock;
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
        std::list<style::StyleBlock *> *fileRules;
        fileRules = style::StyleDeserializer::deserialize(styleFileContent, fileCount, &ruleNumber, config);
        if (fileRules == nullptr || fileRules->empty()) return -1;
        style.splice(style.end(), *fileRules);
        applySpecificStyleToElement(style, rootElement, true);
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
        if (rootElement != nullptr && files.find(fileNumber) != files.cend()) {
            removeStyleInElements(fileNumber, rootElement);
            for (style::StyleBlock *block : style) {
                style::StyleValuesMap *styleMap = block->getStyleMap();
                for (style::StyleValuesMap::iterator ruleIt = styleMap->begin(); ruleIt != styleMap->end();) {
                    if (ruleIt->second.fileNumber == fileNumber) ruleIt = styleMap->erase(ruleIt);
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
                if (!element) return false;
                if (!element->style()->hasSelector(it->first)) return false;
                break;
            case style::StyleRelation::AnyParent:
                while (element) {
                    element = element->parent();
                    if (!element) return false;

                    if (element->style()->hasSelector(it->first)) continue;
                    style::StyleComponentDataList::const_reverse_iterator nextIt = std::next(it);
                    if (nextIt == componentDataListEndIt || elementSelectorsCompatiblesLoop(nextIt, componentDataListEndIt, element)) {
                        return true;
                    }
                }
                return false;
            default:
                return false;
            }
        }
        return true;
    }

    void StyleManager::applyStyleToElement(element::AbstractElement *elementStyle, bool recursive) {
        applySpecificStyleToElement(style, elementStyle, recursive);
    }

    void StyleManager::addElementStyle(element::AbstractElement *elementStyle) {
        if (rootElement == nullptr) setParentElementStyle(elementStyle);
        applyStyleToElement(elementStyle, true);
    }

} // namespace gui::elementStyle::manager
