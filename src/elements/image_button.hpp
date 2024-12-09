#ifndef IMAGE_BUTTON_HPP
#define IMAGE_BUTTON_HPP

#include "button.hpp"

class ImageButton: Button {
    void render(SDL_Renderer *renderer) const override;
};

#endif // IMAGE_BUTTON_HPP
