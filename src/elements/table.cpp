#include "table.hpp"

Table::Table(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager, std::vector<std::string> *classes,
             const std::string &identifier, AbstractElement *parent, AbstractElement *child, AbstractElement *next)
    : UIElement{window, renderer, "table", elementsStyleManager, classes, identifier, parent, child, next} {}

void Table::render() const {}