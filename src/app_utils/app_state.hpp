#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "../elements/managers/abstract_manager.hpp"
#include <SDL3/SDL.h>

/**
 * Only exists for testing the gui
 */
class AppState {
    gui::element::manager::AbstractManager *manager;
    gui::elementStyle::manager::ElementsStyleManager *styleManager;
    SDL_Window *window;
    SDL_Renderer *renderer = nullptr;
public:
    AppState(gui::element::manager::AbstractManager *manager, SDL_Window *window, SDL_Renderer *renderer, gui::elementStyle::manager::ElementsStyleManager *styleManager=nullptr);
    ~AppState();
    gui::element::manager::AbstractManager *getManager() const {return manager;}
    gui::elementStyle::manager::ElementsStyleManager *getStyleManager() const {return styleManager;}
    SDL_Window *getWindow() const {return window;}
    SDL_Renderer *getRenderer() const {return renderer;}
};

#endif // APP_STATE_HPP
