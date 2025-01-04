#include "image_button.hpp"

ImageButton::ImageButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                         const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : UIElement{window, renderer, "imagebutton", elementsStyleManager, classes, identifier, parent, child, next} {}

void ImageButton::render() const {}