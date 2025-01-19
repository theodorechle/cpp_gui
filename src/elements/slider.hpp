#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "ui_element.hpp"

class Slider : public UIElement {
public:
    Slider(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
           std::vector<std::string> *classes = nullptr, const std::string &identifier = "", UIElement *parent = nullptr,
           UIElement *child = nullptr, UIElement *next = nullptr);
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // SLIDER_HPP
