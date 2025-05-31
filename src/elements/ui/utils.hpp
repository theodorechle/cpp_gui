#ifndef UI_RENDER_DATA_HPP
#define UI_RENDER_DATA_HPP

#include "../abstracts/abstract_utils.hpp"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"

namespace gui::element::ui {
    enum GUI_LogCategory { GUI_RENDERING = SDL_LOG_CATEGORY_CUSTOM };

    struct Size {
        int width;
        int height;
    };

    struct Pos {
        int x;
        int y;
    };

    struct UiRenderData : RenderData {
        SDL_Rect elementRect;

        UiRenderData(SDL_Rect elementRect) : elementRect{elementRect} {}
    };

    struct UiElementData : ElementData {
        Size elementSize;

        UiElementData(Size size) : elementSize{size} {}
    };
}

#endif // UI_RENDER_DATA_HPP