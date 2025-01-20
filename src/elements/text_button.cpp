#include "text_button.hpp"

TextButton::TextButton(ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                       const std::string &identifier)
    : UIElement{"textbutton", elementsStyleManager, classes, identifier} {}

void TextButton::renderSelfBeforeChilds() const {}

void TextButton::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}