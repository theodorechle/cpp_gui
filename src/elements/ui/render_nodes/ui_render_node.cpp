#include "ui_render_node.hpp"

namespace gui::element::ui::render {
    UiRenderNode::UiRenderNode(SDL_Renderer *renderer, UiRenderNode *parent, gui::element::UiElement *baseElement)
        : renderer{renderer}, _parent{parent}, baseElement{baseElement} {}

    UiRenderNode::~UiRenderNode() {
        SDL_DestroyTexture(nodeTexture);
        delete _child;
        delete _next;
    }

    void UiRenderNode::removeChilds() { _child = nullptr; }

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

        baseElement->computeSelfAndChildsLayout(&(defaultSizeWithChilds.width), &(defaultSizeWithChilds.height), &(defaultSelfSize.width),
                                                &(defaultSelfSize.height), childsSizes);
        baseElement->computeTotalLayout(&(defaultSizeWithChilds.width), &(defaultSizeWithChilds.height));
    }

    void UiRenderNode::computeRelativeLayout() {
        if (baseElement == nullptr) return;
        // set variables like rem, em, ...
        // percentages should be set at this moment
        std::list<std::tuple<int, int>> childsSizes = {};
        UiRenderNode *child = _child;
        while (child != nullptr) {
            child->computeRelativeLayout();
            childsSizes.push_back(std::tuple<int, int>(child->relativeSize.width, child->relativeSize.height));
            child = child->_next;
        }

        baseElement->computeSelfAndChildsLayout(&(relativeSize.width), &(relativeSize.height), &(defaultSizeWithChilds.width),
                                                &(defaultSizeWithChilds.height), childsSizes);
        baseElement->computeTotalLayout(&(relativeSize.width), &(relativeSize.height));
    }

    const SDL_Rect *UiRenderNode::elementRect() const { return &usedLayout.elementRect; }

    const SDL_Rect *UiRenderNode::elementClippedRect() const { return &usedLayout.elementClippedRect; }

    const Pos *UiRenderNode::startCoords() const { return &usedLayout.startCoords; }

    void UiRenderNode::computeFinalLayout(SDL_Rect *clipRect, bool forceSize) {
        // TODO: overflows, ...

        usedLayout.elementRect.w = relativeSize.width;
        usedLayout.elementRect.h = relativeSize.height;

        usedLayout.contentRect.w = relativeSize.width;
        usedLayout.contentRect.h = relativeSize.height;

        if (clipRect == nullptr) {
            usedLayout.elementClippedRect = usedLayout.elementRect;
        }
        else {
            usedLayout.elementClippedRect = *clipRect;
        }
        if (forceSize && clipRect != nullptr) {
            usedLayout.elementRect = *clipRect;
        }

        UiRenderNode *childNode = child();
        SDL_Rect childClipRect;
        SDL_Rect newClipRect;
        while (childNode != nullptr) {
            childClipRect = {0, 0, childNode->relativeSize.width, childNode->relativeSize.height};
            if (clipRect != nullptr) {
                newClipRect = computeNewClipRect(clipRect, &childClipRect);
            }
            childNode->computeFinalLayout(&newClipRect);
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

        UiRenderData data = UiRenderData(usedLayout.elementRect);

        renderElement(&data);
    }

    void UiRenderNode::createTexture() {
        SDL_DestroyTexture(nodeTexture);
        nodeTexture =
            SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, usedLayout.elementRect.w, usedLayout.elementRect.h);
        if (nodeTexture == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                         "UiRenderNode::createTexture (linked to UiElement '%s'): Can't create a texture for an ui_render_node: %s",
                         baseElement->name().c_str(), SDL_GetError());
        }
    }

    void UiRenderNode::renderElement(UiRenderData *data) {
        if (nodeTexture == nullptr) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderOnParentSurface: texture is null, maybe node is 0-sized");
            return;
        }
        usedLayout.startCoords = Pos{data->elementRect.x, data->elementRect.y};
        SDL_Texture *parentTexture = SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer, nodeTexture);
        SDL_SetRenderClipRect(renderer, &(usedLayout.elementRect)); // TODO: get rid of get clip rects and get texture size instead

        if (!baseElement->render(
                [this](const AbstractElement *element, RenderData *elementData) {
                    return this->renderChildElement(static_cast<const UiElement *>(element), static_cast<UiRenderData *>(elementData));
                },
                [this](const AbstractElement *element) { return this->childData(static_cast<const UiElement *>(element)); })) {
            SDL_RenderClear(renderer);
        }

        SDL_SetRenderTarget(renderer, parentTexture);
        renderOnParentSurface();
    }

    void UiRenderNode::renderOnParentSurface() const {
        SDL_Rect previousClipRect;
        if (!SDL_GetRenderClipRect(renderer, &previousClipRect)) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderElement: Can't get clip rect: '%s'", SDL_GetError());
            return;
        }

        SDL_SetRenderClipRect(renderer, nullptr);

        SDL_FRect renderFRect;
        SDL_RectToFRect(&(usedLayout.elementRect), &renderFRect);
        renderFRect.x = usedLayout.startCoords.x;
        renderFRect.y = usedLayout.startCoords.y;
        if (!SDL_RenderTexture(renderer, nodeTexture, nullptr, &renderFRect)) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderOnParentSurface: Can't render current surface on parent surface : '%s'", SDL_GetError());
        }

        if (!SDL_SetRenderClipRect(renderer, &previousClipRect)) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderOnParentSurface: Can't restore clip rect: '%s'", SDL_GetError());
        }
    }

    bool UiRenderNode::renderChildElement(const UiElement *element, UiRenderData *data) {
        UiRenderNode *node = child();
        while (node != nullptr) {
            if (node->baseElement == element) {
                node->renderElement(data);
            }
            node = node->next();
        }
        return false;
    }

    const UiElementData *UiRenderNode::childData(const UiElement *child) const {
        const UiRenderNode *node = constChild();
        while (node != nullptr) {
            if (node->baseElement == child) {
                return new UiElementData({node->usedLayout.elementRect.w, node->usedLayout.elementRect.h});
            }
            node = node->constNext();
        }
        return nullptr;
    }
} // namespace gui::element::ui::renderNode
