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
        int x;
        int y;
    } Pos;

    class UiRenderNode {
        // tree structure
        UiRenderNode *parent = nullptr;
        UiRenderNode *child = nullptr;
        UiRenderNode *next = nullptr;

        // the UIElement corresponding to this node. Used for computing layouts.
        const gui::element::UIElement *baseElement;

        // computed by computeSelfLayout
        Size defaultSelfSize = {0, 0};

        // computed by computeRelativeLayout
        Size relativeSize = {0, 0};

        // computed by computeFinalLayout
        Size finalSize = {0, 0};

        struct {
            SDL_Rect elementRect; // the rect containing the entire element corresponding to this node, including margin, padding and borders
            SDL_Rect contentRect; // relative to the elementRect
            Size fullSize;
            Pos scrollOffset;
        } usedLayout;

        SDL_Texture texture;
        
        void computeSelfLayout();
        void computeRelativeLayout();
        void computeFinalLayout();
    };
} // namespace renderNode

#endif // UI_RENDER_NODE_HPP
