#ifndef TABLE_HPP
#define TABLE_HPP

#include "ui_element.hpp"

class Table : public UIElement {
public:
    Table(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
          std::vector<std::string> *classes = nullptr, const std::string &identifier = "", AbstractElement *parent = nullptr,
          AbstractElement *child = nullptr, AbstractElement *next = nullptr);
    void render() const override;
};

#endif // TABLE_HPP
