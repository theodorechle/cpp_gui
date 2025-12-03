#include "input.hpp"

namespace gui {
    namespace element {

        Input::Input(const std::string &text, const std::string &placeholder, gui::elementStyle::manager::StyleManager *elementsStyleManager,
                     std::vector<std::string> *classes, const std::string &identifier, TTF_TextEngine *textEngine)
            : Label{"input", text, elementsStyleManager, classes, identifier, textEngine}, placeholder{placeholder} {}

        void Input::catchEvent(const SDL_Event *event) {
            switch (event->type) {
            case ui::FOCUS:
                SDL_Rect rect;
                SDL_GetRenderClipRect(getRenderer(), &rect);
                if (!SDL_StartTextInput(getWindow())) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::onFocus: GetCan't start text input: %s", SDL_GetError());
                    return;
                }
                if (!SDL_SetTextInputArea(getWindow(), &rect, cursorPos.x)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::onFocus: Can't set text area: %s", SDL_GetError());
                    return;
                }
                sendEventToManager(ElementEvent::CONTENT_CHANGED);
                break;
            case ui::FOCUS_LOST:
                if (!SDL_SetTextInputArea(getWindow(), nullptr, 0)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::onFocus: Can't clear text area: %s", SDL_GetError());
                    return;
                }
                if (!SDL_StopTextInput(getWindow())) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Input::onFocus: Can't stop text input: %s", SDL_GetError());
                    return;
                }
                break;
            case SDL_EVENT_TEXT_INPUT:
                addText(event->text.text);
                sendEventToManager(ElementEvent::CONTENT_CHANGED);
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event->key.key == SDLK_BACKSPACE) removeText(1);
                sendEventToManager(ElementEvent::CONTENT_CHANGED);
                break;
            default:
                break;
            }
        }
    } // namespace element
} // namespace gui
