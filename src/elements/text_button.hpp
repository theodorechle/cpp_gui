#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "button.hpp"

namespace gui {
    namespace element {

        class TextButton : public UIElement {
        public:
            TextButton(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                       const std::string &identifier = "");
        };
    } // namespace element
} // namespace gui

#endif // TEXT_BUTTON_HPP
