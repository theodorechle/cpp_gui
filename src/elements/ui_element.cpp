#include "ui_element.hpp"

namespace gui {
    namespace element {

        void UIElement::computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) {
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UIElement *child = getChild();
            while (child != nullptr) {
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                if (!child->sizeParentRelative) {
                    (*desiredWidth) += childDesiredWidth;
                    (*desiredHeight) += childDesiredHeight;
                }
                child = child->getNext();
            }
        }

        void UIElement::computeDesiredLayout(int *desiredWidth, int *desiredHeight) {
            sizeParentRelative = false;
            *desiredWidth = 0;
            *desiredHeight = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            computeDesiredInnerLayout(desiredWidth, desiredHeight);
            (*desiredWidth) += paddingLeft(&sizeParentRelative) + paddingRight(&sizeParentRelative);
            (*desiredHeight) += paddingTop(&sizeParentRelative) + paddingBottom(&sizeParentRelative);
            (*desiredWidth) += borderLeft(&sizeParentRelative) + borderRight(&sizeParentRelative);
            (*desiredHeight) += borderTop(&sizeParentRelative) + borderBottom(&sizeParentRelative);
            UIElement *child = getChild();
            while (child != nullptr) {
                childDesiredWidth = child->marginLeft(&child->sizeParentRelative) + child->marginRight(&child->sizeParentRelative);
                childDesiredHeight = child->marginTop(&child->sizeParentRelative) + child->marginBottom(&child->sizeParentRelative);
                if (!child->sizeParentRelative) {
                    (*desiredWidth) += childDesiredWidth;
                    (*desiredHeight) += childDesiredHeight;
                }
                child = child->getNext();
            }
            setDesiredSize(*desiredWidth, *desiredHeight);
        }

        bool UIElement::areAllParentSizesParentRelative() const {
            const UIElement *parent = getConstParent();
            while (parent != nullptr) {
                if (!parent->isSizeParentRelative()) return false;
                parent = parent->getConstParent();
            }
            return true;
        }

        void UIElement::computeLayout(int x, int y, int availableWidth, int availableHeight) {
            int childWidth = 0;
            int childHeight = 0;
            setPos(x, y);
            setSize(availableWidth, availableHeight);
            x += borderLeft() + marginLeft();
            y += borderTop() + marginTop();
            availableWidth -= borderLeft() + borderRight();
            availableHeight -= borderTop() + borderBottom();
            UIElement *child = getChild();
            while (child != nullptr) {
                child->getDesiredSize(&childWidth, &childHeight);
                child->computeLayout(x, y, std::min(childWidth, availableWidth), std::min(childHeight, availableHeight));
                child = child->getNext();
            }
        }

        void UIElement::setRect(SDL_Rect rect) { elementRect = rect; }

        void UIElement::setPos(int x, int y) {
            elementRect.x = x;
            elementRect.y = y;
        }

        void UIElement::getRect(SDL_Rect *rect) const { *rect = this->elementRect; }

        void UIElement::getPos(int *x, int *y) const {
            *x = elementRect.x;
            *y = elementRect.y;
        }

        void UIElement::getSize(int *width, int *height) const {
            *width = this->elementRect.w;
            *height = this->elementRect.h;
        }

        void UIElement::getDesiredSize(int *width, int *height) const {
            *width = elementDesiredWidth;
            *height = elementDesiredHeight;
        }

        void UIElement::setSize(int width, int height) {
            this->elementRect.w = width;
            this->elementRect.h = height;
        }

        void UIElement::setDesiredSize(int width, int height) {
            this->elementDesiredWidth = width;
            this->elementDesiredHeight = height;
        }

        SDL_Rect UIElement::computeNewClipRect(SDL_Rect *oldClipRect, SDL_Rect *wantedNewClipRect) {
            return SDL_Rect{std::max(oldClipRect->x, wantedNewClipRect->x), std::max(oldClipRect->y, wantedNewClipRect->y),
                            std::min(oldClipRect->w, wantedNewClipRect->w), std::min(oldClipRect->h, wantedNewClipRect->h)};
        }

        SDL_FRect UIElement::createFRect(int x, int y, int width, int height) { return SDL_FRect{(float)x, (float)y, (float)width, (float)height}; }

        int UIElement::getIntFromRule(const std::vector<std::string> &styleNames, int defaultSize, bool canInherit) const {
            if (elementStyle == nullptr) return defaultSize;
            style::StyleValue *rule = nullptr;
            int size = 0;
            elementStyle->getRule(styleNames, &rule, canInherit);
            if (rule == nullptr) {
                return defaultSize;
            }
            if (!converter::NumberConverter::convertToInt(rule, &size)) {
                return defaultSize;
            }
            return size;
        }

