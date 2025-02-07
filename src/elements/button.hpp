#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class Button : public UIElement {
            void (*onClickFunction)();
        public:
            Button(void (*onClick)(), gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                   const std::string &identifier = "");

        void catchEvent(const SDL_Event &event) override;
        };

    } // namespace element
} // namespace gui

#endif // BUTTON_HPP
