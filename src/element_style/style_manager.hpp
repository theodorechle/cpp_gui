#ifndef STYLE_MANAGER_HPP
#define STYLE_MANAGER_HPP

#include "../../cpp_style/src/abstract_configuration.hpp"
#include "../../cpp_style/src/style_component.hpp"
#include "../../cpp_style/src/style_deserializer.hpp"
#include "../elements/abstracts/abstract_element.hpp"
#include "element_style.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>

namespace gui::elementStyle::manager {

    class StyleManager {
        element::AbstractElement *_rootElement = nullptr;

        const style::config::Config *config;
        std::list<style::StyleDefinition *> style = {};
        int fileCount = 0;
        // file number: <file name, nb rules>
        std::unordered_map<int, std::pair<std::string, int>> files = {};
        std::unordered_set<std::string> defaultFontsPaths = {};

        int findFile(const std::string &fileName);
        void updateRulesPrioritiesInElements(int oldFileNumber, int newFileNumber, element::AbstractElement *element);
        void updateRulesPriorities(int fileNumber);
        bool elementSelectorsCompatibles(element::AbstractElement *elementStyle, const style::StyleComponentDataList *componentsList);
        bool elementSelectorsCompatiblesLoop(style::StyleComponentDataList::const_reverse_iterator componentDataIt,
                                             style::StyleComponentDataList::const_reverse_iterator componentDataListEndIt,
                                             element::AbstractElement *elementStyle);

    public:
        StyleManager(const style::config::Config *config) : config{config} {}
        ~StyleManager();
        void rootElement(element::AbstractElement *elementStyle) { _rootElement = elementStyle; }

        // returns a number who corresponds to the file number (used to delete it) or -1 if not added
        int addStyleFile(const std::string &fileName);
        // returns a number who corresponds to the style pseudo-file number (used to delete it) or -1 if not added
        int addStyle(const std::string &style);
        void removeStyleInElements(int fileNumber, element::AbstractElement *element);
        void removeStyle(int fileNumber);

        // TODO: Are both methods needed?
        void applyStyleToElement(element::AbstractElement *elementStyle);
        void addElementStyle(element::AbstractElement *elementStyle);

        void addDefaultFontPath(const std::string &path);
        const std::unordered_set<std::string> &getDefaultFontsPaths() const;
    };

} // namespace gui::elementStyle::manager

#endif // STYLE_MANAGER_HPP
