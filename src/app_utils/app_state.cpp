#include "app_state.hpp"

AppState::AppState(AbstractManager *manager, SDL_Renderer *renderer)
: manager{manager}, renderer{renderer} {

}

AppState::~AppState() {
    delete manager;
}