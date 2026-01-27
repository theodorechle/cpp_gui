#include "../cpp_style/src/style_deserializer.hpp"
#include "app_utils/app_state.hpp"
#include "element_style/element_style.hpp"
#include "element_style/style_manager.hpp"
#include "elements/abstracts/abstract_manager.hpp"
#include "elements/ui/button.hpp"
#include "elements/ui/container.hpp"
#include "elements/ui/input.hpp"
#include "elements/ui/label.hpp"
#include "elements/ui/list.hpp"
#include "elements/ui/style_config.hpp"
#include "elements/ui/ui_element.hpp"
#include "elements/ui/ui_manager.hpp"
#include "elements/ui/view_container.hpp"

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <SDL3_ttf/SDL_ttf.h>

void displayHelloWorld(const gui::element::event::Event *) { std::cout << "hello world!\n"; }

void displayAlsoHelloWorld(const gui::element::event::Event *) { std::cout << "also hello world!\n"; }

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
#ifdef DEBUG
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_DEBUG);
#endif
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

    gui::element::manager::AbstractManager *manager = new gui::element::manager::UiManager(sdl_window, sdl_renderer);

    gui::elementStyle::manager::StyleManager *elementsStyleManager = new gui::elementStyle::manager::StyleManager(style::config::config());
    manager->styleManager(elementsStyleManager);
    TTF_TextEngine *textEngine = TTF_CreateRendererTextEngine(sdl_renderer);

    if (textEngine == nullptr) {
        SDL_Log("Can't create a renderer text engine %s", SDL_GetError());
    }

    *appstate = new AppState(manager, sdl_window, sdl_renderer, elementsStyleManager, textEngine);

    elementsStyleManager->addDefaultFontPath("tests/fonts");
    elementsStyleManager->addStyleFile("tests/tests-files/main-test.txt");

    gui::element::UiElement *parentContainer = new gui::element::List(elementsStyleManager, nullptr, "red-container");
    manager->setSubRootElement(parentContainer);

    std::vector<std::string> labelClasses = std::vector<std::string>{"red"};

    parentContainer->addChild(new gui::element::Label("a text rendered (a big big big very really big text)\non multiple lines", elementsStyleManager,
                                                      &labelClasses, "test-label", textEngine));
    parentContainer->addChild(new gui::element::Label("an other text rendered (a big big big very really big text)\non multiple lines",
                                                      elementsStyleManager, &labelClasses, "test-label", textEngine));
    parentContainer->addChild(new gui::element::Label("aaaaa", elementsStyleManager, &labelClasses, "aaaaa-label", textEngine));
    gui::element::UiElement *button = new gui::element::Button(displayHelloWorld, elementsStyleManager, {}, "hello-world-button");
    parentContainer->addChild(button);
    gui::element::UiElement *list = new gui::element::List(elementsStyleManager);
    button->addChild(list);
    list->addChild(new gui::element::Label("press this button", elementsStyleManager, {}, "", textEngine));
    list->addChild(new gui::element::Label("to display", elementsStyleManager, {}, "", textEngine));
    std::vector<std::string> lastLabelClasses = std::vector<std::string>{"last"};
    list->addChild(new gui::element::Label("hello world!", elementsStyleManager, &lastLabelClasses, "", textEngine));
    parentContainer->addChild(new gui::element::Input("", "type text", elementsStyleManager, {}, "", textEngine));

    // gui::element::manager::UiManager *subManager = new gui::element::manager::UiManager(sdl_window, sdl_renderer);

    // gui::elementStyle::manager::StyleManager *subElementsStyleManager = new
    // gui::elementStyle::manager::StyleManager(&guiStyleConfig);
    // subElementsStyleManager->addStyleFile("tests/tests-files/main-test-sub-view.txt");

    // parentContainer->addChild(new gui::element::ViewContainer(subManager, subElementsStyleManager));

    // subManager->setSubRootElement(new gui::element::Button(displayAlsoHelloWorld, subElementsStyleManager));

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *state = static_cast<AppState *>(appstate);
    gui::element::manager::AbstractManager *manager = state->manager();
    static_cast<gui::element::manager::UiManager *>(manager)->processEvent(event);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = static_cast<AppState *>(appstate);
    gui::element::manager::AbstractManager *manager = state->manager();
    manager->render();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState *>(appstate);
    delete state;
    TTF_Quit();
}
