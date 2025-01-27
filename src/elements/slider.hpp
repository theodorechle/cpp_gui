#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class Slider : public UIElement {
        public:
            Slider(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                   const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // SLIDER_HPP
