#include "app_state.hpp"

AppState::AppState(gui::element::manager::AbstractManager *manager, SDL_Window *window, SDL_Renderer *renderer,
                   gui::elementStyle::manager::ElementsStyleManager *styleManager, TTF_TextEngine *textEngine)
    : manager{manager}, styleManager{styleManager}, window{window}, renderer{renderer}, textEngine{textEngine} {}

AppState::~AppState() {
    delete manager;
    delete styleManager;
    TTF_DestroyRendererTextEngine(textEngine);
}