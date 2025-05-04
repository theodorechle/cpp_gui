#include "ui_render_node.hpp"

namespace gui::element::ui::render {
    UiRenderNode::UiRenderNode(SDL_Renderer *renderer, UiRenderNode *parent, const gui::element::UiElement *baseElement)
        : renderer{renderer}, _parent{parent}, baseElement{baseElement} {}

    void UiRenderNode::addChild(UiRenderNode *newChild) {
        if (newChild == nullptr) return;
        UiRenderNode *nextChild = child();
        UiRenderNode *selfChild;
        if (nextChild == nullptr) {
            this->_child = newChild;
        }
        else {
            do {
                selfChild = nextChild;
                nextChild = selfChild->next();
            } while (nextChild != nullptr);
            selfChild->next(newChild);
        }
        if (newChild != nullptr) {
            newChild->parent(this);
        }
    }

    void UiRenderNode::computeSelfLayout() {
        if (baseElement == nullptr) return;
        std::cerr << "computeSelfLayout (" << baseElement->name() << "): ";
        baseElement->computeInnerLayout(&(defaultSelfSize.width), &(defaultSelfSize.height));
        baseElement->computeTotalLayout(&(defaultSelfSize.width), &(defaultSelfSize.height));
    }

    void UiRenderNode::computeSelfAndChildsLayout() {
        if (baseElement == nullptr) return;
        std::list<std::tuple<int, int>> childsSizes = {};
        UiRenderNode *child = _child;
        while (child != nullptr) {
            child->computeSelfAndChildsLayout();
            childsSizes.push_back(std::tuple<int, int>(child->defaultSizeWithChilds.width, child->defaultSizeWithChilds.height));
            child = child->_next;
        }

        std::cerr << "computeSelfAndChildsLayout (" << baseElement->name() << "): ";
        baseElement->computeSelfAndChildsLayout(&(defaultSizeWithChilds.width), &(defaultSizeWithChilds.height), &(defaultSelfSize.width),
                                                &(defaultSelfSize.height), childsSizes);
        baseElement->computeTotalLayout(&(defaultSizeWithChilds.width), &(defaultSizeWithChilds.height));
    }

    void UiRenderNode::computeRelativeLayout() {
        if (baseElement == nullptr) return;
        // set variables like rem, em, ...
        // percentages should be set now
        std::list<std::tuple<int, int>> childsSizes = {};
        UiRenderNode *child = _child;
        while (child != nullptr) {
            child->computeRelativeLayout();
            childsSizes.push_back(std::tuple<int, int>(child->relativeSize.width, child->relativeSize.height));
            child = child->_next;
        }

        std::cerr << "computeRelativeLayout (" << baseElement->name() << "): ";
        baseElement->computeSelfAndChildsLayout(&(relativeSize.width), &(relativeSize.height), &(defaultSizeWithChilds.width),
                                                &(defaultSizeWithChilds.height), childsSizes);
        baseElement->computeTotalLayout(&(relativeSize.width), &(relativeSize.height));
    }

    SDL_Rect *UiRenderNode::elementRect() { return &usedLayout.elementRect; }

    SDL_Rect *UiRenderNode::elementClippedRect() { return &usedLayout.elementClippedRect; }

    void UiRenderNode::computeFinalLayout(SDL_Rect clipRect) {
        // overflows, ...

        usedLayout.elementRect.w = relativeSize.width;
        usedLayout.elementRect.h = relativeSize.height;

        // TODO: update this code
        usedLayout.contentRect.w = relativeSize.width;
        usedLayout.contentRect.h = relativeSize.height;

        usedLayout.elementClippedRect = clipRect;
        std::cerr << "computeFinalLayout (" << baseElement->name() << "): ";
        std::cerr << "width=" << usedLayout.elementRect.w << ", height=" << usedLayout.elementRect.h << " --- clipped: ";
        std::cerr << "width=" << usedLayout.elementClippedRect.w << ", height=" << usedLayout.elementClippedRect.h << "\n";
    }

    void UiRenderNode::render(bool recursive) {
        if (baseElement == nullptr) return;
        // baseElement->renderScrollBars(); // TODO

        SDL_Rect clipRect;
        SDL_GetRenderClipRect(renderer, &clipRect);

        if (!SDL_SetRenderClipRect(renderer, &(usedLayout.elementClippedRect))) {
            SDL_LogError(GUI_RENDERING, "uiRenderNode::render: can't set clip rect '%s'", SDL_GetError());
            return;
        }
        if (!baseElement->render()) SDL_RenderClear(renderer);
        if (!SDL_SetRenderClipRect(renderer, &clipRect)) {
            SDL_LogError(GUI_RENDERING, "uiRenderNode::render: can't restore clip rect '%s'", SDL_GetError());
        }
    }
} // namespace gui::element::ui::renderNode
