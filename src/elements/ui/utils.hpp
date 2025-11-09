#ifndef UI_RENDER_DATA_HPP
#define UI_RENDER_DATA_HPP

#include "../abstracts/abstract_utils.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"

namespace gui::element::ui {
    enum GuiLogCategory { GUI_RENDERING = SDL_LOG_CATEGORY_CUSTOM };
    enum GuiEvent { FOCUS = SDL_EVENT_USER, FOCUS_LOST, GUI_EVENTS_ENUM_SIZE };

    const int guiEventsStartingCode = SDL_RegisterEvents(GUI_EVENTS_ENUM_SIZE - SDL_EVENT_USER);

    Uint32 eventCode(Uint32 event);

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
        Size clippedElementSize;
        Pos scrollOffset;

        UiElementData(Size size, Size clippedSize, Pos scrollOffset)
            : elementSize{size}, clippedElementSize{clippedSize}, scrollOffset{scrollOffset} {}
    };
} // namespace gui::element::ui

#endif // UI_RENDER_DATA_HPP