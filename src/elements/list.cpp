#include "list.hpp"

List::List(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
           const std::string &identifier)
    : UIElement{window, renderer, "list", elementsStyleManager, classes, identifier} {}

void List::renderSelfBeforeChilds() const {}

void List::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}