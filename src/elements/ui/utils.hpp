#ifndef UI_RENDER_DATA_HPP
#define UI_RENDER_DATA_HPP

#include "../abstracts/abstract_utils.hpp"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_rect.h"
#include <SDL3/SDL_scancode.h>
#include <cstddef>
#include <cstdint>
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

    struct FPos {
        float x;
        float y;
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
            EVENT_TEXT_INPUT,
            EVENT_TEXT_SET,
            EVENT_TEXT_REMOVE,
            EVENT_TEXT_CLEAR
        };

        struct FocusEvent : gui::element::event::Event {
            FocusEvent(uint32_t type);
            Event *copy() const override;
        };

        struct MouseEvent : gui::element::event::Event {
            float x;
            float y;
            Uint8 button;
            MouseEvent(uint32_t type, float x, float y, Uint8 button);
            Event *copy() const override;
        };

        struct MouseWheelEvent : MouseEvent {
            float scrollX;
            float scrollY;
            MouseWheelEvent(uint32_t type, float x, float y, Uint8 button, float scrollX, float scrollY);
            Event *copy() const override;
        };

        struct MouseMotionEvent : gui::element::event::Event {
            float x;
            float y;
            SDL_MouseButtonFlags mouseFlags;
            MouseMotionEvent(uint32_t type, float x, float y, SDL_MouseButtonFlags mouseFlags);
            Event *copy() const override;
        };

        struct KeyEvent : gui::element::event::Event {
            SDL_Scancode scancode; /**< SDL physical key code */
            SDL_Keycode key;       /**< SDL virtual key code */
            SDL_Keymod mod;        /**< current key modifiers */
            KeyEvent(uint32_t type, SDL_Scancode scancode, SDL_Keycode key, SDL_Keymod mod);
            Event *copy() const override;
        };

        struct TextEvent : gui::element::event::Event {
            TextEvent(uint32_t type);
            Event *copy() const override;
        };

        struct TextInputEvent : TextEvent {
            std::string text;
            TextInputEvent(uint32_t type, const std::string &text);
            Event *copy() const override;
        };

        struct TextSetEvent : TextEvent {
            std::string text;
            bool append; // whether to append or to replace
            TextSetEvent(uint32_t type, const std::string &text, bool append);
            Event *copy() const override;
        };

        struct TextRemoveEvent : TextEvent {
            size_t nbToDelete;
            TextRemoveEvent(uint32_t type, size_t nbToDelete);
            Event *copy() const override;
        };

        struct TextClearEvent : TextEvent {
            TextClearEvent(uint32_t type);
            Event *copy() const override;
        };

    } // namespace event
} // namespace gui::element::ui

#endif // UI_RENDER_DATA_HPP
