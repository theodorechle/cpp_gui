#include "app_state.hpp"

AppState::AppState(gui::element::manager::AbstractManager *manager, SDL_Window *window, SDL_Renderer *renderer,
                   gui::elementStyle::manager::StyleManager *styleManager, TTF_TextEngine *textEngine)
    : _manager{manager}, _styleManager{styleManager}, _window{window}, _renderer{renderer}, _textEngine{textEngine} {}

AppState::~AppState() {
    delete _manager;
    delete _styleManager;
    TTF_DestroyRendererTextEngine(_textEngine);
}