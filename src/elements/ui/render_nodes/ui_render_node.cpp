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
        baseElement->computeSelfLayout(&(defaultSelfSize.width), &(defaultSelfSize.height));
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

        baseElement->computeSelfAndChildsLayout(&(defaultSelfSize.width), &(defaultSelfSize.height), childsSizes);
    }

    void UiRenderNode::computeRelativeLayout() {
        if (baseElement == nullptr) return;
        baseElement->computeSelfLayout(&(relativeSize.width), &(relativeSize.height));
    }

    void UiRenderNode::computeFinalLayout() {
        // overflows, ...
        // TODO: remove this code
        usedLayout.elementRect.w = relativeSize.width;
        usedLayout.elementRect.h = relativeSize.height;
        usedLayout.contentRect.w = relativeSize.width;
        usedLayout.contentRect.h = relativeSize.height;
    }

    void UiRenderNode::updateTexture(bool recursive) {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, usedLayout.elementRect.w, usedLayout.elementRect.h);
        if (texture == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't create a texture for an ui_render_node");
        }
        if (baseElement == nullptr) return;
        baseElement->render();
    }

    void UiRenderNode::render(SDL_Renderer *renderer) {
        SDL_FRect renderFRect;
        SDL_RectToFRect(&(usedLayout.elementRect), &renderFRect);
        SDL_RenderTexture(renderer, texture, nullptr, &renderFRect);
    }

} // namespace gui::element::ui::renderNode
