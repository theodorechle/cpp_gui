#include "container.hpp"

Container::Container(ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                     const std::string &identifier)
    : UIElement{"container", elementsStyleManager, classes, identifier} {}

void Container::renderChilds() {
    UIElement *child = getChild();
    while (child != nullptr) {
        child->render();
        child = child->getNext();
    }
}

void Container::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}