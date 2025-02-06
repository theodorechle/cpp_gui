#ifndef INPUT_HPP
#define INPUT_HPP

#include "ui_element.hpp"

namespace gui {
    namespace element {

        class Input : public UIElement {
        public:
            Input(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                   const std::string &identifier = "");
        };

    } // namespace element
} // namespace gui


#endif // INPUT_HPP
