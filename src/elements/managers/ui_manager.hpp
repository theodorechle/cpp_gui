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
                bool mouseEventsOccurred = false;

            public:
                UIManager(SDL_Window *window, SDL_Renderer *renderer): window{window}, renderer{renderer} {}

                void setElementsTree(gui::element::AbstractElement *element) override;

                void computeElementsLayout() override;

                void renderElements() const override;

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