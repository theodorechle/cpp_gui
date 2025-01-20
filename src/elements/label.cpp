#include "label.hpp"
#include <SDL3/SDL.h>
#include "../converters/color_converter.hpp"

Label::Label(ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
             const std::string &identifier)
    : UIElement{"label", elementsStyleManager, classes, identifier} {}

void Label::renderSelfBeforeChilds() const {
    // SDL_FRect rect = SDL_FRect{0, 0, 50, 50};
    // SDL_Color color = SDL_Color();
    // Uint8 r, g, b, a;
    // if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
    //     SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't get draw color");
    //     return;
    // }
    // if (ColorConverter::convert(elementStyle->getRule("text-color"), &color)) {
    //     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    // }
    // else {
    //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    // }
    // SDL_RenderRect(renderer, &rect);
    // SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Label::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}