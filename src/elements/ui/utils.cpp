#include "utils.hpp"
#include <SDL3/SDL_scancode.h>

namespace gui::element::ui::event {
    FocusEvent::FocusEvent(uint32_t type) : gui::element::event::Event{type} {}
    gui::element::event::Event *FocusEvent::copy() const { return new FocusEvent(type); }

    MouseEvent::MouseEvent(uint32_t type, float x, float y, Uint8 button) : gui::element::event::Event{type}, x{x}, y{y}, button{button} {}
    gui::element::event::Event *MouseEvent::copy() const { return new MouseEvent(type, x, y, button); }

    MouseWheelEvent::MouseWheelEvent(uint32_t type, float x, float y, Uint8 button, float scrollX, float scrollY)
        : MouseEvent{type, x, y, button}, scrollX{scrollX}, scrollY{scrollY} {}
    gui::element::event::Event *MouseWheelEvent::copy() const { return new MouseWheelEvent(type, x, y, button, scrollX, scrollY); }

    MouseMotionEvent::MouseMotionEvent(uint32_t type, float x, float y, SDL_MouseButtonFlags mouseFlags)
        : gui::element::event::Event{type}, x{x}, y{y}, mouseFlags{mouseFlags} {}
    gui::element::event::Event *MouseMotionEvent::copy() const { return new MouseMotionEvent(type, x, y, mouseFlags); }

    KeyEvent::KeyEvent(uint32_t type, SDL_Scancode scancode, SDL_Keycode key, SDL_Keymod mod)
        : gui::element::event::Event{type}, scancode{scancode}, key{key}, mod{mod} {}
    gui::element::event::Event *KeyEvent::copy() const { return new KeyEvent(type, scancode, key, mod); }

    TextEvent::TextEvent(uint32_t type) : gui::element::event::Event{type} {}
    gui::element::event::Event *TextEvent::copy() const { return new TextEvent(type); }

    TextInputEvent::TextInputEvent(uint32_t type, const std::string &text) : TextEvent{type}, text{text} {}
    gui::element::event::Event *TextInputEvent::copy() const { return new TextInputEvent(type, text); }

    TextSetEvent::TextSetEvent(uint32_t type, const std::string &text, bool append) : TextEvent{type}, text{text}, append{append} {}
    gui::element::event::Event *TextSetEvent::copy() const { return new TextSetEvent(type, text, append); }

    TextRemoveEvent::TextRemoveEvent(uint32_t type, size_t nbToDelete) : TextEvent{type}, nbToDelete{nbToDelete} {}
    gui::element::event::Event *TextRemoveEvent::copy() const { return new TextRemoveEvent(type, nbToDelete); }

    TextClearEvent::TextClearEvent(uint32_t type) : TextEvent{type} {}
    gui::element::event::Event *TextClearEvent::copy() const { return new TextClearEvent(type); }

} // namespace gui::element::ui::event
