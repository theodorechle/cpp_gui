#include "renderer.hpp"

void Renderer::render() const {
    SDL_RenderPresent(renderer);
}