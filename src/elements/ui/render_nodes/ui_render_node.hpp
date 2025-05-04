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
            SDL_Rect elementRect =
                SDL_Rect{0, 0, 0, 0}; // the rect containing the entire element corresponding to this node, including margin, padding and borders
            SDL_Rect elementClippedRect = SDL_Rect{0, 0, 0, 0};
            // contentRect is implementation detail and thus should not be usable outside this class
            SDL_Rect contentRect = SDL_Rect{0, 0, 0, 0}; // relative to the elementRect
            Pos scrollOffset = {0, 0};
        } usedLayout;

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
        void computeFinalLayout(SDL_Rect clipRect = SDL_Rect{0, 0, 0, 0});
        SDL_Rect *elementRect();
        SDL_Rect *elementClippedRect();

        void render(bool recursive = true);
    };
} // namespace renderNode

#endif // UI_RENDER_NODE_HPP
