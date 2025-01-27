#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class Label : public UIElement {
        public:
            Label(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                  const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui

#endif // LABEL_HPP
