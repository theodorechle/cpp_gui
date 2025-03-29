#ifndef UI_RENDER_NODE_HPP
#define UI_RENDER_NODE_HPP

#include "../ui_element.hpp"
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

namespace gui::element::ui::render {
    typedef struct {
        int width;
        int height;
    } Size;

    typedef struct {
        float width;
        float height;
    } FSize;

    typedef struct {
        int x;
        int y;
    } Pos;

    typedef struct {
        float x;
        float y;
    } FPos;

    class UiRenderNode {
        SDL_Renderer *renderer = nullptr;
        // tree structure
        UiRenderNode *_parent = nullptr;
        UiRenderNode *_child = nullptr;
        UiRenderNode *_next = nullptr;

        // the UiElement corresponding to this node. Used for computing layouts.
        const gui::element::UiElement *baseElement;

        // computed by computeSelfLayout
        Size defaultSelfSize = {0, 0};

        // computed by computeSelfAndChildsLayout
        Size defaultSizeWithChilds = {0, 0};

        // computed by computeRelativeLayout
        Size relativeSize = {0, 0};

        // computed by computeFinalLayout
        struct {
            SDL_Rect elementRect; // the rect containing the entire element corresponding to this node, including margin, padding and borders
            SDL_Rect contentRect; // relative to the elementRect
            Pos scrollOffset;
        } usedLayout;

        SDL_Texture *texture;

    public:
        UiRenderNode(SDL_Renderer *renderer, UiRenderNode *parent = nullptr, const gui::element::UiElement *baseElement = nullptr);
        // tree
        UiRenderNode *parent() { return _parent; }
        UiRenderNode *child() { return _child; }
        UiRenderNode *next() { return _next; }
        void addChild(UiRenderNode *newChild);
        void next(UiRenderNode *nextNode) { _next = nextNode; }
        void parent(UiRenderNode *parentNode) { _parent = parentNode; }

        void computeSelfLayout();
        void computeSelfAndChildsLayout();
        void computeRelativeLayout();
        void computeFinalLayout();

        void updateTexture(bool recursive = true);
        void render(SDL_Renderer *renderer);
    };
} // namespace renderNode

#endif // UI_RENDER_NODE_HPP
