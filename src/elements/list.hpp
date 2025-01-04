#ifndef LIST_HPP
#define LIST_HPP

#include "ui_element.hpp"

class List : public UIElement {
public:
    List(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
         std::vector<std::string> *classes = nullptr, const std::string &identifier = "", AbstractElement *parent = nullptr,
         AbstractElement *child = nullptr, AbstractElement *next = nullptr);
    void render() const override;
};

#endif // LIST_HPP
