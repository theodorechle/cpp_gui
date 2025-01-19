#include "image_button.hpp"

ImageButton::ImageButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
                         const std::string &identifier)
    : UIElement{window, renderer, "imagebutton", elementsStyleManager, classes, identifier} {}

void ImageButton::renderSelfBeforeChilds() const {}

void ImageButton::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}