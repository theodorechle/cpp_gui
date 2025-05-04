#ifndef LIST_HPP
#define LIST_HPP

#include "ui_element.hpp"
#include <numeric>

namespace gui {
    namespace element {

        class List : public UiElement {
        public:
            List(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                 const std::string &identifier = "");
            void computeSelfAndChildsLayout(int *selfWidth, int *selfHeight, int *selfWidthWithoutChilds, int *selfHeightWithoutChilds,
                                            std::list<std::tuple<int, int>> childsSizes) const override;
        };

    } // namespace element
} // namespace gui

#endif // LIST_HPP
