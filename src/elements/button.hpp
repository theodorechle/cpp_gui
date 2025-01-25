#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class Button : public UIElement {
        public:
            Button(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                   const std::string &identifier = "");
            void renderSelfBeforeChilds() const override;
            void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
        };

    } // namespace element
} // namespace gui

#endif // BUTTON_HPP
