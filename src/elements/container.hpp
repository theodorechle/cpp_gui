#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "ui_element.hpp"

class Container : public UIElement {
public:
    Container(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr, const std::string &identifier = "", AbstractElement *parent = nullptr, AbstractElement *child = nullptr, AbstractElement *next = nullptr) : UIElement{window, renderer, "label", elementsStyleManager, classes, identifier, parent, child, next} {}
    void render() const override;
};

#endif // CONTAINER_HPP
