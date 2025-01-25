#include "table.hpp"

namespace gui {
    namespace element {

        Table::Table(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"table", elementsStyleManager, classes, identifier} {}

        void Table::renderSelfBeforeChilds() const {}

        void Table::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}

    } // namespace element
} // namespace gui
