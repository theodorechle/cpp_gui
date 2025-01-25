#include "button.hpp"

namespace gui {
    namespace element {

        Button::Button(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"button", elementsStyleManager, classes, identifier} {}

        void Button::renderSelfBeforeChilds() const {}

        void Button::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}

    } // namespace element
} // namespace gui
