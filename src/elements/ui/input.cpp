#include "input.hpp"
#include "utils.hpp"

namespace gui::element {
    Input::Input(const std::string &text, const std::string &placeholder, gui::elementStyle::manager::StyleManager *elementsStyleManager,
                 std::vector<std::string> classes, const std::string &identifier, TTF_TextEngine *textEngine)
        : Label{"input", text, elementsStyleManager, classes, identifier, textEngine}, placeholder{placeholder} {
        registerEventHandler(ui::event::GuiEventType::EVENT_FOCUS_GAINED, [this](const event::Event *event) { this->focusGained(event); });
        registerEventHandler(ui::event::GuiEventType::EVENT_FOCUS_LOST, [this](const event::Event *event) { this->focusLost(event); });
        registerEventHandler(ui::event::GuiEventType::EVENT_TEXT_INPUT,
                             [this](const event::Event *event) { this->textInput(static_cast<const ui::event::TextEvent *>(event)); });
        registerEventHandler(ui::event::GuiEventType::EVENT_KEY_DOWN,
                             [this](const event::Event *event) { this->keyDown(static_cast<const ui::event::KeyEvent *>(event)); });
    }

    void Input::focusGained(const event::Event *event) {
        SDL_Rect rect;
        SDL_GetRenderClipRect(renderer(), &rect);
        if (!SDL_StartTextInput(window())) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::focusGained: Can't start text input: %s", SDL_GetError());
            return;
        }
        // FIXME: can't work since element rect is defined in UiRenderNodes not UiElement
        // Find a generic way to do something like this (displaying a virtual keyboard or other things)
        if (!SDL_SetTextInputArea(window(), &rect, cursorPos.x)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::focusGained: Can't set text area: %s", SDL_GetError());
            return;
        }
        sendEventToManager(event::ElementEvent::CONTENT_CHANGED);
    }

    void Input::focusLost(const event::Event *event) {
        if (!SDL_SetTextInputArea(window(), nullptr, 0)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::focusLost: Can't clear text area: %s", SDL_GetError());
            return;
        }
        if (!SDL_StopTextInput(window())) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::focusLost: Can't stop text input: %s", SDL_GetError());
            return;
        }
    }

    void Input::textInput(const ui::event::TextEvent *event) {
        addText(event->text);
    }

    void Input::keyDown(const ui::event::KeyEvent *event) {
        if (event->key == SDLK_BACKSPACE) removeText(1);
    }
} // namespace gui::element
