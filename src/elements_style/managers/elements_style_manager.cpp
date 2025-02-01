#include "elements_style_manager.hpp"

namespace gui {
    namespace elementStyle {
        namespace manager {

            int ElementsStyleManager::findFile(const std::string &fileName) {
                for (std::pair<int, std::pair<std::string, int>> file : files) {
                    if (file.second.first == fileName) return file.first;
                }
                return -1;
            }

            void ElementsStyleManager::updateRulesPrioritiesInElements(int oldFileNumber, int newFileNumber,
                                                                       gui::elementStyle::ElementStyle *element) {
                gui::elementStyle::ElementStyle *child;
                element->updateStylePriorityFromFile(oldFileNumber, newFileNumber);
                child = element->getChild();
                while (child != nullptr) {
                    updateRulesPrioritiesInElements(oldFileNumber, newFileNumber, child);
                    child = child->getNext();
                }
            }

            void ElementsStyleManager::updateRulesPriorities(int fileNumber) {
                std::unordered_map<int, std::pair<std::string, int>>::node_type node;
                if (elements != nullptr) {
                    updateRulesPrioritiesInElements(fileNumber, fileCount, elements);
                    node = files.extract(fileNumber);
                    node.key() = fileCount;
                    files.insert(std::move(node));
                    fileCount++;
                }
            }

            void ElementsStyleManager::applySpecificStyleToElement(std::list<style::StyleBlock *> specificStyle,
                                                                   gui::elementStyle::ElementStyle *elementStyle, bool recursive) {
                if (elementStyle == nullptr) return;
                gui::elementStyle::ElementStyle *actualElementStyle = elementStyle;
                const style::StyleValuesMap *styleMap;
                AppliedStyleMap elementStyleMap;
                std::string modifier = "";
                actualElementStyle->clear();
                for (style::StyleBlock *styleComponent : specificStyle) {

                    const style::StyleComponentDataList *componentsList = styleComponent->getComponentsList();

                    if (!areElementSelectorsCompatibles(actualElementStyle, componentsList)) continue;
                    styleMap = styleComponent->getStyleMap();
                    elementStyleMap = AppliedStyleMap();

                    if (componentsList->back().first.second == style::StyleComponentType::Modifier) {
                        modifier = componentsList->back().first.first;
                        actualElementStyle->addModifier(modifier);
                    }
                    for (std::pair<std::string, style::StyleRule> styleRule : *styleMap) {
                        elementStyleMap[styleRule.first] = {style::StyleRule{styleRule.second.value, modifier.empty(), styleRule.second.specificity,
                                                                             styleRule.second.fileNumber, styleRule.second.ruleNumber}};
                        if (!modifier.empty()) {
                            actualElementStyle->addRuleAffectedByModifier(styleRule.second.fileNumber, styleRule.second.ruleNumber, modifier);
                        }
                    }

                    actualElementStyle->addStyle(elementStyleMap);
                }

                if (recursive) {
                    actualElementStyle = actualElementStyle->getChild();
                    while (actualElementStyle != nullptr) {
                        applySpecificStyleToElement(specificStyle, actualElementStyle, recursive);
                        actualElementStyle = actualElementStyle->getNext();
                    }
                }
            }

            ElementsStyleManager::~ElementsStyleManager() {
                for (style::StyleBlock *styleBlock : style) {
                    delete styleBlock;
                }
            }

            int ElementsStyleManager::addStyleFile(const std::string &fileName) {
                int fileNumber = findFile(fileName);
                if (fileNumber != -1) {
                    updateRulesPriorities(fileNumber);
                    return fileNumber;
                }
                std::ifstream file(fileName);
                std::stringstream buffer;
                if (!file.is_open()) {
                    std::cerr << "File '" << fileName << "' couldn't be opened\n";
                    return -1;
                }
                buffer << file.rdbuf();
                return addStyle(buffer.str());
            }

            int ElementsStyleManager::addStyle(const std::string &styleFileContent) {
                int ruleNumber;
                std::list<style::StyleBlock *> *fileRules;
                fileRules = style::StyleDeserializer::deserialize(styleFileContent, fileCount, &ruleNumber);
                if (fileRules == nullptr || fileRules->empty()) return -1;
                applySpecificStyleToElement(*fileRules, elements, true);
                style.splice(style.end(), *fileRules);
                delete fileRules;
                files[fileCount] = std::pair<std::string, int>("", ruleNumber);
                fileCount++;
                return fileCount;
            }

            void ElementsStyleManager::removeStyleInElements(int fileNumber, gui::elementStyle::ElementStyle *element) {
                gui::elementStyle::ElementStyle *child;
                element->deleteStyleFromFile(fileNumber);
                child = element->getChild();
                while (child != nullptr) {
                    removeStyleInElements(fileNumber, child);
                    child = child->getNext();
                }
            }

            void ElementsStyleManager::removeStyle(int fileNumber) {
                if (elements != nullptr && files.find(fileNumber) != files.end()) {
                    removeStyleInElements(fileNumber, elements);
                    files.erase(fileNumber);
                }
            }

            bool ElementsStyleManager::areElementSelectorsCompatibles(gui::elementStyle::ElementStyle *elementStyle,
                                                                      const style::StyleComponentDataList *componentsList) {
                if (elementStyle == nullptr || componentsList == nullptr) return false;
                bool selectorExists = false;
                gui::elementStyle::ElementStyle *currentStyle = elementStyle;
                style::StyleComponentDataList::const_reverse_iterator listEndIt = componentsList->crbegin();
                const std::set<style::StyleComponentData> *elementSelectors;
                elementSelectors = currentStyle->getSelectors();
                if (listEndIt->first.second == style::StyleComponentType::Modifier) {
                    listEndIt++;
                }
                for (style::StyleComponentDataList::const_reverse_iterator it = listEndIt; it != componentsList->crend(); it++) {
                    switch (it->second) {
                    case style::StyleRelation::SameElement:
                        selectorExists = (elementSelectors->find(it->first) != elementSelectors->cend());
                        break;
                    case style::StyleRelation::DirectParent:
                        currentStyle = currentStyle->getParent();
                        if (currentStyle == nullptr) {
                            selectorExists = false;
                            break;
                        }
                        elementSelectors = currentStyle->getSelectors();
                        selectorExists = (elementSelectors->find(it->first) != elementSelectors->cend());
                        break;
                    case style::StyleRelation::AnyParent:
                        while (currentStyle != nullptr) { // FIXME: do the next steps with all compatible parents. The first found can be good for
                                                          // this step but not the next, but an other could work
                            currentStyle = currentStyle->getParent();
                            if (currentStyle == nullptr) {
                                selectorExists = false;
                                break; // TODO: better handling
                            }
                            elementSelectors = currentStyle->getSelectors();
                            selectorExists = (elementSelectors->find(it->first) != elementSelectors->cend());
                            if (selectorExists) break;
                        }
                        break;
                    default:
                        selectorExists = false;
                        break;
                    }
                    if (!selectorExists) break;
                }
                return selectorExists;
            }

            void ElementsStyleManager::applyStyleToElement(gui::elementStyle::ElementStyle *elementStyle, bool recursive) {
                applySpecificStyleToElement(style, elementStyle, recursive);
            }

            void ElementsStyleManager::addElementStyle(gui::elementStyle::ElementStyle *elementStyle) {
                if (elements == nullptr) elements = elementStyle;
                applyStyleToElement(elementStyle, true);
            }

        } // namespace manager
    } // namespace elementStyle
} // namespace gui
