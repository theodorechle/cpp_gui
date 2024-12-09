#ifndef UI_ELEMENT_HPP
#define UI_ELEMENT_HPP

#include "abstract_element.hpp"

#include <iostream>

class UIElement: AbstractElement {
protected:
    SDL_Renderer *renderer;
private:
    void computeLayout() override final;
public:
    UIElement(SDL_Renderer *renderer): renderer{renderer} {}
    virtual int getXpos() const = 0;
    virtual int getYpos() const = 0;
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

    /**
     * Automatically computed using getXPos and getYPos.
     * Can be overridden for performance (reduce function calls) or to recalculate the coords each call
     */
    virtual void getCoords(int *x, int *y) const;
  
    /**
     * Automatically computed using getWidth and getHeight.
     * Can be overridden for performance (reduce function calls) or to recalculate the size each call
     */
    virtual void getSize(int *width, int *height) const;
  
    /**
     * Automatically computed using getXPos, getYPos, getWidth and getHeight.
     * Can be overridden for performance (reduce function calls) or to recalculate the rect each call
     */
    virtual void getRect(int *x, int *y, int *width, int *height) const;

    virtual void setXpos(int x) const = 0;
    virtual void setYpos(int y) const = 0;
    virtual void setWidth(int width) const = 0;
    virtual void setHeight(int height) const = 0;

    /**
     * Automatically computed using setXpos and setYpos.
     * Can be overridden for performance (reduce function calls)
     */
    virtual void setCoords(int *x, int *y) const;
  
    /**
     * Automatically computed using setWidth and setHeight.
     * Can be overridden for performance (reduce function calls)
     */
    virtual void setSize(int *width, int *height) const;
  
    /**
     * Automatically computed using setXpos, setYpos, setWidth and setHeight.
     * Can be overridden for performance (reduce function calls)
     */
    virtual void setRect(int *x, int *y, int *width, int *height) const;
};

#endif // UI_ELEMENT_HPP
