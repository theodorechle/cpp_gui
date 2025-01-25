#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "../ui_element.hpp"
#include "abstract_manager.hpp"
#include <SDL3/SDL.h>

namespace gui {
    namespace element {
        namespace manager {

            class UIManager : public AbstractManager {
                SDL_Window *window = nullptr;
                SDL_Renderer *renderer = nullptr;

            public:
                UIManager(SDL_Window *window, SDL_Renderer *renderer) : window{window}, renderer{renderer} {}
                void setElementsTree(gui::element::UIElement *element);

                using AbstractManager::setElementsTree;

                void renderElements() const override;

                void processEvent(const SDL_Event &event);
            };

        } // namespace manager
    } // namespace element
} // namespace gui

#endif // UIMANAGER_HPP