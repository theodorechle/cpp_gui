#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "ui_element.hpp"

class Button : public UIElement {
public:
    Button(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
           std::vector<std::string> *classes = nullptr, const std::string &identifier = "", UIElement *parent = nullptr,
           UIElement *child = nullptr, UIElement *next = nullptr);
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // BUTTON_HPP
