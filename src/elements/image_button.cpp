#include "image_button.hpp"

ImageButton::ImageButton(ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                         const std::string &identifier)
    : UIElement{"imagebutton", elementsStyleManager, classes, identifier} {}

void ImageButton::renderSelfBeforeChilds() const {}

void ImageButton::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}