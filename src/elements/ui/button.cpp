#include "button.hpp"

namespace gui {
    namespace element {

        Button::Button(std::function<void()> onClick, gui::elementStyle::manager::StyleNodesManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
            : UiElement{"button", elementsStyleManager, classes, identifier}, onClickFunction{onClick} {}

        void Button::catchEvent(const SDL_Event &event) {
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) (onClickFunction)();
        }
    } // namespace element
} // namespace gui
