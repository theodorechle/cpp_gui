#ifndef ELEMENTS_STYLE_MANAGER_HPP
#define ELEMENTS_STYLE_MANAGER_HPP

#include "../../style/style_component.hpp"
#include "../../style/style_deserializer.hpp"
#include "../element_style.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace gui {
    namespace elementStyle {
        namespace manager {

            class ElementsStyleManager {
                gui::elementStyle::ElementStyle *elements = nullptr;
                std::list<style::StyleBlock *> style = {};
                int fileCount = 0;
                // file number: <file name, nb rules>
                std::unordered_map<int, std::pair<std::string, int>> files = {};
                std::string fontsPath = "";

                int findFile(const std::string &fileName);
                void updateRulesPrioritiesInElements(int oldFileNumber, int newwFileNumber, gui::elementStyle::ElementStyle *element);
                void updateRulesPriorities(int fileNumber);
                void applySpecificStyleToElement(std::list<style::StyleBlock *> specificStyle, gui::elementStyle::ElementStyle *elementStyle,
                                                 bool recursive = false);

            public:
                ~ElementsStyleManager();
                void setParentElementStyle(gui::elementStyle::ElementStyle *elementStyle) { elements = elementStyle; }
                void setFontsPath(const std::string &path);
                const std::string &getFontsPath() const;
                // returns a number who corresponds to the file number (used to delete it) or -1 if not added
                int addStyleFile(const std::string &fileName);
                // returns a number who corresponds to the style pseudo-file number (used to delete it) or -1 if not added
                int addStyle(const std::string &style);
                void removeStyleInElements(int fileNumber, gui::elementStyle::ElementStyle *element);
                void removeStyle(int fileNumber);
                bool areElementSelectorsCompatibles(gui::elementStyle::ElementStyle *elementStyle,
                                                    const style::StyleComponentDataList *componentsList);
                void applyStyleToElement(gui::elementStyle::ElementStyle *elementStyle, bool recursive = false);
                void addElementStyle(gui::elementStyle::ElementStyle *elementStyle);
            };

        } // namespace manager
    } // namespace elementStyle
} // namespace gui

#endif // ELEMENTS_STYLE_MANAGER_HPP
