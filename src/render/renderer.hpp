#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "abstract_renderer.hpp"
#include <SDL3/SDL.h>

class Renderer: public AbstractRenderer {
    SDL_Renderer *renderer = nullptr;
public:
    Renderer(SDL_Renderer *renderer): renderer{renderer} {}
    void render() const override;
};

#endif // RENDERER_HPP
