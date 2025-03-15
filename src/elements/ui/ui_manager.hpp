#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "root_element.hpp"
#include "ui_element.hpp"
#include "abstracts/abstract_manager.hpp"
#include "render_nodes/ui_render_node.hpp"
#include <SDL3/SDL.h>

namespace gui {
    namespace element {
        namespace manager {

            class UIManager : public AbstractManager {
                SDL_Window *window = nullptr;
                SDL_Renderer *renderer = nullptr;
                UIElement *clickedElement = nullptr;
                bool clicked = false;
                UIElement *hoveredElement = nullptr;
                UIElement *focusedElement = nullptr;
                SDL_Rect clipRect;
                bool canChangeSize = true;
                gui::element::ui::render::UiRenderNode *rootRenderNode;

                gui::element::AbstractElement *createRootElement() const override;

            public:
                UIManager(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *clipRect = nullptr);

                void setClipRect(const SDL_Rect &clipRect) { this->clipRect = clipRect; }

                void updateClipRect();

                /**
                 * Not perfect, but should be called when elements are destroyed to prevent trying to call them (events)
                 */
                void resetEvents();

                // parameters are needed for view containers
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