#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "../abstracts/abstract_manager.hpp"
#include "render_nodes/ui_render_node.hpp"
#include "root_element.hpp"
#include "ui_element.hpp"
#include "utils.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

namespace gui::element {
    namespace manager {
        class UiManager : public AbstractManager {
            SDL_Window *window = nullptr;
            SDL_Renderer *renderer = nullptr;
            ui::render::UiRenderNode *clickedElement = nullptr;
            bool clicked = false;
            ui::render::UiRenderNode *hoveredElement = nullptr;
            ui::render::UiRenderNode *focusedElement = nullptr;
            SDL_Rect clipRect;
            bool canChangeSize = true;
            bool windowFocused = false;

            ui::render::UiRenderNode *rootRenderNode = nullptr;
            SDL_Texture *renderedTexture = nullptr;

            void createRootElement() override;

            void updateModifiedElements();
            void computeNodesLayout(ui::render::UiRenderNode *renderNode);
            void initElementsBeforeLayoutComputing(ui::render::UiRenderNode *rootRenderNode);
            void restoreAfterLayoutComputing(ui::render::UiRenderNode *rootRenderNode);
            void prepareRenderNodes(UiElement *rootElement, ui::render::UiRenderNode *rootRenderNode);
            void computeNodesAndChildsLayout(ui::render::UiRenderNode *node);
            void computeNodesRelativeLayout(ui::render::UiRenderNode *node);
            void computeNodesFinalLayout(ui::render::UiRenderNode *node, SDL_Rect *rootClipRect = nullptr);
            void computeElementsLayout();
            void createNodesTextures(ui::render::UiRenderNode *node);
            void createRenderedTexture();

            void renderElements(bool clear = true) const override;
            void update() override;

            void windowFocusLost();

            void updateRenderingData();

            // childElement must be a UiElement
            void addChildToRootElement(gui::element::AbstractElement *childElement) override;

            /**
             * Returns the renderNode who corresponds to the given element.
             * If none found, returns nullptr;
             */
            ui::render::UiRenderNode *renderNodeOf(const AbstractElement *element);

            /**
             * parentNode is the parent of the deleted nodes
             */
            void resetInvalidPointersOnNodesDeletion(const ui::render::UiRenderNode *parentNode, bool deleteUpdateElement = true);

            ui::render::UiRenderNode *hoveredLeafElement(SDL_Point *coordinates);

        public:
            void processMouseEvent(const SDL_Event *event);

            UiManager(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect *clipRect = nullptr);
            ~UiManager();

            void setClipRect(const SDL_Rect &clipRect) { this->clipRect = clipRect; }

            void processEvent(const SDL_Event *event);

            void scroll(int x, int y);

            void elementEvent(event::ElementEvent event, AbstractElement *caller) override;
        };
    } // namespace manager
} // namespace gui::element

#endif // UIMANAGER_HPP