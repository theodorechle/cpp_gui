#include "list.hpp"

namespace gui {
    namespace element {

        List::List(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"list", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
