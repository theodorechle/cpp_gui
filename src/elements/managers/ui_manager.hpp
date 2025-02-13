#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "../root_element.hpp"
#include "../ui_element.hpp"
#include "abstract_manager.hpp"
#include <SDL3/SDL.h>

namespace gui {
    namespace element {
        namespace manager {

            class UIManager : public AbstractManager {

                SDL_Window *window = nullptr;
                SDL_Renderer *renderer = nullptr;
                UIElement *clickedElement = nullptr;
                UIElement *hoveredElement = nullptr;
                UIElement *focusedElement = nullptr;
                SDL_Rect clipRect;

            public:
                UIManager(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *clipRect = nullptr);

                void setClipRect(const SDL_Rect &clipRect) { this->clipRect = clipRect; }

                void setElementsTree(gui::element::AbstractElement *element) override;

                void computeDesiredElementsLayout(int *width, int *height);

                void computeFinalElementsLayout();

                void computeElementsLayout() override;

                void renderElements(bool clear = true) const override;

                void processEvent(const SDL_Event &event);

                void processMouseEvents();

                void sendEvent(const SDL_Event &event, UIElement *leafElement);

                /**
                 * set the modifier's state (enabled, disabled) on leafElement and all its parents
                 * if enabled is true, it will also throw the given event on each concerned elements
                 */
                void setElementsModifierState(const std::string &modifier, UIElement *leafElement, bool enabled, const SDL_Event &event);
            };

        } // namespace manager
    } // namespace element
} // namespace gui

#endif // UIMANAGER_HPP