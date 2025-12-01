#include "ui_render_node.hpp"
#include "../label.hpp"

namespace gui::element::ui::render {
    UiRenderNode::UiRenderNode(SDL_Renderer *renderer, UiRenderNode *parentNode, gui::element::UiElement *baseElement)
        : renderer{renderer}, baseElement{baseElement} {
        parent(parentNode);
    }

    UiRenderNode::~UiRenderNode() { SDL_DestroyTexture(nodeTexture); }

    void UiRenderNode::computeSelfLayout() {
        if (baseElement == nullptr) return;
        baseElement->computeInnerLayout(&(defaultSelfSize.width), &(defaultSelfSize.height));
        baseElement->computeTotalLayout(&(defaultSelfSize.width), &(defaultSelfSize.height));
    }

    void UiRenderNode::computeSelfAndChildsLayout() {
        if (baseElement == nullptr) return;
        std::list<std::tuple<int, int>> childsSizes = {};
        UiRenderNode *childNode = child();
        while (childNode != nullptr) {
            childNode->computeSelfAndChildsLayout();
            childsSizes.push_back(std::tuple<int, int>(childNode->defaultSizeWithChilds.width, childNode->defaultSizeWithChilds.height));
            childNode = childNode->next();
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
        UiRenderNode *nodeChild = child();
        while (nodeChild != nullptr) {
            nodeChild->computeRelativeLayout();
            childsSizes.push_back(std::tuple<int, int>(nodeChild->relativeSize.width, nodeChild->relativeSize.height));
            nodeChild = nodeChild->next();
        }

        baseElement->computeSelfAndChildsLayout(&(relativeSize.width), &(relativeSize.height), &(defaultSizeWithChilds.width),
                                                &(defaultSizeWithChilds.height), childsSizes);
        baseElement->computeTotalLayout(&(relativeSize.width), &(relativeSize.height));
    }

    const SDL_Rect *UiRenderNode::elementRect() const { return &usedLayout.elementRect; }

    const SDL_Rect *UiRenderNode::elementClippedRect() const { return &usedLayout.elementClippedRect; }

    const Pos *UiRenderNode::startCoords() const { return &usedLayout.startCoords; }

    void UiRenderNode::computeFinalLayout(SDL_Rect *clipRect, bool forceSize) {
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
        data.elementRect.x += usedLayout.startCoords.x;
        data.elementRect.y += usedLayout.startCoords.y;

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

        UiElementData elementData = UiElementData({usedLayout.elementRect.w, usedLayout.elementRect.h},
                                                  {usedLayout.elementClippedRect.w, usedLayout.elementClippedRect.h}, usedLayout.scrollOffset);

        if (!baseElement->render(
                &elementData,
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

        std::cerr
            << "rects ("
            << baseElement->name()
            << "): {"
            << usedLayout.elementRect.w
            << ","
            << usedLayout.elementRect.h
            << "}"
            << "; {"
            << usedLayout.elementClippedRect.w
            << ","
            << usedLayout.elementClippedRect.h
            << "}\n";
        SDL_FRect sourceFRect;
        SDL_RectToFRect(&(usedLayout.elementClippedRect), &sourceFRect);
        sourceFRect.x = usedLayout.scrollOffset.x;
        sourceFRect.y = usedLayout.scrollOffset.y;

        SDL_FRect destFRect;
        SDL_RectToFRect(&(usedLayout.elementClippedRect), &destFRect);
        destFRect.x = usedLayout.startCoords.x;
        destFRect.y = usedLayout.startCoords.y;
        if (!SDL_RenderTexture(renderer, nodeTexture, &sourceFRect, &destFRect)) {
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

    bool UiRenderNode::scroll(int x, int y) {
        if ((x != 0 && usedLayout.elementClippedRect.w != usedLayout.elementRect.w)
            || (y != 0 && usedLayout.elementClippedRect.h != usedLayout.elementRect.h)) {
            usedLayout.scrollOffset = {
                std::max(std::min(usedLayout.scrollOffset.x + x, usedLayout.elementRect.w - usedLayout.elementClippedRect.w), 0),
                std::max(std::min(usedLayout.scrollOffset.y + y, usedLayout.elementRect.h - usedLayout.elementClippedRect.h), 0)};
            return true;
        }
        return false;
    }

    const UiElementData *UiRenderNode::childData(const UiElement *childElement) const {
        const UiRenderNode *node = child();
        while (node != nullptr) {
            if (node->baseElement == childElement) {
                return new UiElementData({node->usedLayout.elementRect.w, node->usedLayout.elementRect.h},
                                         {node->usedLayout.elementClippedRect.w, node->usedLayout.elementClippedRect.h},
                                         node->usedLayout.scrollOffset);
            }
            node = node->next();
        }
        return nullptr;
    }

    std::string UiRenderNode::debugValue() const {
        std::stringstream stringStream;
        stringStream
            << "("
            << this
            << ") "
            << baseElement->name()
            << "{x="
            << usedLayout.startCoords.x
            << ",y="
            << usedLayout.startCoords.y
            << ",w="
            << usedLayout.elementClippedRect.w
            << ",h="
            << usedLayout.elementClippedRect.h
            << "}";
        if (baseElement->name() == "label") stringStream << " (" << static_cast<gui::element::Label *>(baseElement)->getText() << ")";
        return stringStream.str();
    }

    bool UiRenderNode::isParentOf(const UiRenderNode *node) const {
        while (node) {
            if (node == this) return true;
            node = node->parent();
        }
        return false;
    }

} // namespace gui::element::ui::render
