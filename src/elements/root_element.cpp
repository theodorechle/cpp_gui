#include "root_element.hpp"

namespace gui {
    namespace element {

        RootElement::RootElement(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                       const std::string &identifier)
            : UIElement{"root", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
