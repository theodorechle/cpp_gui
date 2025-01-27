#include "table.hpp"

namespace gui {
    namespace element {

        Table::Table(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"table", elementsStyleManager, classes, identifier} {}
 } // namespace element
} // namespace gui