        std::string UIElement::getStringFromRule(const std::vector<std::string> &styleNames, const std::string &defaultString,
                                                 bool canInherit) const {
            if (elementStyle == nullptr) return defaultString;
            style::StyleValue *rule = nullptr;
            elementStyle->getRule(styleNames, &rule, canInherit);
            if (rule == nullptr) {
                return defaultString;
            }
            return rule->getValue();
        }

        int UIElement::computeSize(const std::vector<std::string> &styleNames, int defaultSize, bool canInherit, int parentSize,
                                   bool *relativeSize) const {
            if (elementStyle == nullptr) return defaultSize;
            style::StyleValue *rule = nullptr;
            int size = 0;
            elementStyle->getRule(styleNames, &rule, canInherit);
            if (rule == nullptr) {
                return defaultSize;
            }
            if (!converter::SizeConverter::convert(rule, &size, parentSize, relativeSize, areAllParentSizesParentRelative())) {
                return defaultSize;
            }
            return size;
        }

        SDL_Color UIElement::computeColor(const std::vector<std::string> &styleNames, SDL_Color defaultColor, bool canInherit) const {
            if (elementStyle == nullptr) return defaultColor;
            style::StyleValue *rule = nullptr;
            SDL_Color color = SDL_Color();
            elementStyle->getRule(styleNames, &rule, canInherit);
            if (rule == nullptr) {
                return defaultColor;
            }
            if (!converter::ColorConverter::convert(rule, &color)) {
                return defaultColor;
            }
            return color;
        }

        void UIElement::addChild(UIElement *child) {
            AbstractElement::addChild(child);
            child->setRenderer(renderer);
        }

        int UIElement::marginLeft(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-left", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), relativeSize);
        }

        int UIElement::marginRight(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-right", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), relativeSize);
        }

        int UIElement::marginTop(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-top", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), relativeSize);
        }

        int UIElement::marginBottom(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-bottom", "margin"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), relativeSize);
        }

        int UIElement::paddingLeft(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-left", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), relativeSize);
        }

        int UIElement::paddingRight(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-right", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), relativeSize);
        }

        int UIElement::paddingTop(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-top", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), relativeSize);
        }

        int UIElement::paddingBottom(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-bottom", "padding"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), relativeSize);
        }

        int UIElement::borderLeft(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-left", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), relativeSize);
        }

        int UIElement::borderRight(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-right", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getWidth(), relativeSize);
        }

        int UIElement::borderTop(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-top", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), relativeSize);
        }

