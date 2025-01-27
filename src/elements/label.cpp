#include "label.hpp"
#include "../converters/color_converter.hpp"
#include <SDL3/SDL.h>

namespace gui {
    namespace element {

        Label::Label(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"label", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
