#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

class Label: public UIElement {
public:
    Label(): UIElement{} {}
    void render(SDL_Renderer *renderer) const override;
};

#endif // LABEL_HPP
