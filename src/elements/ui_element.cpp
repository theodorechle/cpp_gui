#include "ui_element.hpp"

namespace gui {
    namespace element {

        void UIElement::computeLayout() {
            // int w, h;
            // SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
            int width, height;
            computeDesiredLayout(&width, &height);
            setSize(width, height);
        }

        void UIElement::computeDesiredLayout(int *width, int *height) {
            *width = 0;
            *height = 0;
            computeDesiredLayoutWithoutMargins(width, height);
            (*width) += paddingLeft() + paddingRight();
            (*height) += paddingTop() + paddingBottom();
            (*width) += borderLeft() + borderRight();
            (*height) += borderTop() + borderBottom();
        }

        int UIElement::getSize(const std::vector<const char *> &styleNames, int defaultSize) const {
            style::StyleValue *rule = nullptr;
            for (const char *styleName : styleNames) {
                if (elementStyle->getRule(styleName, &rule)) {
                    break;
                }
            }
            if (rule == nullptr) {
                return defaultSize;
            }

            if (rule->getType() == style::StyleValueType::PixelUnit) {
                return (rule->getChild()->getType() == style::StyleValueType::Int) ? (std::stoul(rule->getChild()->getValue(), nullptr, 10))
                                                                                   : defaultSize;
            }
            // TODO: add support for other values
            return defaultSize;
        }

        SDL_Color UIElement::getColor(const std::vector<const char *> &styleNames, SDL_Color defaultColor) const {
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

        void UIElement::getPos(int *x, int *y) const {
            *x = elementRect.x;
            *y = elementRect.y;
        }

        void UIElement::getSize(int *width, int *height) const {
            *width = elementRect.w;
            *height = elementRect.h;
        }

        void UIElement::getRect(int *x, int *y, int *width, int *height) const {
            *x = elementRect.x;
            *y = elementRect.y;
            *width = elementRect.w;
            *height = elementRect.h;
        }

        void UIElement::setPos(int x, int y) {
            elementRect.x = x;
            elementRect.y = y;
        }

        void UIElement::setSize(int width, int height) {
            elementRect.w = width;
            elementRect.h = height;
        }

        void UIElement::setRect(int x, int y, int width, int height) {
            elementRect.x = x;
            elementRect.y = y;
            elementRect.w = width;
            elementRect.h = height;
        }

        void UIElement::setRect(const SDL_Rect &rect) { elementRect = rect; }

        SDL_FRect UIElement::createFRect(int x, int y, int width, int height) { return SDL_FRect{(float)x, (float)y, (float)width, (float)height}; }

        void UIElement::addChild(UIElement *child) {
            AbstractElement::addChild(child);
            child->setRenderer(renderer);
        }

        int UIElement::marginLeft() const { return getSize({"margin-left", "margin"}); }

        int UIElement::marginRight() const { return getSize({"margin-right", "margin"}); }

        int UIElement::marginTop() const { return getSize({"margin-top", "margin"}); }

        int UIElement::marginBottom() const { return getSize({"margin-bottom", "margin"}); }

        int UIElement::paddingLeft() const { return getSize({"padding-left", "padding"}); }

        int UIElement::paddingRight() const { return getSize({"padding-right", "padding"}); }

        int UIElement::paddingTop() const { return getSize({"padding-top", "padding"}); }

        int UIElement::paddingBottom() const { return getSize({"padding-bottom", "padding"}); }

        int UIElement::borderLeft() const { return getSize({"border-left", "border"}); }

        int UIElement::borderRight() const { return getSize({"border-right", "border"}); }

        int UIElement::borderTop() const { return getSize({"border-top", "border"}); }

        int UIElement::borderBottom() const { return getSize({"border-bottom", "border"}); }

        SDL_Color UIElement::borderLeftColor() const { return getColor({"border-left-color", "border-color"}); }

        SDL_Color UIElement::borderRightColor() const { return getColor({"border-right-color", "border-color"}); }

        SDL_Color UIElement::borderTopColor() const { return getColor({"border-top-color", "border-color"}); }

        SDL_Color UIElement::borderBottomColor() const { return getColor({"border-bottom-color", "border-color"}); }

        SDL_Color UIElement::backgroundColor() const { return getColor({"background-color"}, SDL_Color{255, 255, 255, 0}); }

        void UIElement::render() {
            if (renderer == nullptr) throw NoRendererException();
            computeLayout();
            SDL_Rect oldClipRect;
            SDL_Rect clipRect = getRect();
            SDL_Rect newClipRect;
            SDL_Rect clipRectNoBordersAndPaddingsAndMargins;

            if (!SDL_GetRenderClipRect(renderer, &oldClipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't get clip rect: '%s'", SDL_GetError());
                return;
            }

            clipRect.x += oldClipRect.x;
            clipRect.y += oldClipRect.y;
            if (!SDL_GetRectIntersection(&oldClipRect, &clipRect, &newClipRect)) newClipRect = clipRect;

            clipRectNoBordersAndPaddingsAndMargins.x = newClipRect.x + borderLeft();
            clipRectNoBordersAndPaddingsAndMargins.w = newClipRect.w - borderLeft() - borderRight();
            clipRectNoBordersAndPaddingsAndMargins.y = newClipRect.y + borderTop();
            clipRectNoBordersAndPaddingsAndMargins.h = newClipRect.h - borderTop() - borderBottom();

            if (!SDL_SetRenderClipRect(renderer, &clipRectNoBordersAndPaddingsAndMargins)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect no borders and margins and paddings '%s'", SDL_GetError());
                return;
            }
            // TODO: compute margins
            // TODO: give only a portion of the surface to the elements (and childs)
            renderBackground();
            renderSelfBeforeChilds();
            renderChilds();
            renderSelfAfterChilds();

            if (!SDL_SetRenderClipRect(renderer, &newClipRect)) {
                SDL_LogError(SDL_LOG_CATEGORY_ERROR, "can't set clip rect '%s'", SDL_GetError());
                return;
            }
            renderBorder();
            if (!SDL_SetRenderClipRect(renderer, &oldClipRect)) {
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

            getRect(&x, &y, &w, &h);
            SDL_GetRenderClipRect(renderer, &rect);
            x += rect.x;
            y += rect.y;

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
