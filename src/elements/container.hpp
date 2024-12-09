#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "abstract_container.hpp"

class Container: AbstractContainer {
    void render(SDL_Renderer *renderer) const override;
};

#endif // CONTAINER_HPP
