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

    void UiRenderNode::updateTexture(bool recursive) {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, usedLayout.elementRect.w, usedLayout.elementRect.h);
        if (texture == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                         "UiRenderNode::updateTexture (linked to UiElement '%s'): Can't create a texture for an ui_render_node: %s",
                         baseElement->name().c_str(), SDL_GetError());
        }
        if (baseElement == nullptr) return;
        SDL_SetRenderTarget(renderer, texture);
        if (!baseElement->render()) SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    void UiRenderNode::render(SDL_Renderer *renderer) {
        SDL_FRect renderFRect;
        SDL_RectToFRect(&(usedLayout.elementClippedRect), &renderFRect);
        SDL_RenderTexture(renderer, texture, nullptr, &renderFRect);
    }

} // namespace gui::element::ui::renderNode
