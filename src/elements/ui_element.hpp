#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "../converters/color_converter.hpp"
#include "abstract_element.hpp"

#include <SDL3/SDL.h>
#include <iostream>
#include <vector>

class UIElement : public AbstractElement {
    SDL_Rect elementRect = SDL_Rect{};
    SDL_Renderer *renderer = nullptr;

    void computeLayout() override final;
    virtual void computeDesiredLayoutWithoutMargins(int *width, int *height) const = 0;
    void computeDesiredLayout(int *width, int *height);

    /**
     * If any of the style names is found in current loaded style, returns the corresponding value.
     * Else returns default;
     */
    int getSize(const std::vector<const char *> &styleNames, int defaultSize = 0) const;

    /**
     * If any of the style names is found in current loaded style, returns the corresponding value.
     * Else returns default;
     */
    SDL_Color getColor(const std::vector<const char *> &styleNames, SDL_Color defaultColor = SDL_Color{0, 0, 0, 255}) const;

    void setXpos(int x) { elementRect.x = x; }
    void setYpos(int y) { elementRect.y = y; }
    void setWidth(int width) { elementRect.w = width; }
    void setHeight(int height) { elementRect.h = height; }

    void setPos(int x, int y);
    void setSize(int width, int height);
    void setRect(int x, int y, int width, int height);
    void setRect(const SDL_Rect &rect);

    static SDL_FRect createFRect(int x, int y, int width, int height);
    void setParent(UIElement *parent) { AbstractElement::setParent(parent); }

public:
    UIElement(std::string elementName, ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr,
              const std::string &identifier = "")
        : AbstractElement{elementName, elementsStyleManager, classes, identifier} {}

    UIElement *getParent() { return static_cast<UIElement *>(AbstractElement::getParent()); }
    void addChild(UIElement *child);
    UIElement *getChild() { return static_cast<UIElement *>(AbstractElement::getChild()); }
    void setNext(UIElement *next) { AbstractElement::setNext(next); }
    UIElement *getNext() { return static_cast<UIElement *>(AbstractElement::getNext()); }

    void setRenderer(SDL_Renderer *renderer) { this->renderer = renderer; }
    SDL_Renderer *getRenderer() const { return renderer; }

    int getXPos() const { return elementRect.x; }
    int getYPos() const { return elementRect.y; }
    int getWidth() const { return elementRect.w; }
    int getHeight() const { return elementRect.h; }

    void getPos(int *x, int *y) const;
    void getSize(int *width, int *height) const;
    void getRect(int *x, int *y, int *width, int *height) const;
    SDL_Rect getRect() const { return elementRect; };

    int marginLeft() const;
    int marginRight() const;
    int marginTop() const;
    int marginBottom() const;

    int paddingLeft() const;
    int paddingRight() const;
    int paddingTop() const;
    int paddingBottom() const;

    int borderLeft() const;
    int borderRight() const;
    int borderTop() const;
    int borderBottom() const;

    SDL_Color borderLeftColor() const;
    SDL_Color borderRightColor() const;
    SDL_Color borderTopColor() const;
    SDL_Color borderBottomColor() const;

    SDL_Color backgroundColor() const;

    /**
     * Computes the layout of the element and draw it.
     * Draw the border and call renderSelf to render the element content.
     * To override the rendering, see te renderSelf method.
     */
    void render() override final;

    void renderBackground() const;
    void renderBorder() const;
};

class NoRendererException : public std::logic_error {
public:
    NoRendererException() : std::logic_error{"No renderer available"} {};
};

#endif // UI_ELEMENT_HPP
