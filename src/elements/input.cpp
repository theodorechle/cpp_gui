#include "input.hpp"

namespace gui {
    namespace element {
        void Input::onFocusGet() {
            SDL_Rect rect = SDL_Rect{0, 0, 0, 0};
            getRect(&rect);
            if (!SDL_StartTextInput(getWindow())) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't start text input: %s", SDL_GetError());
                return;
            }
            if (!SDL_SetTextInputArea(getWindow(), &rect, cursorPos.x)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set text area: %s", SDL_GetError());
                return;
            }
        }

        void Input::onFocusLoose() {
            if (!SDL_SetTextInputArea(getWindow(), nullptr, 0)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't clear text area: %s", SDL_GetError());
                return;
            }
            if (!SDL_StopTextInput(getWindow())) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't stop text input: %s", SDL_GetError());
                return;
            }
        }

        Input::Input(const std::string &text, const std::string &placeholder, gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager,
                     std::vector<std::string> *classes, const std::string &identifier, TTF_TextEngine *textEngine)
            : Label{"input", text, elementsStyleManager, classes, identifier, textEngine}, placeholder{placeholder} {}

        void Input::catchEvent(const SDL_Event &event) {
            if (event.type == SDL_EVENT_TEXT_INPUT) {
                addText(event.text.text);
            }
            else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_BACKSPACE) {
                removeText(1);
            }
        }
    } // namespace element
} // namespace gui
