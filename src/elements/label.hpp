#ifndef LABEL_HPP
#define LABEL_HPP

#include "ui_element.hpp"

class Label : public UIElement {
public:
    Label(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
          std::vector<std::string> *classes = nullptr, const std::string &identifier = "", AbstractElement *parent = nullptr,
          AbstractElement *child = nullptr, AbstractElement *next = nullptr);
    void render() const override;
};

#endif // LABEL_HPP
