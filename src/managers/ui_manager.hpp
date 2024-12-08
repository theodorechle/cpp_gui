#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "abstract_manager.hpp"

class UIManager: public AbstractManager {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
public:
    UIManager(SDL_Window *window, SDL_Renderer *renderer): window{window}, renderer{renderer} {}
    void render() const override;
};

#endif // UIMANAGER_HPP
