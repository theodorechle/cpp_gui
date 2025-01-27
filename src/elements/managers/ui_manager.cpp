#include "ui_manager.hpp"

namespace gui {
    namespace element {
        namespace manager {

            void UIManager::setElementsTree(gui::element::AbstractElement *element) {
                AbstractManager::setElementsTree(new gui::element::Button()); // TODO: replace with a special element
                static_cast<gui::element::UIElement *>(elementsTree)->setRenderer(static_cast<gui::element::UIElement *>(element)->getRenderer());
                elementsTree->addChild(static_cast<gui::element::UIElement *>(element));
            }

            void UIManager::computeElementsLayout() {
                int width = 0, height = 0;
                elementsTree->computeDesiredLayout(&width, &height);
                SDL_GetCurrentRenderOutputSize(renderer, &width, &height);
                elementsTree->computeLayout(width, height);
            }

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
