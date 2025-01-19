#include "slider.hpp"

Slider::Slider(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
               const std::string &identifier)
    : UIElement{window, renderer, "slider", elementsStyleManager, classes, identifier} {}

void Slider::renderSelfBeforeChilds() const {}

void Slider::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}