#ifndef TABLE_HPP
#define TABLE_HPP

#include "ui_element.hpp"

class Table : public UIElement {
public:
    Table(ElementsStyleManager *elementsStyleManager = nullptr,
          std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // TABLE_HPP
