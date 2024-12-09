#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "../managers/abstract_manager.hpp"

class AppState {
    AbstractManager *manager = nullptr;
    SDL_Renderer *renderer = nullptr;
public:
    AppState(AbstractManager *manager, SDL_Renderer *renderer);
    ~AppState();
    AbstractManager *getManager() const {return manager;}
    SDL_Renderer *getRenderer() const {return renderer;}
};

#endif // APP_STATE_HPP
