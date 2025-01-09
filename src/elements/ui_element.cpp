#include "ui_element.hpp"

void UIElement::computeDesiredLayout() {}

void UIElement::computeLayout() {
    // int w, h;
    // SDL_GetCurrentRenderOutputSize(renderer, &w, &h);
    // UIElement *child = getChild();
}

void UIElement::getCoords(int *x, int *y) const {
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

void UIElement::setCoords(int x, int y) {
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

void UIElement::render() {
    computeLayout();
    // TODO: compute margins
    renderSelf();
}
