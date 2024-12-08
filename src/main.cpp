#include "managers/abstract_manager.hpp"
#include "managers/ui_manager.hpp"
#include "elements/ui_element.hpp"
#include "elements/label.hpp"
#include "app_utils/app_state.hpp"

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

void init(AbstractManager *manager) {
    manager->addElement(new Label{});
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    int windowLength = 500;
    int windowHeight = 500;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    AbstractManager *manager = new UIManager();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL! %s", SDL_GetError());
        delete manager;
        return SDL_APP_FAILURE;
    }
    
    if (!SDL_CreateWindowAndRenderer("GUI Tests", windowLength, windowHeight, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        delete manager;
        return SDL_APP_FAILURE;
    }

    init(manager);
    *appstate = new AppState(manager);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate) {
    static_cast<AppState*>(appstate)->getManager()->render();
    return SDL_APP_CONTINUE;
}


void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState*>(appstate);
    delete state;
}
