#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "../abstracts/abstract_manager.hpp"
#include "render_nodes/element_to_render_node_converter.hpp"
#include "render_nodes/ui_render_node.hpp"
#include "root_element.hpp"
#include "ui_element.hpp"
#include "utils.hpp"
#include <SDL3/SDL.h>

namespace gui {
    namespace element {
        namespace manager {

            class UIManager : public AbstractManager {
                SDL_Window *window = nullptr;
                SDL_Renderer *renderer = nullptr;
                UiElement *clickedElement = nullptr;
                bool clicked = false;
                UiElement *hoveredElement = nullptr;
                UiElement *focusedElement = nullptr;
                SDL_Rect clipRect;
                bool canChangeSize = true;
                bool windowFocused = false;

                bool needRenderingUpdate = true;
                gui::element::ui::render::UiRenderNode *rootRenderNode = nullptr;
                SDL_Texture *renderedTexture = nullptr;

                void createRootElement() override;

                void computeNodesLayout(ui::render::UiRenderNode *renderNode);
                void initElementsBeforeLayoutComputing(gui::element::ui::render::UiRenderNode *rootRenderNode);
                void restoreAfterLayoutComputing(gui::element::ui::render::UiRenderNode *rootRenderNode);
                void prepareRenderNodes(UiElement *rootElement, gui::element::ui::render::UiRenderNode *rootRenderNode, bool isRoot = false);
                void computeNodesAndChildsLayout(gui::element::ui::render::UiRenderNode *node);
                void computeNodesRelativeLayout(gui::element::ui::render::UiRenderNode *node);
                void computeNodesFinalLayout(gui::element::ui::render::UiRenderNode *node, SDL_Rect *rootClipRect = nullptr);
                void computeElementsLayout();
                void createNodesTextures(gui::element::ui::render::UiRenderNode *node);
                void createRenderedTexture();

                void renderElements(bool clear = true) const override;
                void update() override;

                void sendEvent(const SDL_Event &event, UiElement *leafElement);

                /**
                 * set the modifier's state (enabled, disabled) on leafElement and all its parents
                 * if enabled is true, it will also throw the given event on each concerned elements
                 */
                void setElementsModifierState(const std::string &modifier, UiElement *leafElement, bool enabled, const SDL_Event &event);

                void updateRenderingData();

                void addChildToRootElement(gui::element::AbstractElement *childElement) override;

            public:
                UIManager(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *clipRect = nullptr);

                void setClipRect(const SDL_Rect &clipRect) { this->clipRect = clipRect; }

                void processEvent(const SDL_Event &event);

                void processMouseEvents();

                /**
                 * Not perfect, but should be called when elements are destroyed to prevent trying to call them (events)
                 */
                void resetEvents();
            };

        } // namespace manager
    } // namespace element
} // namespace gui

#endif // UIMANAGER_HPP