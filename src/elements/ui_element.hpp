#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "abstract_element.hpp"

#include <iostream>

class UIElement: AbstractElement {
protected:
    SDL_Renderer *renderer;
    SDL_Rect elementRect = SDL_Rect{};
private:
    void computeLayout() override final;
public:
    UIElement(SDL_Renderer *renderer): renderer{renderer} {}
    int getXpos() {return elementRect.x;}
    int getYpos() {return elementRect.y;}
    int getWidth() {return elementRect.w;}
    int getHeight() {return elementRect.h;}

    void getCoords(int *x, int *y) const;
    void getSize(int *width, int *height) const;
    void getRect(int *x, int *y, int *width, int *height) const;

    void setXpos(int x) {elementRect.x = x;}
    void setYpos(int y) {elementRect.y = y;}
    void setWidth(int width) {elementRect.w = width;}
    void setHeight(int height) {elementRect.h = height;}
    
    void setCoords(int x, int y);
    void setSize(int width, int height);
    void setRect(int x, int y, int width, int height);
};

#endif // UI_ELEMENT_HPP
