#include "ui_element.hpp"

namespace gui {
    namespace element {

        void UIElement::computeLayout(int availableWidth, int availableHeight) {
            int childWidth = 0;
            int childHeight = 0;
            setSize(availableWidth, availableHeight);
            availableWidth -= borderLeft() + borderRight();
            availableHeight -= borderTop() + borderBottom();
            UIElement *child = getChild();
            while (child != nullptr) {
                child->getDesiredSize(&childWidth, &childHeight);
                child->computeLayout(std::min(childWidth, availableWidth), std::min(childHeight, availableHeight));
                child = child->getNext();
            }
        }

        void UIElement::computeDesiredInnerLayout(int *desiredWidth, int *desiredHeight) {
            (*desiredWidth) = 0;
            (*desiredHeight) = 0;
            int childDesiredWidth = 0;
            int childDesiredHeight = 0;
            UIElement *child = getChild();
            while (child != nullptr) {
                child->computeDesiredLayout(&childDesiredWidth, &childDesiredHeight);
                (*desiredWidth) += childDesiredWidth;
                (*desiredHeight) += childDesiredHeight;
                child = child->getNext();
            }
        }

        void UIElement::computeDesiredLayout(int *desiredWidth, int *desiredHeight) {
            *desiredWidth = 0;
            *desiredHeight = 0;
            computeDesiredInnerLayout(desiredWidth, desiredHeight);
            (*desiredWidth) += paddingLeft() + paddingRight();
            (*desiredHeight) += paddingTop() + paddingBottom();
            (*desiredWidth) += borderLeft() + borderRight();
            (*desiredHeight) += borderTop() + borderBottom();
            UIElement *child = getChild();
            while (child != nullptr) {
                (*desiredWidth) += child->marginLeft() + child->marginRight();
                (*desiredHeight) += child->marginTop() + child->marginBottom();
                child = child->getNext();
            }
            setDesiredSize(*desiredWidth, *desiredHeight);
        }

        int UIElement::computeSize(const std::vector<const char *> &styleNames, int defaultSize, int parentSize) const {
            if (elementStyle == nullptr) return defaultSize;
            style::StyleValue *rule = nullptr;
            int size = 0;
            for (const char *styleName : styleNames) {
                if (elementStyle->getRule(styleName, &rule)) {
                    break;
                }
            }
            if (rule == nullptr) {
                return defaultSize;
            }
            if (!converter::SizeConverter::convert(rule, &size, parentSize)) {
                return defaultSize;
            }
            // TODO: add support for other values
            return size;
        }

        SDL_Color UIElement::computeColor(const std::vector<const char *> &styleNames, SDL_Color defaultColor) const {
            if (elementStyle == nullptr) return defaultColor;
            style::StyleValue *rule = nullptr;
            SDL_Color color = SDL_Color();
            for (const char *styleName : styleNames) {
                if (elementStyle->getRule(styleName, &rule)) {
                    break;
                }
            }
            if (!converter::ColorConverter::convert(rule, &color)) {
                return defaultColor;
            }
            return color;
        }

        void UIElement::getSize(int *width, int *height) const {
            *width = this->elementWidth;
            *height = this->elementHeight;
        }

        void UIElement::getDesiredSize(int *width, int *height) const {
            *width = elementDesiredWidth;
            *height = elementDesiredHeight;
        }

        void UIElement::setSize(int width, int height) {
            this->elementWidth = width;
            this->elementHeight = height;
        }

        void UIElement::setDesiredSize(int width, int height) {
            this->elementDesiredWidth = width;
            this->elementDesiredHeight = height;
        }

        SDL_FRect UIElement::createFRect(int x, int y, int width, int height) { return SDL_FRect{(float)x, (float)y, (float)width, (float)height}; }

        void UIElement::addChild(UIElement *child) {
            AbstractElement::addChild(child);
            child->setRenderer(renderer);
        }

        int UIElement::marginLeft() const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-left", "margin"}, 0, (parent == nullptr) ? 0 : parent->getWidth());
        }

