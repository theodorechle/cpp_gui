#ifndef SLIDER_HPP
#define SLIDER_HPP

#include "ui_element.hpp"

class Slider: public UIElement {
public:
    Slider(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr, const std::string &identifier = "", AbstractElement *parent = nullptr, AbstractElement *child = nullptr, AbstractElement *next = nullptr) : UIElement{window, renderer, "label", elementsStyleManager, classes, identifier, parent, child, next} {}
    void render() const override;
};

#endif // SLIDER_HPP
