#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

class Label: public UIElement {
public:
    Label(SDL_Window *window, SDL_Renderer *renderer): UIElement{window, renderer} {}
    void render() const override;
};

#endif // LABEL_HPP
