![build status](https://github.com/theodorechle/cpp_gui/actions/workflows/c-cpp.yml/badge.svg)

# cpp_gui
A cpp replacement and improvement of my ![python_gui](https://github.com/theodorechle/python_gui)

This GUI is made on top of the ![SDL3 library](https://wiki.libsdl.org/SDL3/FrontPage)

![Readme for the GUI's style](https://github.com/theodorechle/cpp_style/blob/main/README.md)

# Installation

To use this program, you need g++ and make.

![Install SDL](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md)

![Install SDL_ttf](https://github.com/libsdl-org/SDL_ttf/blob/main/INSTALL.md)

# Demo

Note: For the demo, I'm using the SDL3 callbacks instead of a main function.

To use the GUI, we first need to initialize SDL window and renderer and the SDL ttf:
```cpp
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    int windowLength = 500;
    int windowHeight = 500;
    SDL_Window *sdl_window = nullptr;
    SDL_Renderer *sdl_renderer = nullptr;

    if (!SDL_Init(SDL_INIT_VIDEO)) { // Initialize the graphical part of SDL
        SDL_Log("Couldn't initialize SDL! %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!TTF_Init()) { // Initialize SDL TTF, needed in order to render text
        SDL_Log("Couldn't initialize SDL TTF! %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // create a 500x500 resizable window named GUI Tests and store it in sdl_window
    // also creates a renderer (stored in sdl_renderer) to be able to render anything on the window
    if (!SDL_CreateWindowAndRenderer("GUI Tests", windowLength, windowHeight, SDL_WINDOW_RESIZABLE, &sdl_window, &sdl_renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // allow transparency
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);

    // the text engine must be given to every element who does text rendering
    // it might be simplified in the future so the UiManager handles it itself
    TTF_TextEngine *textEngine = TTF_CreateRendererTextEngine(sdl_renderer);

    if (textEngine == nullptr) {
        SDL_Log("Can't create a renderer text engine %s", SDL_GetError());
    }

    ...

    return SDL_APP_CONTINUE;
```
This configuration part might be done by the GUI automatically in the future, but for now it's necessary.

Now we need to create an element manager. It handles rendering of our elements and the dispatching of the events.
```cpp
    gui::element::manager::AbstractManager *manager = new gui::element::manager::UiManager(sdl_window, sdl_renderer);
```
Note that `UiManager` is the default one, included by the library, but the code infrastructure is made to allow creating customs element managers, as long as they inherit from `gui::element::manager::AbstractManager`.

After that, if we want to be able to apply some style on our elements, we need to instanciate a style manager, who handles it:
```cpp
    // an explanation of how to personalize the configuration may come in the README of the cpp_style library (https://github.com/theodorechle/cpp_style/blob/main/README.md)
    gui::elementStyle::manager::StyleManager *elementsStyleManager = new gui::elementStyle::manager::StyleManager(style::config::config());
    manager->styleManager(elementsStyleManager); // let the element manager know of the style manager so it can use it
```

![More infos about the styling](https://github.com/theodorechle/cpp_style/blob/main/README.md)

Some data should be stored to be able to access them in other SDL callbacks.
For that we can create a custom class who contains all of this data and which we can use as the appstate parameter of the SDL callbacks.

The one used for the demo is:

.hpp
```cpp
class AppState {
    gui::element::manager::AbstractManager *_manager = nullptr;
    gui::elementStyle::manager::StyleManager *_styleManager = nullptr;
    TTF_TextEngine *_textEngine = nullptr;

public:
    AppState(gui::element::manager::AbstractManager *manager, gui::elementStyle::manager::StyleManager *styleManager = nullptr,
             TTF_TextEngine *textEngine = nullptr);
    ~AppState();
    gui::element::manager::AbstractManager *manager() const { return _manager; }
    gui::elementStyle::manager::StyleManager *styleManager() const { return _styleManager; }
};
```
.cpp
```cpp
AppState::AppState(gui::element::manager::AbstractManager *manager, gui::elementStyle::manager::StyleManager *styleManager,
                   TTF_TextEngine *textEngine)
    : _manager{manager}, _styleManager{styleManager} {}

AppState::~AppState() {
    delete _manager;
    delete _styleManager;
    TTF_DestroyRendererTextEngine(_textEngine);
}
```

The data is stored with:
```cpp
    *appstate = new AppState(manager, sdl_window, sdl_renderer, elementsStyleManager, textEngine);
```

We can now create elements!
```cpp
    gui::element::UiElement *parentContainer = new gui::element::List(elementsStyleManager, {}, "red-container");
```
We need to give it our style manager so it can style itself, which should not be the case in the future, since the ui manager should handle it for us.

It also takes a list of classes (used for styling) and an identifier (also used for styling) as parameter.
You can also provide a textEngine, the same one we initialized before.

All parameters are optional, but providing the style manager should always be given, as well as the text engine for elements who renders text (for instance, labels and inputs).

Note that, as for the element manager, the UiElement and its inheritors are only the ones provided by the library, but custom ones can be created by inheriting ```gui::element::AbstractElement```.

An element must be given to the element manager as the root node, like the html element for HTML:
```cpp
    manager->setSubRootElement(parentContainer);
```
This allow the element manager to keep track of our elements and their updates, so it can render them and transmit the events it receives.

Now, to create new elements, we simply need to add them as child of other elements, to create a tree structure.
```cpp
    parentContainer->addChild(new gui::element::Label("some text", elementsStyleManager,
                                                      {"red"}, "test-label", textEngine));
```
Since `parentContainer` is known to the element manager, all of its childs will also be known to it.


After that we need to implements the other SDL callbacks.

In order to have our GUI responding to the SDL events, and not be static, we need to transfer these events to it:
```cpp
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *state = static_cast<AppState *>(appstate);
    gui::element::manager::AbstractManager *manager = state->manager();
    static_cast<gui::element::manager::UiManager *>(manager)->processEvent(event);
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

```
In this function and the next callbacks, we start by retrieving our AppState instance from the appstate parameter.

In this one, we simply get SDL event and give it to the manager. Pretty straightforward.

Also if the event is `SDL_EVENT_QUIT`, we close the app.

That's all for this one.

Now, the next is `SDL_AppIterate`. We use it to ask the element manager to render elements.
It should only refresh updated elements.
```cpp
SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = static_cast<AppState *>(appstate);
    gui::element::manager::AbstractManager *manager = state->manager();
    manager->render();
    return SDL_APP_CONTINUE;
}
```
Again, we don't do much, just asking the manager to render the elements.

The last callback is called when exiting the app, so we can clean everything:
```cpp
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    AppState *state = static_cast<AppState *>(appstate);
    delete state;
    TTF_Quit();
}
```

# Available elements

For below elements, Color in style rules means either:
- a rgb (255, 0, 0) or rgba tuple (255, 0, 0, 0)
- a hexadecimal value:
  - 3-character format: #123 (rgb) (translated in the hoods to the 6 characters format #112233)
  - 6-character format: #123456 (rgb)
  - 8-character format: #12345678 (rgba)

Size means either:
- a pixel size, e.g.: 10px
- a percentage size, e.g.: 10% (/!\ They don't work for now, since a big part of the code was modified and is not anymore compatible)

## UiElement
A basic element which can be rendered on a window with no special behavior.
Think of it as an equivalent of an HTML div.
All elements listed below inherits of it, including its style rules.

Style rules:
- overflow-x: hidden|scroll|auto
- overflow-y: hidden|scroll|auto
- scroll-bar-color: Color
- scroll-bar-size: integer (should be changed soon to a Size)
- margin-left: Size
- margin-right: Size
- margin-top: Size
- margin-bottom: Size
- margin: shorthand to set all margins
- padding-left: Size
- padding-right: Size
- padding-top: Size
- padding-bottom: Size
- padding: shorthand to set all paddings
- border-left: Size
- border-right: Size
- border-top: Size
- border-bottom: Size
- border: shorthand to set all borders
- width: Size
- height: Size
- max-width: Size
- min-width: Size
- max-height: Size
- minèheight: Size
- border-left-color: Color
- border-right-color: Color
- border-top-color: Color
- border-bottom-color: Color
- border-color: shorthand to set all border colors
- background-color: Color

## Button
Can run a function when clicked.
Essentially a UiElement with an event handler on the click:
```cpp
registerEventHandler(ui::event::GuiEventType::EVENT_MOUSE_BUTTON_DOWN, onClick);
```

## Label
An element who displays text.

Style rules:
- font-size: integer
- font-weight: normal|bold
- font-name: string
- font-italic: boolean
- font-underline: boolean
- font-strike-through: boolean
- text-wrap: wrapped|line-break
- horizontal-alignment: start|centered|end
- vertical-alignment: start|centered|end
- text-color: Color

## Input
A label where the text can be overriden by the user.

Style rules: same as label

## Canvas
An element on which you can draw shapes.

For now it only supports drawing rects, but more are coming.

## List
Displays its childs element in a list, one after the other, vertically or horizontally, with optional gap between them.

## Container
Displays its childs one above the other.
Might be removed to become the default behavior of an UiElement.