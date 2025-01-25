#include "text_button.hpp"

namespace gui {
    namespace element {

        TextButton::TextButton(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"textbutton", elementsStyleManager, classes, identifier} {}

        void TextButton::renderSelfBeforeChilds() const {}

        void TextButton::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}

    } // namespace element
} // namespace gui
