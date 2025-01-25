#include "ui_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {

            void UIManager::setElementsTree(gui::element::UIElement *element) { return AbstractManager::setElementsTree(element); }

            void UIManager::renderElements() const {
                Uint8 r, g, b, a;
                if (SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, r, g, b, a);
                }
                elementsTree->render();
                SDL_RenderPresent(renderer);
            }

            void UIManager::processEvent(const SDL_Event &event) {
                if (event.type == SDL_EVENT_QUIT) {
                    status(Status::ENDED);
                    return;
                }
            }

        } // namespace manager
    } // namespace element
} // namespace gui
