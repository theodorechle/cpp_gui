#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "ui_element.hpp"

class Slider : public UIElement {
public:
    Slider(ElementsStyleManager *elementsStyleManager = nullptr,
           std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // SLIDER_HPP