        int UIElement::borderBottom(bool *relativeSize) const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-bottom", "border"}, 0, false, (parent == nullptr) ? 0 : parent->getHeight(), relativeSize);
        }

        SDL_Color UIElement::borderLeftColor() const { return computeColor({"border-left-color", "border-color"}); }

        SDL_Color UIElement::borderRightColor() const { return computeColor({"border-right-color", "border-color"}); }

        SDL_Color UIElement::borderTopColor() const { return computeColor({"border-top-color", "border-color"}); }

        SDL_Color UIElement::borderBottomColor() const { return computeColor({"border-bottom-color", "border-color"}); }

        SDL_Color UIElement::backgroundColor() const { return computeColor({"background-color"}, SDL_Color{255, 255, 255, 0}); }

        void UIElement::tryRender(SDL_Rect oldClipRect) {
            if (!styleManagerAvailable()) {
                renderChilds();
                return;
            }
            if (renderer == nullptr) throw NoRendererException();
            SDL_Rect clipRect;
            SDL_Rect finalClipRect;
            SDL_Rect clipRectNoBorders;
            SDL_Rect finalClipRectNoBorders;
            SDL_Rect clipRectNoPaddings;
            SDL_Rect finalClipRectNoPaddings;

            clipRect = SDL_Rect{oldClipRect.x, oldClipRect.y, this->elementRect.w, this->elementRect.h};
            finalClipRect = computeNewClipRect(&oldClipRect, &clipRect);

            clipRectNoBorders.x = clipRect.x + borderLeft();
            clipRectNoBorders.w = clipRect.w - borderLeft() - borderRight();
            clipRectNoBorders.y = clipRect.y + borderTop();
            clipRectNoBorders.h = clipRect.h - borderTop() - borderBottom();

            finalClipRectNoBorders = computeNewClipRect(&oldClipRect, &clipRectNoBorders);
            if (!SDL_SetRenderClipRect(renderer, &finalClipRectNoBorders)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect no borders '%s'", SDL_GetError());
                return;
            }
            renderBackground();

            clipRectNoPaddings.x = clipRectNoBorders.x + paddingLeft();
            clipRectNoPaddings.w = clipRectNoBorders.w - paddingLeft() - paddingRight();
            clipRectNoPaddings.y = clipRectNoBorders.y + paddingTop();
            clipRectNoPaddings.h = clipRectNoBorders.h - paddingTop() - paddingBottom();
            finalClipRectNoPaddings = computeNewClipRect(&oldClipRect, &clipRectNoPaddings);

            if (!SDL_SetRenderClipRect(renderer, &finalClipRectNoPaddings)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect no paddings '%s'", SDL_GetError());
                return;
            }

            renderSelfBeforeChilds();
            renderChilds();
            renderSelfAfterChilds();

            if (!SDL_SetRenderClipRect(renderer, &finalClipRect)) { // ensure rect has correct size
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect '%s'", SDL_GetError());
                return;
            }
            renderBorders();
        }

        void UIElement::render() {
            SDL_Rect clipRect;

            if (!SDL_GetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't get clip rect: '%s'", SDL_GetError());
                return;
            }

            tryRender(clipRect);

            if (!SDL_SetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't restore clip rect '%s'", SDL_GetError());
                return;
            }
        }

        void UIElement::renderChilds() {
            SDL_Rect clipRect;
            SDL_Rect childClipRect;
            SDL_Rect childFinalClipRect;
            UIElement *child = getChild();
            if (!SDL_GetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't get clip rect: '%s'", SDL_GetError());
                return;
            }
            while (child != nullptr) {
                childClipRect = SDL_Rect{clipRect.x, clipRect.y, child->getWidth(), child->getHeight()};
                childClipRect.x += child->marginLeft();
                childClipRect.y += child->marginTop();
                childClipRect.w = std::min(childClipRect.w + child->marginRight(), clipRect.w - (childClipRect.x - clipRect.x));
                childClipRect.h = std::min(childClipRect.h + child->marginBottom(), clipRect.h - (childClipRect.y - clipRect.y));
                childFinalClipRect = computeNewClipRect(&clipRect, &childClipRect);
                if (!SDL_SetRenderClipRect(renderer, &childFinalClipRect)) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect '%s'", SDL_GetError());
                    break;
                }
                child->render();
                child = child->getNext();
            }
            if (!SDL_SetRenderClipRect(renderer, &clipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't restore clip rect '%s'", SDL_GetError());
                return;
            }
        }

        void UIElement::renderBorders() const {
            SDL_Color color;
            Uint8 r, g, b, a;
            SDL_FRect fRect;
            SDL_Rect clipRect;
            SDL_Rect realRect;
            SDL_Rect borderClippedRect;
            int bLeft = borderLeft();
            int bRight = borderRight();
            int bTop = borderTop();
            int bBottom = borderBottom();

            if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't get draw color '%s'", SDL_GetError());
                return;
            }
            getRect(&realRect);
            SDL_GetRenderClipRect(renderer, &clipRect);
            borderClippedRect = computeNewClipRect(&clipRect, &realRect);
            // set border render order in the README

            // left border
            color = borderLeftColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(borderClippedRect.x, borderClippedRect.y + bTop, bLeft, borderClippedRect.h - bTop - bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // right border
            color = borderRightColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(borderClippedRect.x + borderClippedRect.w, borderClippedRect.y + bTop, -bRight, borderClippedRect.h - bTop - bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // top border
            color = borderTopColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(borderClippedRect.x, borderClippedRect.y, borderClippedRect.w, bTop);
            SDL_RenderFillRect(renderer, &fRect);

            // bottom border
            color = borderBottomColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(borderClippedRect.x, borderClippedRect.y + borderClippedRect.h, borderClippedRect.w, -bBottom);
            SDL_RenderFillRect(renderer, &fRect);

            // restore previous color
            if (!SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
        }

        void UIElement::renderBackground() const {
            Uint8 r, g, b, a;
            SDL_Rect rect;
            SDL_FRect fRect;

            if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't get draw color '%s'", SDL_GetError());
                return;
            }

            SDL_Color color = backgroundColor();
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_GetRenderClipRect(renderer, &rect);

            SDL_RectToFRect(&rect, &fRect);
            SDL_RenderFillRect(renderer, &fRect);

            if (!SDL_SetRenderDrawColor(renderer, r, g, b, a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
        }

    } // namespace element
} // namespace gui
