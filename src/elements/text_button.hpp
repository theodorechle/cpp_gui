#ifndef TEXT_BUTTON_HPP
#define TEXT_BUTTON_HPP

#include "button.hpp"

class TextButton : public UIElement {
public:
    TextButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
               std::vector<std::string> *classes = nullptr, const std::string &identifier = "", AbstractElement *parent = nullptr,
               AbstractElement *child = nullptr, AbstractElement *next = nullptr);
    void render() const override;
};

#endif // TEXT_BUTTON_HPP
