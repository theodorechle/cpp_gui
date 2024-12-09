#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

class Label: public UIElement {
public:
    Label(SDL_Renderer *renderer): UIElement{renderer} {}
    void render() const override;
};

#endif // LABEL_HPP
