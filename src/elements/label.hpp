#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

class Label : public UIElement {
public:
    Label(ElementsStyleManager *elementsStyleManager = nullptr,
          std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // LABEL_HPP
