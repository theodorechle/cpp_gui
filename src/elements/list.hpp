#ifndef LIST_HPP
#define LIST_HPP

#include "ui_element.hpp"

class List : public UIElement {
public:
    List(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
         std::vector<std::string> *classes = nullptr, const std::string &identifier = "");
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // LIST_HPP
