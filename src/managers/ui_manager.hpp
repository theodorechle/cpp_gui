#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "../elements/ui_element.hpp"
#include "abstract_manager.hpp"
#include <SDL3/SDL.h>

class UIManager : public AbstractManager {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

public:
    UIManager(SDL_Window *window, SDL_Renderer *renderer) : window{window}, renderer{renderer} {}
    void setElementsTree(UIElement *element);
    UIElement *removeElementsTree();

    using AbstractManager::setElementsTree;
    using AbstractManager::removeElementsTree;

    void render() const override;
};

#endif // UIMANAGER_HPP
