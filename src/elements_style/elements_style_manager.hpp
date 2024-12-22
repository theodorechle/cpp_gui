#ifndef ELEMENTS_STYLE_MANAGER_HPP
#define ELEMENTS_STYLE_MANAGER_HPP

#include "../style/style_component.hpp"
#include "../style/style_deserializer.hpp"
#include "element_style.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

class ElementsStyleManager {
    ElementStyle *elements = nullptr;
    std::list<StyleComponent *> style = std::list<StyleComponent *>();
    int fileCount = 0;
    // file number: <file name, rule number>
    std::unordered_map<int, std::pair<std::string, int>> files = {};
    int findFile(const std::string &fileName);
    void updateRulesPrioritiesInElements(int oldFileNumber, int newwFileNumber, ElementStyle *element);
    void updateRulesPriorities(int fileNumber);
    void applySpecificStyleToElement(std::list<StyleComponent *> specificStyle, ElementStyle *elementStyle, bool recursive = false);

public:
    void setParentElementStyle(ElementStyle *elementStyle) { elements = elementStyle; }
    // returns a number who corresponds to the file number (used to delete it)
    int addStyleFile(const std::string &fileName);
    // returns a number who corresponds to the style pseudo-file number (used to delete it)
    int addStyle(const std::string &style);
    void removeStyleInElements(int fileNumber, ElementStyle *element);
    void removeStyle(int fileNumber);
    bool areElementSelectorsCompatibles(ElementStyle *elementStyle, const StyleComponentDataList *componentsList);
    void applyStyleToElement(ElementStyle *elementStyle, bool recursive = false);
    void addElementStyle(ElementStyle *elementStyle);
};

#endif // ELEMENTS_STYLE_MANAGER_HPP
