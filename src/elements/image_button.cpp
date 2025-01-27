#include "image_button.hpp"

namespace gui {
    namespace element {

        ImageButton::ImageButton(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"imagebutton", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
