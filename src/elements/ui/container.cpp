#include "container.hpp"

namespace gui {
    namespace element {

        Container::Container(gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes,
                             const std::string &identifier)
            : UiElement{"container", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
