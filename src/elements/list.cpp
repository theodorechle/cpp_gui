#include "list.hpp"

List::List(ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
           const std::string &identifier)
    : UIElement{"list", elementsStyleManager, classes, identifier} {}

void List::renderSelfBeforeChilds() const {}

void List::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}