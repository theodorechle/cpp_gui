#include "app_utils/app_state.hpp"
#include "elements/ui/button.hpp"
#include "elements/ui/container.hpp"
#include "elements/ui/input.hpp"
#include "elements/ui/label.hpp"
#include "elements/ui/list.hpp"
#include "elements/abstracts/abstract_manager.hpp"
#include "elements/ui/ui_manager.hpp"
#include "elements/ui/ui_element.hpp"
#include "elements/ui/view_container.hpp"
#include "style_nodes/style_node.hpp"
#include "style_nodes/style_nodes_manager.hpp"
#include "style/style_deserializer.hpp"

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

void displayHelloWorld() { std::cout << "hello world!\n"; }

void displayAlsoHelloWorld() { std::cout << "also hello world!\n"; }

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    int windowLength = 500;
    int windowHeight = 500;
    SDL_Window *sdl_window = nullptr;
    SDL_Renderer *sdl_renderer = nullptr;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL! %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) {
        SDL_Log("Couldn't initialize SDL TTF! %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("GUI Tests", windowLength, windowHeight, SDL_WINDOW_RESIZABLE, &sdl_window, &sdl_renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);

    gui::element::manager::AbstractManager *manager = new gui::element::manager::UIManager(sdl_window, sdl_renderer);

    gui::elementStyle::manager::StyleNodesManager *elementsStyleManager = new gui::elementStyle::manager::StyleNodesManager();
    TTF_TextEngine *textEngine = TTF_CreateRendererTextEngine(sdl_renderer);

    if (textEngine == nullptr) {
        SDL_Log("Can't create a renderer text engine %s", SDL_GetError());
    }

    *appstate = new AppState(manager, sdl_window, sdl_renderer, elementsStyleManager, textEngine);

    elementsStyleManager->setFontsPath("tests/fonts");
    elementsStyleManager->addStyleFile("tests/style_deserialization_tests/tests-files/main-test.txt");

    gui::element::UiElement *parentContainer = new gui::element::List(elementsStyleManager, nullptr, "red-container");
    manager->setElementsTree(parentContainer);

    std::vector<std::string> labelClasses = std::vector<std::string>{"red"};

    parentContainer->addChild(new gui::element::Label("a text rendered (a big big big very really big text)\non multiple lines", elementsStyleManager,
                                                      &labelClasses, "test-label", textEngine));
    parentContainer->addChild(new gui::element::Label("aaaaa", elementsStyleManager, &labelClasses, "aaaaa-label", textEngine));
    gui::element::UiElement *button = new gui::element::Button(displayHelloWorld, elementsStyleManager, {}, "hello-world-button");
    // FIXME: button click zone is bigger than displayed
    parentContainer->addChild(button);
    gui::element::UiElement *list = new gui::element::List(elementsStyleManager);
    button->addChild(list);
    list->addChild(new gui::element::Label("press this button", elementsStyleManager, {}, "", textEngine));
    list->addChild(new gui::element::Label("to display", elementsStyleManager, {}, "", textEngine));
    std::vector<std::string> lastLabelClasses = std::vector<std::string>{"last"};
    list->addChild(new gui::element::Label("hello world!", elementsStyleManager, &lastLabelClasses, "", textEngine));
    parentContainer->addChild(new gui::element::Input("", "type text", elementsStyleManager, {}, "", textEngine));

    gui::element::manager::UIManager *subManager = new gui::element::manager::UIManager(sdl_window, sdl_renderer);

    gui::elementStyle::manager::StyleNodesManager *subElementsStyleManager = new gui::elementStyle::manager::StyleNodesManager();
    subElementsStyleManager->addStyleFile("tests/style_deserialization_tests/tests-files/main-test-sub-view.txt");

    parentContainer->addChild(new gui::element::ViewContainer(subManager, subElementsStyleManager));

    subManager->setElementsTree(new gui::element::Button(displayAlsoHelloWorld, subElementsStyleManager));

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
    TTF_Quit();
}
