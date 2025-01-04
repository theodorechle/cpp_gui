#include "list.hpp"

List::List(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
           const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : UIElement{window, renderer, "list", elementsStyleManager, classes, identifier, parent, child, next} {}

void List::render() const {}