#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "button.hpp"

class TextButton : public UIElement {
public:
    TextButton(ElementsStyleManager *elementsStyleManager = nullptr,
               std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // TEXT_BUTTON_HPP
