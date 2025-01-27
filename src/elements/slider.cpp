#include "slider.hpp"

namespace gui {
    namespace element {

        Slider::Slider(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                       const std::string &identifier)
            : UIElement{"slider", elementsStyleManager, classes, identifier} {}
    } // namespace element
} // namespace gui
