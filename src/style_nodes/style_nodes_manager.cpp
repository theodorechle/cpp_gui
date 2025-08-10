#include "style_nodes_manager.hpp"

namespace gui {
    namespace elementStyle {
        namespace manager {

            int StyleNodesManager::findFile(const std::string &fileName) {
                for (std::pair<int, std::pair<std::string, int>> file : files) {
                    if (file.second.first == fileName) return file.first;
                }
                return -1;
            }

            void StyleNodesManager::updateRulesPrioritiesInElements(int oldFileNumber, int newFileNumber, gui::elementStyle::StyleNode *element) {
                gui::elementStyle::StyleNode *child;
                element->updateStylePriorityFromFile(oldFileNumber, newFileNumber);
                child = element->getChild();
                while (child != nullptr) {
                    updateRulesPrioritiesInElements(oldFileNumber, newFileNumber, child);
                    child = child->getNext();
                }
            }

            void StyleNodesManager::updateRulesPriorities(int fileNumber) {
                std::unordered_map<int, std::pair<std::string, int>>::node_type node;
                if (elements != nullptr) {
                    updateRulesPrioritiesInElements(fileNumber, fileCount, elements);
                    node = files.extract(fileNumber);
                    node.key() = fileCount;
                    files.insert(std::move(node));
                    fileCount++;
                }
            }

            void StyleNodesManager::applySpecificStyleToElement(std::list<style::StyleBlock *> specificStyle,
                                                                gui::elementStyle::StyleNode *elementStyle, bool recursive) {
                if (elementStyle == nullptr) return;
                // std::cerr << "Applying style to element:\n";
                // std::cerr << "selectors: ";
                // for (style::StyleComponentData selector : *(elementStyle->getSelectors())) {
                //     std::cerr << selector.first << ", ";
                // }
                // std::cerr << "\n";
                gui::elementStyle::StyleNode *actualElementStyle = elementStyle;
                const style::StyleValuesMap *styleMap;
                AppliedStyleMap elementStyleMap;
                std::string modifier = "";
                actualElementStyle->clear();
                for (style::StyleBlock *styleComponent : specificStyle) {
                    modifier = "";
                    const style::StyleComponentDataList *componentsList = styleComponent->getComponentsList();

                    if (!areElementSelectorsCompatibles(actualElementStyle, componentsList)) continue;

                    styleMap = styleComponent->getStyleMap();
                    elementStyleMap = AppliedStyleMap();

                    if (componentsList->back().first.second == style::StyleComponentType::Modifier) {
                        modifier = componentsList->back().first.first;
                        actualElementStyle->addModifier(modifier);
                    }

                    for (std::pair<std::string, style::StyleRule> styleRule : *styleMap) {
                        elementStyleMap[styleRule.first] = {style::StyleRule{styleRule.second.value->copy(), modifier.empty(), // disable if affected by modifier should be done by elementStyle
                                                                             styleRule.second.specificity, styleRule.second.fileNumber,
                                                                             styleRule.second.ruleNumber}};
                        if (!modifier.empty()) {
                            actualElementStyle->addRuleAffectedByModifier(styleRule.second.fileNumber, styleRule.second.ruleNumber, modifier);
                        }
                    }
                    // std::cerr << "applied style: ";
                    // for (std::pair<std::string, StyleRules> appliedStyle : elementStyleMap) {
                    //     std::cerr << appliedStyle.first << ", ";
                    // }
                    // std::cerr << "\n";
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

            StyleNodesManager::~StyleNodesManager() {
                for (style::StyleBlock *styleBlock : style) {
                    delete styleBlock;
                }
            }

            void StyleNodesManager::setFontsPath(const std::string &path) {
                if (path[path.size() - 1] != '/') fontsPath = path + '/';
                else fontsPath = path;
            }

            const std::string &StyleNodesManager::getFontsPath() const { return fontsPath; }

            int StyleNodesManager::addStyleFile(const std::string &fileName) {
                std::cerr << "Adding file '" << fileName << "'\n";
                int fileNumber = findFile(fileName); //  TODO: allow force reload
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
                fileNumber = addStyle(buffer.str());
                files[fileNumber].first = fileName;
                return fileNumber;
            }

            int StyleNodesManager::addStyle(const std::string &styleFileContent) {
                int ruleNumber;
                std::list<style::StyleBlock *> *fileRules;
                fileRules = style::StyleDeserializer::deserialize(styleFileContent, fileCount, &ruleNumber);
                if (fileRules == nullptr || fileRules->empty()) return -1;
                style.splice(style.end(), *fileRules);
                applySpecificStyleToElement(style, elements, true);
                delete fileRules;
                files[fileCount] = std::pair<std::string, int>("", ruleNumber);
                fileCount++;
                return fileCount - 1;
            }

            void StyleNodesManager::removeStyleInElements(int fileNumber, gui::elementStyle::StyleNode *element) {
                std::cerr << "Removing style from element\n";
                gui::elementStyle::StyleNode *child;
                element->deleteStyleFromFile(fileNumber);
                child = element->getChild();
                while (child != nullptr) {
                    removeStyleInElements(fileNumber, child);
                    child = child->getNext();
                }
            }

            void StyleNodesManager::removeStyle(int fileNumber) {
                for (std::pair<int, std::pair<std::string, int>> file : files) {
                }
                if (elements != nullptr && files.find(fileNumber) != files.cend()) {
                    removeStyleInElements(fileNumber, elements);
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

            bool StyleNodesManager::areElementSelectorsCompatibles(gui::elementStyle::StyleNode *elementStyle,
                                                                   const style::StyleComponentDataList *componentsList) {
                if (elementStyle == nullptr || componentsList == nullptr) return false;
                bool selectorExists = false;
                gui::elementStyle::StyleNode *currentStyle = elementStyle;
                style::StyleComponentDataList::const_reverse_iterator listEndIt = componentsList->crbegin();
                const std::set<style::StyleComponentData> *elementSelectors;
                elementSelectors = currentStyle->getSelectors();
                if (listEndIt->first.second == style::StyleComponentType::Modifier) {
                    listEndIt++;
                }
                for (style::StyleComponentDataList::const_reverse_iterator it = listEndIt; it != componentsList->crend(); it++) {
                    if (it->first.second == style::StyleComponentType::StarWildcard) {
                        selectorExists = true;
                        continue; // allow any element
                    }
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
                                                          // this step but not the next, and an other could work
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

            void StyleNodesManager::applyStyleToElement(gui::elementStyle::StyleNode *elementStyle, bool recursive) {
                applySpecificStyleToElement(style, elementStyle, recursive);
            }

            void StyleNodesManager::addElementStyle(gui::elementStyle::StyleNode *elementStyle) {
                if (elements == nullptr) setParentElementStyle(elementStyle);
                applyStyleToElement(elementStyle, true);
            }

        } // namespace manager
    } // namespace elementStyle
} // namespace gui
