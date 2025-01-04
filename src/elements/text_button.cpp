#include "text_button.hpp"

TextButton::TextButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                       const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : UIElement{window, renderer, "textbutton", elementsStyleManager, classes, identifier, parent, child, next} {}

void TextButton::render() const {}