#include "list.hpp"

List::List(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
           const std::string &identifier, UIElement *parent, UIElement *child, UIElement *next)
    : UIElement{window, renderer, "list", elementsStyleManager, classes, identifier, parent, child, next} {}

void List::renderSelfBeforeChilds() const {}

void List::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}