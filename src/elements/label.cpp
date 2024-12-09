#include "label.hpp"
#include <SDL3/SDL.h>


void Label::render() const {
    SDL_FRect rect = SDL_FRect{0, 0, 50, 50};
    Uint8 r, g, b, a;
    if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
        SDL_Log("Couldn't get draw color");
        return;
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}