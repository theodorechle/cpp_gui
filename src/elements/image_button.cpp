#include "image_button.hpp"

namespace gui {
    namespace element {

        ImageButton::ImageButton(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"imagebutton", elementsStyleManager, classes, identifier} {}

        void ImageButton::renderSelfBeforeChilds() const {}

        void ImageButton::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}

    } // namespace element
} // namespace gui
