#include "ui_element.hpp"

void UIElement::computeLayout() {
    
}

void UIElement::getCoords(int *x, int *y) const {
    *x = getXpos();
    *y = getYpos();
}

void UIElement::getSize(int *width, int *height) const {
    *width = getWidth();
    *height = getHeight();
}

void UIElement::getRect(int *x, int *y, int *width, int *height) const {
    *x = getXpos();
    *y = getYpos();
    *width = getWidth();
    *height = getHeight();
}

void UIElement::setCoords(int *x, int *y) const {
    setXpos(*x);
    setYpos(*y);
}

void UIElement::setSize(int *width, int *height) const {
    setWidth(*width);
    setHeight(*height);
}

void UIElement::setRect(int *x, int *y, int *width, int *height) const {
    setXpos(*x);
    setYpos(*y);
    setWidth(*width);
    setHeight(*height);
}