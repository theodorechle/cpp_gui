#include "table.hpp"

Table::Table(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
             const std::string &identifier, UIElement *parent, UIElement *child, UIElement *next)
    : UIElement{window, renderer, "table", elementsStyleManager, classes, identifier, parent, child, next} {}

void Table::renderSelfBeforeChilds() const {}

void Table::computeDesiredLayoutWithoutMargins(int *width, int *height) const {}