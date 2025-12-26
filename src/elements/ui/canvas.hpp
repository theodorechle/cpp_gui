#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "SDL3/SDL_render.h"

#include "ui_element.hpp"

namespace gui::element {
    class Canvas : public UiElement {
        SDL_Texture *texture = nullptr;

        void computeInnerLayout(int *width, int *height) const override;

        void renderSelfBeforeChilds() const override;

    public:
        Canvas(gui::elementStyle::manager::StyleManager *elementsStyleManager = nullptr,
               std::vector<std::string> *classes = nullptr, const std::string &identifier = "");

        ~Canvas() override;

        void init(int width, int height);

        void fillRect(int x, int y, int width, int height, SDL_Color color);

        void clear();

        // TODO: intercept events
    };
} // namespace gui::element

#endif // CANVAS_HPP
