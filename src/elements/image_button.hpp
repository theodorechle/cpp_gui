#ifndef IMAGE_BUTTON_HPP
#define IMAGE_BUTTON_HPP

#include "button.hpp"

class ImageButton : public UIElement {
public:
    ImageButton(ElementsStyleManager *elementsStyleManager = nullptr,
                std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // IMAGE_BUTTON_HPP
