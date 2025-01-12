#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "../converters/color_converter.hpp"
#include "abstract_element.hpp"

#include <SDL3/SDL.h>
#include <iostream>

class UIElement : public AbstractElement {
    SDL_Rect elementRect = SDL_Rect{};

protected:
    SDL_Window *window;
    SDL_Renderer *renderer;

private:
    void computeLayout() override final;
    virtual void computeDesiredLayoutWithoutMargins(int *width, int *height) const = 0;
    void computeDesiredLayout(int *width, int *height);
    /**
     * If styleName is found in current loaded style, returns the styleName corresponding value.
     * If styleName is not found and globalStyleName is not empty and is found in the loaded style, returns its value.
     * Else returns default;
     */
    int getSize(const std::string &styleName, const std::string &globalStyleName, int defaultSize = 0) const;
    /**
     * If styleName is found in current loaded style, returns the styleName corresponding value.
     * If styleName is not found and globalStyleName is not empty and is found in the loaded style, returns its value.
     * Else returns default;
     */
    SDL_Color getColor(const std::string &styleName, const std::string &globalStyleName, SDL_Color defaultColor = SDL_Color{0, 0, 0, 255}) const;

    void setXpos(int x) { elementRect.x = x; }
    void setYpos(int y) { elementRect.y = y; }
    void setWidth(int width) { elementRect.w = width; }
    void setHeight(int height) { elementRect.h = height; }

    void setPos(int x, int y);
    void setSize(int width, int height);
    void setRect(int x, int y, int width, int height);
    void setRect(const SDL_Rect &rect);

    static SDL_FRect createFRect(int x, int y, int width, int height);

protected:
    /**
     * Override this function to draw self element.
     * Only draw element content (no border)
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

    /**
     * Computes the layout of the element and draw it.
     * Draw the border and call renderSelf to render the element content.
     * To override the rendering, see te renderSelf method.
     */
    void render() override final;

    void renderBorder() const;
};

#endif // UI_ELEMENT_HPP
