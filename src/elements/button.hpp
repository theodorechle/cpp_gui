#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ui_element.hpp"

class Button : public UIElement {
public:
    Button(ElementsStyleManager *elementsStyleManager = nullptr,
           std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // BUTTON_HPP
