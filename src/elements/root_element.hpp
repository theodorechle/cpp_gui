#ifndef ROOT_ELEMENT_HPP
#define ROOT_ELEMENT_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class RootElement : public UIElement {
        public:
            RootElement(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                   const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // ROOT_ELEMENT_HPP
