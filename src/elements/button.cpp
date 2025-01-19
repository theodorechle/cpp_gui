#include "button.hpp"

Button::Button(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
               const std::string &identifier)
    : UIElement{window, renderer, "button", elementsStyleManager, classes, identifier} {}

void Button::renderSelfBeforeChilds() const {}

void Button::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}
