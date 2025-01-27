#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class Container : public UIElement {
        public:
            Container(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                      const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // CONTAINER_HPP
