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

    void UiRenderNode::computeFinalLayout(SDL_Rect clipRect, bool forceSize) {
        // TODO: overflows, ...

        usedLayout.elementRect.w = relativeSize.width;
        usedLayout.elementRect.h = relativeSize.height;

        usedLayout.contentRect.w = relativeSize.width;
        usedLayout.contentRect.h = relativeSize.height;

        usedLayout.elementClippedRect = clipRect;
        if (forceSize) {
            usedLayout.elementRect = clipRect;
        }

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

        UiRenderData data = UiRenderData(usedLayout.elementRect);

        renderElement(&data);
    }

    void UiRenderNode::createTexture() {
        nodeTexture =
            SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, usedLayout.elementRect.w, usedLayout.elementRect.h);
        if (nodeTexture == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                         "UiRenderNode::createTexture (linked to UiElement '%s'): Can't create a texture for an ui_render_node: %s",
                         baseElement->name().c_str(), SDL_GetError());
        }

        std::cerr << "elementRect: w=" << usedLayout.elementRect.w << ", h=" << usedLayout.elementRect.h << "\n";
        std::cerr << "elementClippedRect: w=" << usedLayout.elementClippedRect.w << ", h=" << usedLayout.elementClippedRect.h << "\n";
    }

    void UiRenderNode::renderElement(UiRenderData *data) {
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

        std::cerr
            << "previousClipRect: x="
            << previousClipRect.x
            << ", y="
            << previousClipRect.y
            << ", w="
            << previousClipRect.w
            << ", h="
            << previousClipRect.h
            << "\n";
        std::cerr << SDL_RenderClipEnabled(renderer) << "\n";

        SDL_SetRenderClipRect(renderer, nullptr);

        SDL_FRect renderFRect;
        SDL_RectToFRect(&(usedLayout.elementRect), &renderFRect);
        renderFRect.x = usedLayout.startCoords.x;
        renderFRect.y = usedLayout.startCoords.y;
        std::cerr << "renderFRect: x=" << renderFRect.x << ", y=" << renderFRect.y << ", w=" << renderFRect.w << ", h=" << renderFRect.h << "\n";
        SDL_RenderTexture(renderer, nodeTexture, nullptr, &renderFRect);

        if (!SDL_SetRenderClipRect(renderer, &previousClipRect)) {
            SDL_LogError(GUI_RENDERING, "UiRenderNode::renderElement: Can't restore clip rect: '%s'", SDL_GetError());
        }
    }

    bool UiRenderNode::renderChildElement(const UiElement *element, UiRenderData *data) {
        UiRenderNode *node = child();
        while (node != nullptr) {
            if (node->baseElement == element) {
                std::cerr << "UiRenderNode: rendering element '" << baseElement->name() << "'\n";
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
                std::cerr << "UiRenderNode: getting data of child '" << node->baseElement->name() << "'\n";
                return new UiElementData({node->usedLayout.elementRect.w, node->usedLayout.elementRect.h});
            }
            node = node->constNext();
        }
        return nullptr;
    }
} // namespace gui::element::ui::renderNode
