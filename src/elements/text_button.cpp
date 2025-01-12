#include "text_button.hpp"

TextButton::TextButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                       const std::string &identifier, UIElement *parent, UIElement *child, UIElement *next)
    : UIElement{window, renderer, "textbutton", elementsStyleManager, classes, identifier, parent, child, next} {}

void TextButton::renderSelf() const {}

void TextButton::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}