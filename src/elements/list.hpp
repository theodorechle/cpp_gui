#ifndef LIST_HPP
#define LIST_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class List : public UIElement {
        public:
            List(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                 const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // LIST_HPP
