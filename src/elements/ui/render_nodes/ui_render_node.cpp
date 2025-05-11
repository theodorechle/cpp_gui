#include "ui_render_node.hpp"

namespace gui::element::ui::render {
    UiRenderNode::UiRenderNode(SDL_Renderer *renderer, UiRenderNode *parent, gui::element::UiElement *baseElement)
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

    const SDL_Rect *UiRenderNode::elementRect() const { return &usedLayout.elementRect; }

    const SDL_Rect *UiRenderNode::elementClippedRect() const { return &usedLayout.elementClippedRect; }

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

        UiRenderNode *childNode = child();
        SDL_Rect childClipRect;
        while (childNode != nullptr) {
            childClipRect = {0, 0, childNode->relativeSize.width, childNode->relativeSize.height};
            childNode->computeFinalLayout(computeNewClipRect(&clipRect, &childClipRect));
            childNode = childNode->next();
        }
    }

    SDL_Rect UiRenderNode::computeNewClipRect(SDL_Rect *oldClipRect, SDL_Rect *wantedNewClipRect) { // maybe in utils since also used in UiElement
        return SDL_Rect{std::max(oldClipRect->x, wantedNewClipRect->x), std::max(oldClipRect->y, wantedNewClipRect->y),
                        std::min(oldClipRect->w, wantedNewClipRect->w), std::min(oldClipRect->h, wantedNewClipRect->h)};
    }

    void UiRenderNode::initBeforeLayoutComputing() {
        baseElement->initBeforeLayoutComputing();
        UiRenderNode *renderNode = child();
        while (renderNode != nullptr) {
            renderNode->initBeforeLayoutComputing();
            renderNode = renderNode->next();
        }
    }

    void UiRenderNode::restoreAfterLayoutComputing() {
        baseElement->restoreAfterLayoutComputing();
        UiRenderNode *renderNode = child();
        while (renderNode != nullptr) {
            renderNode->restoreAfterLayoutComputing();
            renderNode = renderNode->next();
        }
    }

    void UiRenderNode::render(bool recursive) {
        if (baseElement == nullptr) return;

        UiRenderData data = UiRenderData(usedLayout.elementClippedRect);

        if (!renderElement(&data)) SDL_RenderClear(renderer);
    }

    bool UiRenderNode::renderElement(UiRenderData *data) const {
        // restore clip rect, get the size from UiRenderNode, but find a way to give coordinates

        SDL_Rect previousClipRect;
        if (!SDL_GetRenderClipRect(renderer, &previousClipRect)) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderElement: Can't get clip rect: '%s'", SDL_GetError());
            return false;
        }

        std::cerr
            << "set clip rect <"
            << usedLayout.elementClippedRect.x
            << ","
            << usedLayout.elementClippedRect.y
            << ","
            << usedLayout.elementClippedRect.w
            << ","
            << usedLayout.elementClippedRect.h
            << "> for element '"
            << baseElement->name()
            << "'\n";
        if (!SDL_SetRenderClipRect(renderer, &(usedLayout.elementClippedRect))) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderElement: Can't set clip rect: '%s'", SDL_GetError());
            return false;
        }

        bool rendered = baseElement->render(
            [this](const AbstractElement *element, RenderData *data) {
                return this->renderChildElement(static_cast<const UiElement *>(element), static_cast<UiRenderData *>(data));
            },
            [this](const AbstractElement *element) { return this->childData(static_cast<const UiElement *>(element)); });

        if (!SDL_SetRenderClipRect(renderer, &previousClipRect)) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderElement: Can't restore clip rect: '%s'", SDL_GetError());
            return false;
        }

        return rendered;
    }

    bool UiRenderNode::renderChildElement(const UiElement *element, UiRenderData *data) const {
        const UiRenderNode *node = constChild();
        while (node != nullptr) {
            if (node->baseElement == element) {
                std::cerr << "UiRenderNode: rendering element '" << baseElement->name() << "'\n";
                node->renderElement(data);
            }
            node = node->constNext();
        }
        return false;
    }

    const UiElementData *UiRenderNode::childData(const UiElement *child) const {
        const UiRenderNode *node = constChild();
        while (node != nullptr) {
            if (node->baseElement == child) {
                std::cerr << "UiRenderNode: getting data of child '" << baseElement->name() << "'\n";
                return new UiElementData({node->usedLayout.elementClippedRect.w, node->usedLayout.elementClippedRect.h});
            }
            node = node->constNext();
        }
        return nullptr;
    }

} // namespace gui::element::ui::renderNode
