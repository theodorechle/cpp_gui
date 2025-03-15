#ifndef APP_STATE_HPP
#define APP_STATE_HPP

#include "../elements/abstracts/abstract_manager.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

/**
 * Only exists for testing the gui
 */
class AppState {
    gui::element::manager::AbstractManager *manager;
    gui::elementStyle::manager::StyleNodesManager *styleManager;
    SDL_Window *window;
    SDL_Renderer *renderer = nullptr;
    TTF_TextEngine *textEngine;

public:
    AppState(gui::element::manager::AbstractManager *manager, SDL_Window *window, SDL_Renderer *renderer,
             gui::elementStyle::manager::StyleNodesManager *styleManager = nullptr, TTF_TextEngine *textEngine = nullptr);
    ~AppState();
    gui::element::manager::AbstractManager *getManager() const { return manager; }
    gui::elementStyle::manager::StyleNodesManager *getStyleManager() const { return styleManager; }
    SDL_Window *getWindow() const { return window; }
    SDL_Renderer *getRenderer() const { return renderer; }
    TTF_TextEngine *getTextEngine() const { return textEngine; }
};

#endif // APP_STATE_HPP
