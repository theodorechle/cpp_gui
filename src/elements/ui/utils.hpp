#ifndef UI_RENDER_DATA_HPP
#define UI_RENDER_DATA_HPP

#include "../abstracts/abstract_utils.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_rect.h"
#include <string>

namespace gui::element::ui {
    enum GuiLogCategory { GUI_RENDERING = SDL_LOG_CATEGORY_CUSTOM };

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

    namespace event {

        enum GuiEventType {
            EVENT_MOUSE_BUTTON_DOWN = gui::element::event::OTHER_EVENTS,
            EVENT_MOUSE_BUTTON_UP,
            EVENT_MOUSE_MOTION,
            EVENT_HOVER,
            EVENT_UNHOVER,
            EVENT_SCROLL,
            EVENT_FOCUS_GAINED,
            EVENT_FOCUS_LOST,
            EVENT_KEY_DOWN,
            EVENT_KEY_UP,
            EVENT_TEXT_INPUT
        };

        struct MouseEvent : gui::element::event::Event {
            float x;
            float y;
            Uint8 button;
        };

        struct MouseMotionEvent : gui::element::event::Event {
            float x;
            float y;
            SDL_MouseButtonFlags mouseFlags;
        };

        struct MouseWheelEvent : MouseEvent {
            float scrollX;
            float scrollY;
        };

        struct KeyEvent : gui::element::event::Event {
            SDL_Scancode scancode; /**< SDL physical key code */
            SDL_Keycode key;       /**< SDL virtual key code */
            SDL_Keymod mod;        /**< current key modifiers */
        };

        struct TextEvent : gui::element::event::Event {
            std::string text;
        };

    } // namespace event
} // namespace gui::element::ui

#endif // UI_RENDER_DATA_HPP