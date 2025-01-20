#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "ui_element.hpp"

class Container : public UIElement {
public:
    Container(ElementsStyleManager *elementsStyleManager = nullptr,
              std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderChilds() override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // CONTAINER_HPP
