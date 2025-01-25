#ifndef IMAGE_BUTTON_HPP
#define IMAGE_BUTTON_HPP

#include "button.hpp"

namespace gui {
    namespace element {

        class ImageButton : public UIElement {
        public:
            ImageButton(gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
                        const std::string &identifier = "");
            void renderSelfBeforeChilds() const override;
            void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
        };

    } // namespace element
} // namespace gui

#endif // IMAGE_BUTTON_HPP
