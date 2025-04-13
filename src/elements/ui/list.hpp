#ifndef LIST_HPP
#define LIST_HPP

#include "ui_element.hpp"
#include <numeric>

namespace gui {
    namespace element {

        class List : public UiElement {
            void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, std::list<std::tuple<int, int>> childsSizes) const override;

        public:
            List(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                 const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // LIST_HPP
