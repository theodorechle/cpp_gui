#include "managers/abstract_manager.hpp"
#include "managers/ui_manager.hpp"
#include "elements/ui_element.hpp"
#include "elements/label.hpp"
#include "app_utils/app_state.hpp"

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    int windowLength = 500;
    int windowHeight = 500;
    SDL_Window *sdl_window = nullptr;
    SDL_Renderer *sdl_renderer = nullptr;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL! %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    
    if (!SDL_CreateWindowAndRenderer("GUI Tests", windowLength, windowHeight, SDL_WINDOW_RESIZABLE, &sdl_window, &sdl_renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AbstractManager *manager = new UIManager(sdl_window, sdl_renderer);

    *appstate = new AppState(manager, sdl_window, sdl_renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = static_cast<AppState*>(appstate);
    AbstractManager *manager = state->getManager();
    manager->addElement(new Label{state->getWindow(), state->getRenderer()});
    manager->render();
    SDL_RenderPresent(state->getRenderer());
    manager->removeAllElements();

    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState*>(appstate);
    delete state;
}
