#include "container.hpp"

namespace gui {
    namespace element {

        Container::Container(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                             const std::string &identifier)
            : UIElement{"container", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
