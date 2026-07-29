#include "app_state.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

AppState::AppState(gui::element::manager::AbstractManager *manager, gui::elementStyle::manager::StyleManager *styleManager,
                   TTF_TextEngine *textEngine, SDL_Window *sdlWindow, SDL_Renderer *sdlRenderer)
    : _manager{manager}, _styleManager{styleManager}, _textEngine{textEngine}, _sdlWindow{sdlWindow}, _sdlRenderer{sdlRenderer} {}

AppState::~AppState() {
    delete _manager;
    delete _styleManager;
    TTF_DestroyRendererTextEngine(_textEngine);
    SDL_DestroyRenderer(_sdlRenderer);
    SDL_DestroyWindow(_sdlWindow);
}
