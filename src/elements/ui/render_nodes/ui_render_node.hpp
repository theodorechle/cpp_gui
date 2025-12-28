#ifndef UI_RENDER_NODE_HPP
#define UI_RENDER_NODE_HPP

#include "../../../../cpp_commons/src/node.hpp"
#include "../../abstracts/abstract_element.hpp"
#include "../../abstracts/abstract_utils.hpp"
#include "../ui_element.hpp"
#include "../utils.hpp"
#include <sstream>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace gui::element::ui::render {
    class UiRenderNode : public commons::Node<UiRenderNode> {
        SDL_Renderer *renderer = nullptr;

        // computed by computeSelfLayout
        Size defaultSelfSize = {0, 0};

        // computed by computeSelfAndChildsLayout
        Size defaultSizeWithChilds = {0, 0};

        // computed by computeRelativeLayout
        Size relativeSize = {0, 0};

        // computed by computeFinalLayout
        struct {
            SDL_Rect elementRect =
                SDL_Rect{0, 0, 0, 0}; // the rect containing the entire element corresponding to this node, including margin, padding and borders
            SDL_Rect elementClippedRect = SDL_Rect{0, 0, 0, 0};
            // contentRect is implementation detail and thus should not be usable outside this class
            SDL_Rect contentRect = SDL_Rect{0, 0, 0, 0}; // relative to the elementRect
            Pos scrollOffset = {0, 0};
            Pos startCoords = {0, 0};
        } usedLayout;

        SDL_Texture *nodeTexture = nullptr;

        SDL_Rect computeNewClipRect(SDL_Rect *oldClipRect, SDL_Rect *wantedNewClipRect);

        std::string debugValue() const override;

    public:
        // the UiElement corresponding to this node. Used for computing layouts.
        gui::element::UiElement *baseElement; // FIXME: should be private, but needed by the ui manager

        UiRenderNode(SDL_Renderer *renderer, UiRenderNode *parentNode = nullptr, gui::element::UiElement *baseElement = nullptr);
        ~UiRenderNode();
        // remove pointer to childs, but does not delete them

        void computeSelfLayout();
        void computeSelfAndChildsLayout();
        void computeRelativeLayout();
        void computeFinalLayout(SDL_Rect *clipRect = nullptr, bool forceSize = false);
        const SDL_Rect *elementRect() const;
        const SDL_Rect *elementClippedRect() const;

        const Pos *startCoords() const;

        void initBeforeLayoutComputing();
        void restoreAfterLayoutComputing();
        void render(bool recursive = true);
        void createTexture();
        void renderElement(UiRenderData *data);
        void renderOnParentSurface() const;
        bool renderChildElement(const UiElement *element, UiRenderData *data);

        /*
        returns whether it has successfully scrolled (even if not as much as the asked scroll)
        */
        bool scroll(int x, int y);

        const UiElementData *childData(const UiElement *childElement) const;

        bool isParentOf(const UiRenderNode *node) const;
    };
} // namespace gui::element::ui::render

#endif // UI_RENDER_NODE_HPP
