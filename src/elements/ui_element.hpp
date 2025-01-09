#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "abstract_element.hpp"

#include <SDL3/SDL.h>
#include <iostream>

class UIElement : public AbstractElement {
protected:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect elementRect = SDL_Rect{};

private:
    void computeDesiredLayout();
    void computeLayout() override final;

protected:
    /**
     * Override this function to draw self element instead of render
     */
    virtual void renderSelf() const = 0;

public:
    UIElement(SDL_Window *window, SDL_Renderer *renderer, std::string elementName, ElementsStyleManager *elementsStyleManager = nullptr,
              std::vector<std::string> *classes = nullptr, const std::string &identifier = "", UIElement *parent = nullptr,
              UIElement *child = nullptr, UIElement *next = nullptr)
        : AbstractElement{elementName, elementsStyleManager, classes, identifier, parent, child, next}, window{window}, renderer{renderer} {}

    void setParent(UIElement *parent) { AbstractElement::setParent(parent); }
    UIElement *getParent() { return static_cast<UIElement *>(AbstractElement::getParent()); }
    void setChild(UIElement *child) { AbstractElement::setChild(child); }
    UIElement *getChild() { return static_cast<UIElement *>(AbstractElement::getChild()); }
    void setNext(UIElement *next) { AbstractElement::setNext(next); }
    UIElement *getNext() { return static_cast<UIElement *>(AbstractElement::getNext()); }

    int getXpos() { return elementRect.x; }
    int getYpos() { return elementRect.y; }
    int getWidth() { return elementRect.w; }
    int getHeight() { return elementRect.h; }

    void getCoords(int *x, int *y) const;
    void getSize(int *width, int *height) const;
    void getRect(int *x, int *y, int *width, int *height) const;

    void setXpos(int x) { elementRect.x = x; }
    void setYpos(int y) { elementRect.y = y; }
    void setWidth(int width) { elementRect.w = width; }
    void setHeight(int height) { elementRect.h = height; }

    void setCoords(int x, int y);
    void setSize(int width, int height);
    void setRect(int x, int y, int width, int height);

    /**
     * Computes the layout of the element and draw it.
     * To override the rendering, see te renderSelf method.
     */
    void render() override final;
};

#endif // UI_ELEMENT_HPP
