#include "ui_element.hpp"

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
    (*width) -= borderLeft() + borderRight();
    (*height) -= borderTop() + borderBottom();
}

int UIElement::getSize(const std::vector<const char *> &styleNames, int defaultSize) const {
    StyleValue *rule = nullptr;
    for (const char *styleName : styleNames) {
        if (elementStyle->getRule(styleName, &rule)) {
            break;
        }
    }
    if (rule == nullptr) {
        return defaultSize;
    }

    if (rule->getType() == StyleValueType::PixelUnit) {
        return (rule->getChild()->getType() == StyleValueType::Int) ? (std::stoul(rule->getChild()->getValue(), nullptr, 10)) : defaultSize;
    }
    // TODO: add support for other values
    return defaultSize;
}

SDL_Color UIElement::getColor(const std::vector<const char *> &styleNames, SDL_Color defaultColor) const {
    StyleValue *rule = nullptr;
    SDL_Color color = SDL_Color();
    for (const char *styleName : styleNames) {
        if (elementStyle->getRule(styleName, &rule)) {
            break;
        }
    }
    if (rule == nullptr) {
        return defaultColor;
    }
    if (!ColorConverter::convert(rule, &color)) {
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

void UIElement::render() {
    if (renderer == nullptr) throw NoRendererException();
    computeLayout();
    // TODO: compute margins
    // TODO: give only a portion of the surface to the elements (and childs)
    renderSelfBeforeChilds();
    renderChilds();
    renderSelfAfterChilds();
    renderBorder();
}

void UIElement::renderBorder() const {
    SDL_Color color;
    Uint8 r, g, b, a;
    SDL_FRect fRect;
    int x, y, w, h;

    if (!SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't get draw color");
        return;
    }

    getRect(&x, &y, &w, &h);

    // set border render order in the README

    // left border
    color = borderLeftColor();
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    fRect = createFRect(x, y, borderLeft(), h);
    SDL_RenderFillRect(renderer, &fRect);

    // right border
    color = borderRightColor();
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    fRect = createFRect(x + w - borderRight(), y, borderRight(), h);
    SDL_RenderFillRect(renderer, &fRect);

    // top border
    color = borderTopColor();
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    fRect = createFRect(x, y, w, borderTop());
    SDL_RenderFillRect(renderer, &fRect);

    // bottom border
    color = borderBottomColor();
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    fRect = createFRect(x, y + h - borderBottom(), w, borderBottom());
    SDL_RenderFillRect(renderer, &fRect);

    // restore previous color
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}