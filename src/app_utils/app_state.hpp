#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "../managers/abstract_manager.hpp"
#include <SDL3/SDL.h>

/**
 * Only exists for testing the gui
 */
class AppState {
    AbstractManager *manager = nullptr;
    SDL_Window *window;
    SDL_Renderer *renderer = nullptr;
public:
    AppState(AbstractManager *manager, SDL_Window *window, SDL_Renderer *renderer);
    ~AppState();
    AbstractManager *getManager() const {return manager;}
    SDL_Window *getWindow() const {return window;}
    SDL_Renderer *getRenderer() const {return renderer;}
};

#endif // APP_STATE_HPP
