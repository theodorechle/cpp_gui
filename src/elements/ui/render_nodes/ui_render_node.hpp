#ifndef UI_RENDER_NODE_HPP
#define UI_RENDER_NODE_HPP

#include "../../abstracts/abstract_element.hpp"
#include "../../abstracts/abstract_utils.hpp"
#include "../ui_element.hpp"
#include "../utils.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace gui::element::ui::render {
    class UiRenderNode {
        SDL_Renderer *renderer = nullptr;
        // tree structure
        UiRenderNode *_parent = nullptr;
        UiRenderNode *_child = nullptr;
        UiRenderNode *_next = nullptr;

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

    public:
        // the UiElement corresponding to this node. Used for computing layouts.
        gui::element::UiElement *baseElement; // FIXME: should be private, but needed by the ui manager

        UiRenderNode(SDL_Renderer *renderer, UiRenderNode *parent = nullptr, gui::element::UiElement *baseElement = nullptr);
        ~UiRenderNode();
        // tree
        const UiRenderNode *constParent() const { return _parent; }
        const UiRenderNode *constChild() const { return _child; }
        const UiRenderNode *constNext() const { return _next; }
        UiRenderNode *parent() { return _parent; }
        UiRenderNode *child() { return _child; }
        UiRenderNode *next() { return _next; }
        // remove pointer to childs, but does not delete them
        void removeChilds();
        void addChild(UiRenderNode *newChild);
        void next(UiRenderNode *nextNode) { _next = nextNode; }
        void parent(UiRenderNode *parentNode) { _parent = parentNode; }

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

        void scroll(int x, int y);

        const UiElementData *childData(const UiElement *child) const;
        void debugDisplay(int indent = 0) const;

        bool isParentOf(const UiRenderNode *node) const;
    };
} // namespace gui::element::ui::render

#endif // UI_RENDER_NODE_HPP
