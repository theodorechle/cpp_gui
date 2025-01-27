#include "text_button.hpp"

namespace gui {
    namespace element {

        TextButton::TextButton(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                               const std::string &identifier)
            : UIElement{"textbutton", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
