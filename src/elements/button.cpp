#include "button.hpp"

namespace gui {
    namespace element {

        Button::Button(void (*onClick)(), gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UIElement{"button", elementsStyleManager, classes, identifier}, onClickFunction{onClick} {}

        void Button::catchEvent(gui::Event event) {
            if (event == gui::Event::Clicked) (onClickFunction)();
        }
    } // namespace element
} // namespace gui
