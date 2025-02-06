#include "input.hpp"

namespace gui {
    namespace element {

        Input::Input(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                     const std::string &identifier)
            : UIElement{"input", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
