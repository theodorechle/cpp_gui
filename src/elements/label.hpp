#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

class Label : public UIElement {
public:
    Label(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
          std::vector<std::string> *classes = nullptr, const std::string &identifier = "", UIElement *parent = nullptr,
          UIElement *child = nullptr, UIElement *next = nullptr);
    void renderSelf() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // LABEL_HPP
