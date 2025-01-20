#include "slider.hpp"

Slider::Slider(ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
               const std::string &identifier)
    : UIElement{"slider", elementsStyleManager, classes, identifier} {}

void Slider::renderSelfBeforeChilds() const {}

void Slider::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}