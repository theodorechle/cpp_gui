#include "label.hpp"
#include <SDL3/SDL.h>

Label::Label(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
             const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : UIElement{window, renderer, "label", elementsStyleManager, classes, identifier, parent, child, next} {}

void Label::render() const {
    SDL_FRect rect = SDL_FRect{0, 0, 50, 50};
    Uint8 r, g, b, a;
    if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
        SDL_Log("Couldn't get draw color");
        return;
    }
    std::cerr << "Can get text color: " << (elementStyle->getRule("text-color") != nullptr) << std::endl;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}