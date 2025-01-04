#include "button.hpp"

Button::Button(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
               const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : UIElement{window, renderer, "button", elementsStyleManager, classes, identifier, parent, child, next} {}

void Button::render() const {}