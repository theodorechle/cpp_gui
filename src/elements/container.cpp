#include "container.hpp"

Container::Container(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                     const std::string &identifier, UIElement *parent, UIElement *child, UIElement *next)
    : UIElement{window, renderer, "container", elementsStyleManager, classes, identifier, parent, child, next} {}

void Container::renderSelf() const {}