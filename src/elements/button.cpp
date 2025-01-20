#include "button.hpp"

Button::Button(ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
               const std::string &identifier)
    : UIElement{"button", elementsStyleManager, classes, identifier} {}

void Button::renderSelfBeforeChilds() const {}

void Button::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}
