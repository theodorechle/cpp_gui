![build status](https://github.com/theodorechle/cpp_gui/actions/workflows/c-cpp.yml/badge.svg)

# cpp_gui
A cpp replacement and improvement of my ![python_gui](https://github.com/theodorechle/python_gui)

This GUI is made on top of the ![SDL3 library](https://wiki.libsdl.org/SDL3/FrontPage)

![Readme for the gui's style](src/style/README.md)

# Installation

To use this program, you need g++ and make.

![Install SDL](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md)

![Install SDL_ttf](https://github.com/libsdl-org/SDL_ttf/blob/main/INSTALL.md)

## Elements
The gui is designed so you can implements easily your own elements

### Abstracts
- <i>AbstractElement</i>
- <i>UiElement</i> (inherits the AbstractElement class)

### Concretes (they all inherits the UiElement class)
- Label
- Input
- Container
- Button
- TextButton
- ImageButton
- List
- Table
- Slider


### Default styles:
- UiElement
    - background-color: string|tuple[int,int,int]
    - border-color: string|tuple[int,int,int]
    - border-width: int
    - // TODO border-radius: int
    - // TODO border-top-left-radius: int
    - // TODO border-top-right-radius: int
    - // TODO border-bottom-left-radius: int
    - // TODO border-bottom-right-radius: int
    - height: int
    - min-height: int
    - max-height: int
    - width: int
    - min-width: int
    - max-width: int
    - padding: int
    - overflow: hidden|scroll
    - overflow-x: hidden|scroll
    - overflow-y: hidden|scroll
    - scroll-bar: scroll|always
- Label
    - text-color: string|tuple[int,int,int]
    - font: string
    - font-size: int
    - horizontal-alignment: start|centered|end
    - vertical-alignment: start|centered|end
    - // TODO antialias: bool
- Input // in progress
    - // TODO caret-color: string|tuple[int,int,int]
    - // TODO placeholder-color: string|tuple[int,int,int]
- // TODO Slider
    - // TODO bar-color: string|tuple[int,int,int]
    - // TODO cursor-color: string|tuple[int,int,int]
- Container:
- List
    - vertical: bool
    - childs-size: biggest|keep
    - gap: int
- Button


## Manager
It chooses when its elements rendering is computed and when they are rendered.
It knows the window size and state (focused, ...).
It activates and deactivates the style modifiers.

### Abstract
- <i>AbstractManager</i>

### Concrete
- UIManager
