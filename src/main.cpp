#include "app_utils/app_state.hpp"
#include "elements/container.hpp"
#include "elements/label.hpp"
#include "elements/managers/abstract_manager.hpp"
#include "elements/managers/ui_manager.hpp"
#include "elements/ui_element.hpp"
#include "elements_style/element_style.hpp"
#include "elements_style/managers/elements_style_manager.hpp"
#include "style/style_deserializer.hpp"

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// constexpr char const *DEFAULT_STYLE_FILE = "src/default-style.txt";

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

    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);

    gui::element::manager::AbstractManager *manager = new gui::element::manager::UIManager(sdl_window, sdl_renderer);

    gui::elementStyle::manager::ElementsStyleManager *elementsStyleManager = new gui::elementStyle::manager::ElementsStyleManager();

    *appstate = new AppState(manager, sdl_window, sdl_renderer, elementsStyleManager);

    elementsStyleManager->addStyleFile("tests/style_tests/tests-files/main-test.txt");

    gui::element::UIElement *parentContainer = new gui::element::Container(elementsStyleManager, nullptr, "red-container");
    parentContainer->setRenderer(sdl_renderer);
    manager->setElementsTree(parentContainer);

    std::vector<std::string> labelClasses = std::vector<std::string>{"red"};

    parentContainer->addChild(new gui::element::Label(elementsStyleManager, &labelClasses, "test-label"));

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *state = static_cast<AppState *>(appstate);
    gui::element::manager::AbstractManager *manager = state->getManager();
    static_cast<gui::element::manager::UIManager *>(manager)->processEvent(*event);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = static_cast<AppState *>(appstate);
    gui::element::manager::AbstractManager *manager = state->getManager();
    manager->render();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState *>(appstate);
    delete state;
}
