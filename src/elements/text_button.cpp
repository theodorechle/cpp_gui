#include "text_button.hpp"

TextButton::TextButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                       const std::string &identifier)
    : UIElement{window, renderer, "textbutton", elementsStyleManager, classes, identifier} {}

void TextButton::renderSelfBeforeChilds() const {}

void TextButton::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}