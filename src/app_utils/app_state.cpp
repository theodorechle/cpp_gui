#include "app_state.hpp"

AppState::AppState(AbstractManager *manager, SDL_Window *window, SDL_Renderer *renderer)
: manager{manager}, window{window}, renderer{renderer} {

}

AppState::~AppState() {
    delete manager;
}