#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "../elements/abstracts/abstract_manager.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

/**
 * Only exists for testing the gui
 */
class AppState {
    gui::element::manager::AbstractManager *_manager = nullptr;
    gui::elementStyle::manager::StyleManager *_styleManager = nullptr;
    TTF_TextEngine *_textEngine = nullptr;
    SDL_Window *_sdlWindow = nullptr;
    SDL_Renderer *_sdlRenderer = nullptr;

public:
    AppState(gui::element::manager::AbstractManager *manager, gui::elementStyle::manager::StyleManager *styleManager, TTF_TextEngine *textEngine,
             SDL_Window *sdlWindow, SDL_Renderer *sdlRenderer);
    ~AppState();
    gui::element::manager::AbstractManager *manager() const { return _manager; }
    gui::elementStyle::manager::StyleManager *styleManager() const { return _styleManager; }
};

#endif // APP_STATE_HPP
