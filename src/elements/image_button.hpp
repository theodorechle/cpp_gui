#ifndef IMAGE_BUTTON_HPP
#define IMAGE_BUTTON_HPP

#include "button.hpp"

class ImageButton : public UIElement {
public:
    ImageButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr, std::vector<std::string> *classes = nullptr, const std::string &identifier = "", AbstractElement *parent = nullptr, AbstractElement *child = nullptr, AbstractElement *next = nullptr) : UIElement{window, renderer, "label", elementsStyleManager, classes, identifier, parent, child, next} {}
    void render() const override;
};

#endif // IMAGE_BUTTON_HPP
