#include "canvas.hpp"

namespace gui::element {
    Canvas::Canvas(gui::elementStyle::manager::StyleManager *elementsStyleManager, std::vector<std::string> *classes, const std::string &identifier)
        : UiElement{"canvas", elementsStyleManager, classes, identifier} {}

    Canvas::~Canvas() { SDL_DestroyTexture(texture); }

    void Canvas::init(int width, int height) {
        texture = SDL_CreateTexture(renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    }

    void Canvas::computeInnerLayout(int *width, int *height) const {
        if (texture != nullptr) {
            (*width) = texture->w;
            (*height) = texture->h;
        }
    }

    void Canvas::renderSelfBeforeChilds() const { SDL_RenderTexture(renderer(), texture, nullptr, nullptr); }

    void Canvas::fillRect(int x, int y, int width, int height, SDL_Color color) {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer(), &r, &g, &b, &a);
        SDL_Texture *previousTexture = SDL_GetRenderTarget(renderer());
        SDL_FRect rect = SDL_FRect{static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)};
        SDL_SetRenderTarget(renderer(), texture);
        SDL_SetRenderDrawColor(renderer(), color.r, color.g, color.b, color.a);

        SDL_RenderFillRect(renderer(), &rect); // actual drawing

        SDL_SetRenderDrawColor(renderer(), r, g, b, a);
        SDL_SetRenderTarget(renderer(), previousTexture);
    }

    void Canvas::fillRects(SDL_FRect rects[], size_t nbRects, SDL_Color color) {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(renderer(), &r, &g, &b, &a);
        SDL_Texture *previousTexture = SDL_GetRenderTarget(renderer());
        SDL_SetRenderTarget(renderer(), texture);
        SDL_SetRenderDrawColor(renderer(), color.r, color.g, color.b, color.a);

        SDL_RenderFillRects(renderer(), rects, nbRects); // actual drawing

        SDL_SetRenderDrawColor(renderer(), r, g, b, a);
        SDL_SetRenderTarget(renderer(), previousTexture);
    }

    void Canvas::clear() { fillRect(0, 0, texture->w, texture->h, SDL_Color{0, 0, 0, 0}); }

} // namespace gui::element
