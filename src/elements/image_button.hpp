#ifndef IMAGE_BUTTON_HPP
#define IMAGE_BUTTON_HPP

#include "button.hpp"

class ImageButton : public UIElement {
public:
    ImageButton(SDL_Window *window, SDL_Renderer *renderer, ElementsStyleManager *elementsStyleManager = nullptr,
                std::vector<std::string> *classes = nullptr, const std::string &identifier = "", UIElement *parent = nullptr,
                UIElement *child = nullptr, UIElement *next = nullptr);
    void renderSelfBeforeChilds() const override;
    void computeDesiredLayoutWithoutMargins(int *width, int *height) const override;
};

#endif // IMAGE_BUTTON_HPP
