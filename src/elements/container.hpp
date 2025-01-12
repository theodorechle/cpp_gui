#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "ui_element.hpp"

class Container : public UIElement {
public:
    Container(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
              std::vector<std::string> *classes = nullptr, const std::string &identifier = "", UIElement *parent = nullptr,
              UIElement *child = nullptr, UIElement *next = nullptr);
    void renderSelf() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // CONTAINER_HPP
