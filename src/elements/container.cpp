#include "container.hpp"

Container::Container(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                     const std::string &identifier)
    : UIElement{window, renderer, "container", elementsStyleManager, classes, identifier} {}

void Container::renderChilds() {
    UIElement *child = getChild();
    while (child != nullptr) {
        child->setRenderer(renderer); // TODO: give only a part of the renderer
        child->render();
        child = child->getNext();
    }
}

void Container::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}