        int UIElement::marginRight() const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-right", "margin"}, 0, (parent == nullptr) ? 0 : parent->getWidth());
        }

        int UIElement::marginTop() const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-top", "margin"}, 0, (parent == nullptr) ? 0 : parent->getHeight());
        }

        int UIElement::marginBottom() const {
            const UIElement *parent = getConstParent();
            return computeSize({"margin-bottom", "margin"}, 0, (parent == nullptr) ? 0 : parent->getHeight());
        }

        int UIElement::paddingLeft() const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-left", "padding"}, 0, (parent == nullptr) ? 0 : parent->getWidth());
        }

        int UIElement::paddingRight() const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-right", "padding"}, 0, (parent == nullptr) ? 0 : parent->getWidth());
        }

        int UIElement::paddingTop() const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-top", "padding"}, 0, (parent == nullptr) ? 0 : parent->getHeight());
        }

        int UIElement::paddingBottom() const {
            const UIElement *parent = getConstParent();
            return computeSize({"padding-bottom", "padding"}, 0, (parent == nullptr) ? 0 : parent->getHeight());
        }

        int UIElement::borderLeft() const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-left", "border"}, 0, (parent == nullptr) ? 0 : parent->getWidth());
        }

        int UIElement::borderRight() const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-right", "border"}, 0, (parent == nullptr) ? 0 : parent->getWidth());
        }

        int UIElement::borderTop() const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-top", "border"}, 0, (parent == nullptr) ? 0 : parent->getHeight());
        }

        int UIElement::borderBottom() const {
            const UIElement *parent = getConstParent();
            return computeSize({"border-bottom", "border"}, 0, (parent == nullptr) ? 0 : parent->getHeight());
        }

        SDL_Color UIElement::borderLeftColor() const { return computeColor({"border-left-color", "border-color"}); }

        SDL_Color UIElement::borderRightColor() const { return computeColor({"border-right-color", "border-color"}); }

        SDL_Color UIElement::borderTopColor() const { return computeColor({"border-top-color", "border-color"}); }

        SDL_Color UIElement::borderBottomColor() const { return computeColor({"border-bottom-color", "border-color"}); }

        SDL_Color UIElement::backgroundColor() const { return computeColor({"background-color"}, SDL_Color{255, 255, 255, 0}); }

        void UIElement::render() {
            if (!styleManagerAvailable()) {
                renderChilds();
                return;
            }
            if (renderer == nullptr) throw NoRendererException();
            SDL_Rect oldClipRect;
            SDL_Rect clipRect;
            SDL_Rect newClipRect;
            SDL_Rect clipRectNoBordersAndPaddings;

            if (!SDL_GetRenderClipRect(renderer, &oldClipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't get clip rect: '%s'", SDL_GetError());
                return;
            }

            clipRect = SDL_Rect{oldClipRect.x, oldClipRect.y, this->elementWidth, this->elementHeight};
            if (!SDL_GetRectIntersection(&oldClipRect, &clipRect, &newClipRect)) newClipRect = clipRect;

            clipRectNoBordersAndPaddings.x = newClipRect.x + borderLeft();
            clipRectNoBordersAndPaddings.w = newClipRect.w - borderLeft() - borderRight();
            clipRectNoBordersAndPaddings.y = newClipRect.y + borderTop();
            clipRectNoBordersAndPaddings.h = newClipRect.h - borderTop() - borderBottom();

            if (!SDL_SetRenderClipRect(renderer, &clipRectNoBordersAndPaddings)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect no borders and margins and paddings '%s'", SDL_GetError());
                return; // FIXME: restore original clip rect
            }
            renderBackground();
            renderSelfBeforeChilds();
            renderChilds();
            renderSelfAfterChilds();

            if (!SDL_SetRenderClipRect(renderer, &newClipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect '%s'", SDL_GetError());
                return; // FIXME: restore original clip rect
            }
            renderBorder();
            if (!SDL_SetRenderClipRect(renderer, &oldClipRect)) {
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
                childClipRect.w += child->marginRight();
                childClipRect.h += child->marginBottom();
                if (!SDL_GetRectIntersection(&clipRect, &childClipRect, &childFinalClipRect)) childFinalClipRect = clipRect;
                childFinalClipRect = childClipRect;
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

        void UIElement::renderBorder() const {
            SDL_Color color;
            Uint8 r, g, b, a;
            SDL_FRect fRect;
            SDL_Rect rect;
            int x, y, w, h;

            if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't get draw color '%s'", SDL_GetError());
                return;
            }

            getSize(&w, &h);
            SDL_GetRenderClipRect(renderer, &rect);
            x = rect.x;
            y = rect.y;
            // set border render order in the README

            // left border
            color = borderLeftColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(x, y, borderLeft(), h);
            SDL_RenderFillRect(renderer, &fRect);

            // right border
            color = borderRightColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(x + w - borderRight(), y, borderRight(), h);
            SDL_RenderFillRect(renderer, &fRect);

            // top border
            color = borderTopColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(x, y, w, borderTop());
            SDL_RenderFillRect(renderer, &fRect);

            // bottom border
            color = borderBottomColor();
            if (!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't set draw color '%s'", SDL_GetError());
                return;
            }
            fRect = createFRect(x, y + h - borderBottom(), w, borderBottom());
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
